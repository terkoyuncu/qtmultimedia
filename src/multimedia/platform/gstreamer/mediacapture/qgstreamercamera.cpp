/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qcamerainfo.h>

#include "qgstreamercamera_p.h"
#include "qgstreamercameraimagecapture_p.h"
#include <private/qgstreamerdevicemanager_p.h>
#include <private/qgstreamerintegration_p.h>

#include <QtCore/qdebug.h>

QGstreamerCamera::QGstreamerCamera(QGstreamerMediaCapture *session)
    : QPlatformCamera(session),
      m_session(session),
      gstCameraBin("camerabin")
{
    gstCamera = QGstElement("videotestsrc");
    gstVideoConvert = QGstElement("videoconvert", "videoConvert");
    gstVideoScale = QGstElement("videoscale", "videoScale");
    gstCameraBin.add(gstCamera, gstVideoConvert, gstVideoScale);
    gstCamera.link(gstVideoConvert, gstVideoScale);

    gstCameraBin.addGhostPad(gstVideoScale, "src");
}

QGstreamerCamera::~QGstreamerCamera() = default;

bool QGstreamerCamera::isActive() const
{
    return m_active;
}

void QGstreamerCamera::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;

    emit activeChanged(active);
}

void QGstreamerCamera::setCamera(const QCameraInfo &camera)
{
    if (m_cameraInfo == camera)
        return;

    auto state = gstPipeline.state();
    gstPipeline.setStateSync(GST_STATE_NULL); // ### Can we do it pausing only????

    Q_ASSERT(!gstCamera.isNull());

    gstCamera.setStateSync(GST_STATE_NULL);
    gstCameraBin.remove(gstCamera);

    if (camera.isNull()) {
        gstCamera = QGstElement("videotestsrc");
    } else {
        auto *deviceManager = static_cast<QGstreamerDeviceManager *>(QGstreamerIntegration::instance()->deviceManager());
        auto *device = deviceManager->videoDevice(camera.id());
        gstCamera = gst_device_create_element(device, "camerasrc");
    }

    gstCameraBin.add(gstCamera);
    gstCamera.link(gstVideoConvert);

    gstCamera.setStateSync(GST_STATE_PAUSED);

    gstPipeline.setStateSync(state);

    m_session->cameraChanged();
}


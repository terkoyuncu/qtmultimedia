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


#ifndef QANDROIDCAMERACONTROL_H
#define QANDROIDCAMERACONTROL_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qplatformcamera_p.h>

#include <qmap.h>

QT_BEGIN_NAMESPACE

class QAndroidCameraSession;
class QAndroidCameraVideoRendererControl;
class QAndroidMediaCaptureSession;

class QAndroidCamera : public QPlatformCamera
{
    Q_OBJECT
public:
    explicit QAndroidCamera(QCamera *camera);
    virtual ~QAndroidCamera();

    bool isActive() const override;
    void setActive(bool active) override;

    void setCamera(const QCameraDevice &camera) override;
    bool setCameraFormat(const QCameraFormat &format) override;

    void setCaptureSession(QPlatformMediaCaptureSession *session) override;

    void setFocusMode(QCamera::FocusMode mode) override;
    bool isFocusModeSupported(QCamera::FocusMode mode) const override;

    void zoomTo(float factor, float rate) override;

    void setFlashMode(QCamera::FlashMode mode) override;
    bool isFlashModeSupported(QCamera::FlashMode mode) const override;
    bool isFlashReady() const override;

    void setTorchMode(QCamera::TorchMode mode) override;
    bool isTorchModeSupported(QCamera::TorchMode mode) const override;

    void setExposureMode(QCamera::ExposureMode mode) override;
    bool isExposureModeSupported(QCamera::ExposureMode mode) const override;

    void setExposureCompensation(float bias) override;

    bool isWhiteBalanceModeSupported(QCamera::WhiteBalanceMode mode) const override;
    void setWhiteBalanceMode(QCamera::WhiteBalanceMode mode) override;

private Q_SLOTS:
    void onCameraOpened();
    void setCameraFocusArea();

private:
    void reactivateCameraSession();

    QAndroidCameraSession *m_cameraSession = nullptr;
    QAndroidMediaCaptureSession *m_service = nullptr;

    QList<QCamera::FocusMode> m_supportedFocusModes;
    bool m_continuousPictureFocusSupported = false;
    bool m_continuousVideoFocusSupported = false;
    bool m_focusPointSupported = false;

    float m_maximumZoom;
    QList<int> m_zoomRatios;

    QList<QCamera::ExposureMode> m_supportedExposureModes;
    int m_minExposureCompensationIndex;
    int m_maxExposureCompensationIndex;
    qreal m_exposureCompensationStep;

    bool isFlashSupported = false;
    bool isFlashAutoSupported = false;
    bool isTorchSupported = false;
    QCameraDevice m_cameraDev;

    QMap<QCamera::WhiteBalanceMode, QString> m_supportedWhiteBalanceModes;
    QCameraFormat m_cameraFormat;
};


QT_END_NAMESPACE

#endif // QANDROIDCAMERACONTROL_H

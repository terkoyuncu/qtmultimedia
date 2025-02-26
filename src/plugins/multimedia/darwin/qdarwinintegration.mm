/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
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

#include "qdarwinintegration_p.h"
#include <avfmediaplayer_p.h>
#include <avfcameraservice_p.h>
#include <avfcamera_p.h>
#include <avfimagecapture_p.h>
#include <avfmediaencoder_p.h>
#include <qdarwinformatsinfo_p.h>
#include <avfvideosink_p.h>
#include <avfaudiodecoder_p.h>
#include <VideoToolbox/VideoToolbox.h>
#include <qdebug.h>
#include <private/qplatformmediaplugin_p.h>
#include <qavfcamerabase_p.h>

QT_BEGIN_NAMESPACE

class QDarwinMediaPlugin : public QPlatformMediaPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformMediaPlugin_iid FILE "darwin.json")

public:
    QDarwinMediaPlugin()
        : QPlatformMediaPlugin()
    {}

    QPlatformMediaIntegration* create(const QString &name) override
    {
        if (name == QLatin1String("darwin"))
            return new QDarwinIntegration;
        return nullptr;
    }
};


QDarwinIntegration::QDarwinIntegration()
{
#if defined(Q_OS_MACOS) && QT_MACOS_PLATFORM_SDK_EQUAL_OR_ABOVE(__MAC_11_0)
    if (__builtin_available(macOS 11.0, *))
        VTRegisterSupplementalVideoDecoderIfAvailable(kCMVideoCodecType_VP9);
#endif
    m_videoDevices = new QAVFVideoDevices(this);
}

QDarwinIntegration::~QDarwinIntegration()
{
    delete m_formatInfo;
}

QPlatformMediaFormatInfo *QDarwinIntegration::formatInfo()
{
    if (!m_formatInfo)
        m_formatInfo = new QDarwinFormatInfo();
    return m_formatInfo;
}

QPlatformAudioDecoder *QDarwinIntegration::createAudioDecoder(QAudioDecoder *decoder)
{
    return new AVFAudioDecoder(decoder);
}

QPlatformMediaCaptureSession *QDarwinIntegration::createCaptureSession()
{
    return new AVFCameraService;
}

QPlatformMediaPlayer *QDarwinIntegration::createPlayer(QMediaPlayer *player)
{
    return new AVFMediaPlayer(player);
}

QPlatformCamera *QDarwinIntegration::createCamera(QCamera *camera)
{
    return new AVFCamera(camera);
}

QPlatformMediaRecorder *QDarwinIntegration::createRecorder(QMediaRecorder *recorder)
{
    return new AVFMediaEncoder(recorder);
}

QPlatformImageCapture *QDarwinIntegration::createImageCapture(QImageCapture *imageCapture)
{
    return new AVFImageCapture(imageCapture);
}

QPlatformVideoSink *QDarwinIntegration::createVideoSink(QVideoSink *sink)
{
    return new AVFVideoSink(sink);
}

QT_END_NAMESPACE

#include "qdarwinintegration.moc"

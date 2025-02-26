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

#include "qffmpeghwaccel_p.h"
#if QT_CONFIG(vaapi)
#include "qffmpeghwaccel_vaapi_p.h"
#endif
#ifdef Q_OS_DARWIN
#include "qffmpeghwaccel_videotoolbox_p.h"
#endif
#if QT_CONFIG(wmf)
#include "qffmpeghwaccel_d3d11_p.h"
#endif
#ifdef Q_OS_ANDROID
#    include "qffmpeghwaccel_mediacodec_p.h"
#endif
#include "qffmpeg_p.h"
#include "qffmpegvideobuffer_p.h"

#include <private/qrhi_p.h>
#include <qloggingcategory.h>


/* Infrastructure for HW acceleration goes into this file. */

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(qLHWAccel, "qt.multimedia.ffmpeg.hwaccel");

namespace QFFmpeg {

// HW context initialization

// preferred order of HW accelerators to use
static const AVHWDeviceType preferredHardwareAccelerators[] = {
// Linux/Unix
#if defined(Q_OS_LINUX)
    AV_HWDEVICE_TYPE_VAAPI,
//    AV_HWDEVICE_TYPE_DRM,
#elif defined (Q_OS_WIN)
    AV_HWDEVICE_TYPE_D3D11VA,
#elif defined (Q_OS_DARWIN)
    AV_HWDEVICE_TYPE_VIDEOTOOLBOX,
#elif defined (Q_OS_ANDROID)
    AV_HWDEVICE_TYPE_MEDIACODEC,
#endif
    AV_HWDEVICE_TYPE_NONE
};

static AVBufferRef *loadHWContext(const AVHWDeviceType type)
{
    AVBufferRef *hwContext = nullptr;
    int ret = av_hwdevice_ctx_create(&hwContext, type, nullptr, nullptr, 0);
    qCDebug(qLHWAccel) << "    Checking HW context:" << av_hwdevice_get_type_name(type);
    if (ret == 0) {
        qCDebug(qLHWAccel) << "    Using above hw context.";
        return hwContext;
    }
    qCDebug(qLHWAccel) << "    Could not create hw context:" << ret << strerror(-ret);
    return nullptr;
}

static AVBufferRef *hardwareContextForCodec(const AVCodec *codec)
{
    qCDebug(qLHWAccel) << "Checking HW acceleration for decoder" << codec->name;

    // First try our preferred accelerators. Those are the ones where we can
    // set up a zero copy pipeline
    auto *preferred = preferredHardwareAccelerators;
    while (*preferred != AV_HWDEVICE_TYPE_NONE) {
        for (int i = 0;; ++i) {
            const AVCodecHWConfig *config = avcodec_get_hw_config(codec, i);
            if (!config)
                break;
            if (config->device_type == *preferred) {
                auto *hwContext = loadHWContext(config->device_type);
                if (hwContext)
                    return hwContext;
                break;
            }
        }
        ++preferred;
    }

    // Ok, let's see if we can get any HW acceleration at all. It'll still involve one buffer copy,
    // as we can't move the data into RHI textures without a CPU copy
    for (int i = 0;; ++i) {
        const AVCodecHWConfig *config = avcodec_get_hw_config(codec, i);
        if (!config)
            break;

        auto *hwContext = loadHWContext(config->device_type);
        if (hwContext)
            return hwContext;
    }
    qCDebug(qLHWAccel) << "    No HW accelerators found, using SW decoding.";
    return nullptr;

}

// Used for the AVCodecContext::get_format callback
AVPixelFormat getFormat(AVCodecContext *s, const AVPixelFormat *fmt)
{
    // First check HW accelerated codecs, the HW device context must be set
    if (s->hw_device_ctx && s->codec->hw_configs) {
        auto *device_ctx = (AVHWDeviceContext*)s->hw_device_ctx->data;
        for (int i = 0; const AVCodecHWConfig *config = avcodec_get_hw_config(s->codec, i); i++) {
            if (!(config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX))
                continue;
            if (device_ctx->type != config->device_type)
                continue;
            for (int n = 0; fmt[n] != AV_PIX_FMT_NONE; n++) {
                if (config->pix_fmt == fmt[n]) {
#if QT_CONFIG(wmf)
                    if (fmt[n] == AV_PIX_FMT_D3D11)
                        QFFmpeg::D3D11TextureConverter::SetupDecoderTextures(s);
#endif
#ifdef Q_OS_ANDROID
                    if (fmt[n] == AV_PIX_FMT_MEDIACODEC)
                        QFFmpeg::MediaCodecTextureConverter::setupDecoderSurface(s);
#endif
                    return fmt[n];
                }
            }
        }
    }

    // prefer video formats we can handle directly
    for (int n = 0; fmt[n] != AV_PIX_FMT_NONE; n++) {
        bool needsConversion = true;
        QFFmpegVideoBuffer::toQtPixelFormat(fmt[n], &needsConversion);
        if (!needsConversion)
            return fmt[n];
    }

    // take the native format, this will involve one additional format conversion on the CPU side
    return *fmt;
}

TextureConverter::Data::~Data()
{
    delete backend;
}



HWAccel::Data::~Data()
{
    if (hwDeviceContext)
        av_buffer_unref(&hwDeviceContext);
    if (hwFramesContext)
        av_buffer_unref(&hwFramesContext);
}


HWAccel::HWAccel(const AVCodec *codec)
{
    if (codec->type != AVMEDIA_TYPE_VIDEO)
        return;
    auto *ctx = hardwareContextForCodec(codec);
    if (!ctx)
        return;
    d = new Data;
    d->hwDeviceContext = ctx;
}

HWAccel::HWAccel(AVHWDeviceType deviceType)
{
    auto *ctx = loadHWContext(deviceType);
    if (!ctx)
        return;
    d = new Data;
    d->hwDeviceContext = ctx;
}

HWAccel::~HWAccel() = default;

AVPixelFormat HWAccel::format(AVFrame *frame)
{
    if (!frame->hw_frames_ctx)
        return AVPixelFormat(frame->format);

    auto *hwFramesContext = (AVHWFramesContext *)frame->hw_frames_ctx->data;
    Q_ASSERT(hwFramesContext);
    return AVPixelFormat(hwFramesContext->sw_format);
}

const AVHWDeviceType *HWAccel::preferredDeviceTypes()
{
    return preferredHardwareAccelerators;
}

AVHWDeviceContext *HWAccel::hwDeviceContext() const
{
    if (!d || !d->hwDeviceContext)
        return nullptr;
    return (AVHWDeviceContext *)d->hwDeviceContext->data;
}

AVPixelFormat HWAccel::hwFormat() const
{
    switch (deviceType()) {
    case AV_HWDEVICE_TYPE_VIDEOTOOLBOX:
        return AV_PIX_FMT_VIDEOTOOLBOX;
    case AV_HWDEVICE_TYPE_VAAPI:
        return AV_PIX_FMT_VAAPI;
    case AV_HWDEVICE_TYPE_MEDIACODEC:
        return AV_PIX_FMT_MEDIACODEC;
    default:
        return AV_PIX_FMT_NONE;
    }
}

const AVCodec *HWAccel::hardwareDecoderForCodecId(AVCodecID id)
{
    const AVCodec *codec = nullptr;
#ifdef Q_OS_ANDROID
    const auto getDecoder = [](AVCodecID id) {
        switch (id) {
        case AV_CODEC_ID_H264:
            return avcodec_find_decoder_by_name("h264_mediacodec");
        case AV_CODEC_ID_HEVC:
            return avcodec_find_decoder_by_name("hevc_mediacodec");
        case AV_CODEC_ID_MPEG2VIDEO:
            return avcodec_find_decoder_by_name("mpeg2_mediacodec");
        case AV_CODEC_ID_MPEG4:
            return avcodec_find_decoder_by_name("mpeg4_mediacodec");
        case AV_CODEC_ID_VP8:
            return avcodec_find_decoder_by_name("vp8_mediacodec");
        case AV_CODEC_ID_VP9:
            return avcodec_find_decoder_by_name("vp9_mediacodec");
        default:
            return avcodec_find_decoder(id);
        }
    };
    codec = getDecoder(id);
#endif

    if (!codec)
        codec = avcodec_find_decoder(id);

    return codec;
}

const AVCodec *HWAccel::hardwareEncoderForCodecId(AVCodecID id) const
{
    const char *codec = nullptr;
    switch (deviceType()) {
#ifdef Q_OS_DARWIN
    case AV_HWDEVICE_TYPE_VIDEOTOOLBOX:
        switch (id) {
        case AV_CODEC_ID_H264:
            codec = "h264_videotoolbox";
            break;
        case AV_CODEC_ID_HEVC:
            codec = "hevc_videotoolbox";
            break;
        case AV_CODEC_ID_PRORES:
            codec = "prores_videotoolbox";
            break;
        case AV_CODEC_ID_VP9:
            codec = "vp9_videotoolbox";
            break;
        default:
            break;
        }
        break;
#endif
    case AV_HWDEVICE_TYPE_VAAPI:
        switch (id) {
        case AV_CODEC_ID_H264:
            codec = "h264_vaapi";
            break;
        case AV_CODEC_ID_HEVC:
            codec = "hevc_vaapi";
            break;
        case AV_CODEC_ID_MJPEG:
            codec = "mjpeg_vaapi";
            break;
        case AV_CODEC_ID_MPEG2VIDEO:
            codec = "mpeg2_vaapi";
            break;
        case AV_CODEC_ID_VP8:
            codec = "vp8_vaapi";
            break;
        case AV_CODEC_ID_VP9:
            codec = "vp9_vaapi";
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    if (!codec)
        return nullptr;
    const AVCodec *c = avcodec_find_encoder_by_name(codec);
    qCDebug(qLHWAccel) << "searching for HW codec" << codec << "got" << c;
    return c;
}

HWAccel HWAccel::findHardwareAccelForCodecID(AVCodecID id)
{
    auto *accels = preferredHardwareAccelerators;
    while (*accels != AV_HWDEVICE_TYPE_NONE) {
        auto accel = HWAccel(*accels);
        if (accel.hardwareEncoderForCodecId(id) != nullptr)
            return accel;
        ++accels;
    }
    return {};
}

AVHWDeviceType HWAccel::deviceType() const
{
    if (!d || !d->hwDeviceContext)
        return AV_HWDEVICE_TYPE_NONE;
    return hwDeviceContext()->type;
}

void HWAccel::createFramesContext(AVPixelFormat swFormat, const QSize &size)
{
    if (!d || !d->hwDeviceContext)
        return;
    d->hwFramesContext = av_hwframe_ctx_alloc(d->hwDeviceContext);
    auto *c = (AVHWFramesContext *)d->hwFramesContext->data;
    c->format = hwFormat();
    c->sw_format = swFormat;
    c->width = size.width();
    c->height = size.height();
    qCDebug(qLHWAccel) << "init frames context";
    int err = av_hwframe_ctx_init(d->hwFramesContext);
    if (err < 0)
        qWarning() << "failed to init HW frame context" << err << err2str(err);
    else
        qCDebug(qLHWAccel) << "Initialized frames context" << size << c->format << c->sw_format;
}

AVHWFramesContext *HWAccel::hwFramesContext() const
{
    if (!d || !d->hwFramesContext)
        return nullptr;
    return (AVHWFramesContext *)d->hwFramesContext->data;
}


TextureConverter::TextureConverter(QRhi *rhi)
    : d(new Data)
{
    d->rhi = rhi;
}

TextureSet *TextureConverter::getTextures(AVFrame *frame)
{
    if (!frame || isNull())
        return nullptr;

    Q_ASSERT(frame->format == d->format);
    return d->backend->getTextures(frame);
}

void TextureConverter::updateBackend(AVPixelFormat fmt)
{
    d->backend = nullptr;
    if (!d->rhi)
        return;
    switch (fmt) {
#if QT_CONFIG(vaapi)
    case AV_PIX_FMT_VAAPI:
        d->backend = new VAAPITextureConverter(d->rhi);
        break;
#endif
#ifdef Q_OS_DARWIN
    case AV_PIX_FMT_VIDEOTOOLBOX:
        d->backend = new VideoToolBoxTextureConverter(d->rhi);
        break;
#endif
#if QT_CONFIG(wmf)
    case AV_PIX_FMT_D3D11:
        d->backend = new D3D11TextureConverter(d->rhi);
        break;
#endif
#ifdef Q_OS_ANDROID
    case AV_PIX_FMT_MEDIACODEC:
        d->backend = new MediaCodecTextureConverter(d->rhi);
        break;
#endif
    default:
        break;
    }
    d->format = fmt;
}

std::unique_ptr<QRhiTexture> TextureSet::texture(int /*plane*/)
{
    return {};
}

} // namespace QFFmpeg

QT_END_NAMESPACE

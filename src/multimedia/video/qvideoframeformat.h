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

#ifndef QVIDEOSURFACEFORMAT_H
#define QVIDEOSURFACEFORMAT_H

#include <QtMultimedia/qtmultimediaglobal.h>

#include <QtCore/qlist.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qsize.h>
#include <QtGui/qimage.h>

QT_BEGIN_NAMESPACE


class QDebug;

class QVideoFrameFormatPrivate;
class QVideoFrame;
class QMatrix4x4;

QT_DECLARE_QESDP_SPECIALIZATION_DTOR_WITH_EXPORT(QVideoFrameFormatPrivate, Q_MULTIMEDIA_EXPORT)

class Q_MULTIMEDIA_EXPORT QVideoFrameFormat
{
public:
    enum PixelFormat
    {
        Format_Invalid,
        Format_ARGB8888,
        Format_ARGB8888_Premultiplied,
        Format_XRGB8888,
        Format_BGRA8888,
        Format_BGRA8888_Premultiplied,
        Format_BGRX8888,
        Format_ABGR8888,
        Format_XBGR8888,
        Format_RGBA8888,
        Format_RGBX8888,

        Format_AYUV,
        Format_AYUV_Premultiplied,
        Format_YUV420P,
        Format_YUV422P,
        Format_YV12,
        Format_UYVY,
        Format_YUYV,
        Format_NV12,
        Format_NV21,
        Format_IMC1,
        Format_IMC2,
        Format_IMC3,
        Format_IMC4,
        Format_Y8,
        Format_Y16,

        Format_P010,
        Format_P016,

        Format_SamplerExternalOES,
        Format_Jpeg,
        Format_SamplerRect,

        Format_YUV420P10
    };
#ifndef Q_QDOC
    static constexpr int NPixelFormats = Format_YUV420P10 + 1;
#endif

    enum Direction
    {
        TopToBottom,
        BottomToTop
    };

#if QT_DEPRECATED_SINCE(6, 4)
    enum YCbCrColorSpace
    {
        YCbCr_Undefined = 0,
        YCbCr_BT601 = 1,
        YCbCr_BT709 = 2,
        YCbCr_xvYCC601 = 3,
        YCbCr_xvYCC709 = 4,
        YCbCr_JPEG = 5,
        YCbCr_BT2020 = 6
    };
#endif

    // Keep values compatible with YCbCrColorSpace
    enum ColorSpace
    {
        ColorSpace_Undefined = 0,
        ColorSpace_BT601 = 1,
        ColorSpace_BT709 = 2,
        ColorSpace_AdobeRgb = 5,
        ColorSpace_BT2020 = 6
    };

    enum ColorTransfer
    {
        ColorTransfer_Unknown,
        ColorTransfer_BT709,
        ColorTransfer_BT601,
        ColorTransfer_Linear,
        ColorTransfer_Gamma22,
        ColorTransfer_Gamma28,
        ColorTransfer_ST2084,
        ColorTransfer_STD_B67,
    };

    enum ColorRange
    {
        ColorRange_Unknown,
        ColorRange_Video,
        ColorRange_Full
    };

    QVideoFrameFormat();
    QVideoFrameFormat(const QSize &size, PixelFormat pixelFormat);
    QVideoFrameFormat(const QVideoFrameFormat &format);
    ~QVideoFrameFormat();

    QVideoFrameFormat(QVideoFrameFormat &&other) noexcept = default;
    QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QVideoFrameFormat);
    void swap(QVideoFrameFormat &other) noexcept
    { d.swap(other.d); }

    void detach();

    QVideoFrameFormat &operator=(const QVideoFrameFormat &format);

    bool operator==(const QVideoFrameFormat &format) const;
    bool operator!=(const QVideoFrameFormat &format) const;

    bool isValid() const;

    QVideoFrameFormat::PixelFormat pixelFormat() const;

    QSize frameSize() const;
    void setFrameSize(const QSize &size);
    void setFrameSize(int width, int height);

    int frameWidth() const;
    int frameHeight() const;

    int planeCount() const;

    QRect viewport() const;
    void setViewport(const QRect &viewport);

    Direction scanLineDirection() const;
    void setScanLineDirection(Direction direction);

    qreal frameRate() const;
    void setFrameRate(qreal rate);

#if QT_DEPRECATED_SINCE(6, 4)
    QT_DEPRECATED_VERSION_X_6_0("Use colorSpace()") YCbCrColorSpace yCbCrColorSpace() const;
    QT_DEPRECATED_VERSION_X_6_0("Use setColorSpace()") void setYCbCrColorSpace(YCbCrColorSpace colorSpace);
#endif

    ColorSpace colorSpace() const;
    void setColorSpace(ColorSpace colorSpace);

    ColorTransfer colorTransfer() const;
    void setColorTransfer(ColorTransfer colorTransfer);

    ColorRange colorRange() const;
    void setColorRange(ColorRange range);

    bool isMirrored() const;
    void setMirrored(bool mirrored);

    QString vertexShaderFileName() const;
    QString fragmentShaderFileName() const;
    void updateUniformData(QByteArray *dst, const QVideoFrame &frame, const QMatrix4x4 &transform, float opacity) const;

    float maxLuminance() const;
    void setMaxLuminance(float lum);

    static PixelFormat pixelFormatFromImageFormat(QImage::Format format);
    static QImage::Format imageFormatFromPixelFormat(PixelFormat format);

    static QString pixelFormatToString(QVideoFrameFormat::PixelFormat pixelFormat);

private:
    QExplicitlySharedDataPointer<QVideoFrameFormatPrivate> d;
};

Q_DECLARE_SHARED(QVideoFrameFormat)

#ifndef QT_NO_DEBUG_STREAM
Q_MULTIMEDIA_EXPORT QDebug operator<<(QDebug, const QVideoFrameFormat &);
Q_MULTIMEDIA_EXPORT QDebug operator<<(QDebug, QVideoFrameFormat::Direction);
QT_DEPRECATED_VERSION_X_6_0("Use QVideoFrameFormat::ColorSpace")
Q_MULTIMEDIA_EXPORT QDebug operator<<(QDebug, QVideoFrameFormat::YCbCrColorSpace);
Q_MULTIMEDIA_EXPORT QDebug operator<<(QDebug, QVideoFrameFormat::ColorSpace);
Q_MULTIMEDIA_EXPORT QDebug operator<<(QDebug, QVideoFrameFormat::PixelFormat);
#endif

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QVideoFrameFormat)

#endif


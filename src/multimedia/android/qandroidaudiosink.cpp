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

#include "qandroidaudiosink_p.h"
#include "qopenslesengine_p.h"
#include <QDebug>
#include <qmath.h>
#include <qmediadevices.h>

#ifdef ANDROID
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>
#endif // ANDROID

#define BUFFER_COUNT 2

QT_BEGIN_NAMESPACE

static inline void openSlDebugInfo()
{
    const QAudioFormat &format = QMediaDevices::defaultAudioOutput().preferredFormat();
    qDebug() << "======= OpenSL ES Device info ======="
             << "\nSupports low-latency playback: " << (QOpenSLESEngine::supportsLowLatency() ? "YES" : "NO")
             << "\nPreferred sample rate: " << QOpenSLESEngine::getOutputValue(QOpenSLESEngine::SampleRate, -1)
             << "\nFrames per buffer: " << QOpenSLESEngine::getOutputValue(QOpenSLESEngine::FramesPerBuffer, -1)
             << "\nPreferred Format: "  << format
             << "\nLow-latency buffer size: " << QOpenSLESEngine::getLowLatencyBufferSize(format)
             << "\nDefault buffer size: " << QOpenSLESEngine::getDefaultBufferSize(format);
}

QAndroidAudioSink::QAndroidAudioSink(const QByteArray &device)
    : m_deviceName(device),
      m_state(QAudio::StoppedState),
      m_error(QAudio::NoError),
      m_outputMixObject(nullptr),
      m_playerObject(nullptr),
      m_playItf(nullptr),
      m_volumeItf(nullptr),
      m_bufferQueueItf(nullptr),
      m_audioSource(nullptr),
      m_buffers(nullptr),
      m_volume(1.0),
      m_pullMode(false),
      m_nextBuffer(0),
      m_bufferSize(0),
      m_elapsedTime(0),
      m_processedBytes(0),
      m_availableBuffers(BUFFER_COUNT),
      m_eventMask(SL_PLAYEVENT_HEADATEND),
      m_startRequiresInit(true)
{
#ifndef ANDROID
      m_streamType = -1;
#else
      m_streamType = SL_ANDROID_STREAM_MEDIA;
#endif // ANDROID
}

QAndroidAudioSink::~QAndroidAudioSink()
{
    destroyPlayer();
}

QAudio::Error QAndroidAudioSink::error() const
{
    return m_error;
}

QAudio::State QAndroidAudioSink::state() const
{
    return m_state;
}

void QAndroidAudioSink::start(QIODevice *device)
{
    Q_ASSERT(device);

    if (m_state != QAudio::StoppedState)
        stop();

    if (!preparePlayer())
        return;

    m_pullMode = true;
    m_audioSource = device;
    m_nextBuffer = 0;
    m_processedBytes = 0;
    m_availableBuffers = BUFFER_COUNT;
    setState(QAudio::ActiveState);
    setError(QAudio::NoError);

    // Attempt to fill buffers first.
    for (int i = 0; i != BUFFER_COUNT; ++i) {
        const int index = i * m_bufferSize;
        const qint64 readSize = m_audioSource->read(m_buffers + index, m_bufferSize);
        if (readSize && SL_RESULT_SUCCESS != (*m_bufferQueueItf)->Enqueue(m_bufferQueueItf,
                                                                          m_buffers + index,
                                                                          readSize)) {
            setError(QAudio::FatalError);
            destroyPlayer();
            return;
        }
        m_processedBytes += readSize;
    }

    if (m_processedBytes < 1)
        onEOSEvent();

    // Change the state to playing.
    // We need to do this after filling the buffers or processedBytes might get corrupted.
    startPlayer();
}

QIODevice *QAndroidAudioSink::start()
{
    if (m_state != QAudio::StoppedState)
        stop();

    if (!preparePlayer())
        return nullptr;

    m_pullMode = false;
    m_processedBytes = 0;
    m_availableBuffers = BUFFER_COUNT;
    m_audioSource = new SLIODevicePrivate(this);
    m_audioSource->open(QIODevice::WriteOnly | QIODevice::Unbuffered);

    // Change the state to playing
    startPlayer();

    setState(QAudio::IdleState);
    return m_audioSource;
}

void QAndroidAudioSink::stop()
{
    if (m_state == QAudio::StoppedState)
        return;

    stopPlayer();
    setError(QAudio::NoError);
}

qsizetype QAndroidAudioSink::bytesFree() const
{
    if (m_state != QAudio::ActiveState && m_state != QAudio::IdleState)
        return 0;

    return m_availableBuffers.loadAcquire() ? m_bufferSize : 0;
}

void QAndroidAudioSink::setBufferSize(qsizetype value)
{
    if (m_state != QAudio::StoppedState)
        return;

    m_startRequiresInit = true;
    m_bufferSize = value;
}

qsizetype QAndroidAudioSink::bufferSize() const
{
    return m_bufferSize;
}

qint64 QAndroidAudioSink::processedUSecs() const
{
    if (m_state == QAudio::IdleState || m_state == QAudio::SuspendedState)
        return m_format.durationForBytes(m_processedBytes);

    SLmillisecond processMSec = 0;
    if (m_playItf)
        (*m_playItf)->GetPosition(m_playItf, &processMSec);

    return processMSec * 1000;
}

void QAndroidAudioSink::resume()
{
    if (m_state != QAudio::SuspendedState)
        return;

    if (SL_RESULT_SUCCESS != (*m_playItf)->SetPlayState(m_playItf, SL_PLAYSTATE_PLAYING)) {
        setError(QAudio::FatalError);
        destroyPlayer();
        return;
    }

    setState(m_pullMode ? QAudio::ActiveState : QAudio::IdleState);
    setError(QAudio::NoError);
}

void QAndroidAudioSink::setFormat(const QAudioFormat &format)
{
    m_startRequiresInit = true;
    m_format = format;
}

QAudioFormat QAndroidAudioSink::format() const
{
    return m_format;
}

void QAndroidAudioSink::suspend()
{
    if (m_state != QAudio::ActiveState && m_state != QAudio::IdleState)
        return;

    if (SL_RESULT_SUCCESS != (*m_playItf)->SetPlayState(m_playItf, SL_PLAYSTATE_PAUSED)) {
        setError(QAudio::FatalError);
        destroyPlayer();
        return;
    }

    setState(QAudio::SuspendedState);
    setError(QAudio::NoError);
}

void QAndroidAudioSink::reset()
{
    destroyPlayer();
}

void QAndroidAudioSink::setVolume(qreal vol)
{
    m_volume = qBound(qreal(0.0), vol, qreal(1.0));
    const SLmillibel newVolume = adjustVolume(m_volume);
    if (m_volumeItf && SL_RESULT_SUCCESS != (*m_volumeItf)->SetVolumeLevel(m_volumeItf, newVolume))
        qWarning() << "Unable to change volume";
}

qreal QAndroidAudioSink::volume() const
{
    return m_volume;
}

void QAndroidAudioSink::onEOSEvent()
{
    if (m_state != QAudio::ActiveState)
        return;

    SLBufferQueueState state;
    if (SL_RESULT_SUCCESS != (*m_bufferQueueItf)->GetState(m_bufferQueueItf, &state))
        return;

    if (state.count > 0)
        return;

    setState(QAudio::IdleState);
    setError(QAudio::UnderrunError);
}

void QAndroidAudioSink::onBytesProcessed(qint64 bytes)
{
    m_processedBytes += bytes;
}

void QAndroidAudioSink::bufferAvailable(quint32 count, quint32 playIndex)
{
    Q_UNUSED(count);
    Q_UNUSED(playIndex);

    if (m_state == QAudio::StoppedState)
        return;

    if (!m_pullMode) { // We're in push mode.
        // Signal that there is a new open slot in the buffer and return
        const int val = m_availableBuffers.fetchAndAddRelease(1) + 1;
        if (val == BUFFER_COUNT)
            QMetaObject::invokeMethod(this, "onEOSEvent", Qt::QueuedConnection);

        return;
    }

    // We're in pull mode.
    const int index = m_nextBuffer * m_bufferSize;
    const qint64 readSize = m_audioSource->read(m_buffers + index, m_bufferSize);

    if (readSize < 1) {
        QMetaObject::invokeMethod(this, "onEOSEvent", Qt::QueuedConnection);
        return;
    }


    if (SL_RESULT_SUCCESS != (*m_bufferQueueItf)->Enqueue(m_bufferQueueItf,
                                                          m_buffers + index,
                                                          readSize)) {
        setError(QAudio::FatalError);
        destroyPlayer();
        return;
    }

    m_nextBuffer = (m_nextBuffer + 1) % BUFFER_COUNT;
    QMetaObject::invokeMethod(this, "onBytesProcessed", Qt::QueuedConnection, Q_ARG(qint64, readSize));

    if (m_audioSource->atEnd())
        setState(QAudio::IdleState);
}

void QAndroidAudioSink::playCallback(SLPlayItf player, void *ctx, SLuint32 event)
{
    Q_UNUSED(player);
    QAndroidAudioSink *audioOutput = reinterpret_cast<QAndroidAudioSink *>(ctx);
    if (event & SL_PLAYEVENT_HEADATEND)
        QMetaObject::invokeMethod(audioOutput, "onEOSEvent", Qt::QueuedConnection);
}

void QAndroidAudioSink::bufferQueueCallback(SLBufferQueueItf bufferQueue, void *ctx)
{
    SLBufferQueueState state;
    (*bufferQueue)->GetState(bufferQueue, &state);
    QAndroidAudioSink *audioOutput = reinterpret_cast<QAndroidAudioSink *>(ctx);
    audioOutput->bufferAvailable(state.count, state.playIndex);
}

bool QAndroidAudioSink::preparePlayer()
{
    if (m_startRequiresInit)
        destroyPlayer();
    else
        return true;

    SLEngineItf engine = QOpenSLESEngine::instance()->slEngine();
    if (!engine) {
        qWarning() << "No engine";
        setError(QAudio::FatalError);
        return false;
    }

    SLDataLocator_BufferQueue bufferQueueLocator = { SL_DATALOCATOR_BUFFERQUEUE, BUFFER_COUNT };
    SLAndroidDataFormat_PCM_EX pcmFormat = QOpenSLESEngine::audioFormatToSLFormatPCM(m_format);

    SLDataSource audioSrc = { &bufferQueueLocator, &pcmFormat };

    // OutputMix
    if (SL_RESULT_SUCCESS != (*engine)->CreateOutputMix(engine,
                                                        &m_outputMixObject,
                                                        0,
                                                        nullptr,
                                                        nullptr)) {
        qWarning() << "Unable to create output mix";
        setError(QAudio::FatalError);
        return false;
    }

    if (SL_RESULT_SUCCESS != (*m_outputMixObject)->Realize(m_outputMixObject, SL_BOOLEAN_FALSE)) {
        qWarning() << "Unable to initialize output mix";
        setError(QAudio::FatalError);
        return false;
    }

    SLDataLocator_OutputMix outputMixLocator = { SL_DATALOCATOR_OUTPUTMIX, m_outputMixObject };
    SLDataSink audioSink = { &outputMixLocator, nullptr };

#ifndef ANDROID
    const int iids = 2;
    const SLInterfaceID ids[iids] = { SL_IID_BUFFERQUEUE, SL_IID_VOLUME };
    const SLboolean req[iids] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
#else
    const int iids = 3;
    const SLInterfaceID ids[iids] = { SL_IID_BUFFERQUEUE,
                                      SL_IID_VOLUME,
                                      SL_IID_ANDROIDCONFIGURATION };
    const SLboolean req[iids] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
#endif // ANDROID

    // AudioPlayer
    if (SL_RESULT_SUCCESS != (*engine)->CreateAudioPlayer(engine,
                                                          &m_playerObject,
                                                          &audioSrc,
                                                          &audioSink,
                                                          iids,
                                                          ids,
                                                          req)) {
        qWarning() << "Unable to create AudioPlayer";
        setError(QAudio::OpenError);
        return false;
    }

#ifdef ANDROID
    // Set profile/category
    SLAndroidConfigurationItf playerConfig;
    if (SL_RESULT_SUCCESS == (*m_playerObject)->GetInterface(m_playerObject,
                                                             SL_IID_ANDROIDCONFIGURATION,
                                                             &playerConfig)) {
        (*playerConfig)->SetConfiguration(playerConfig,
                                          SL_ANDROID_KEY_STREAM_TYPE,
                                          &m_streamType,
                                          sizeof(SLint32));
    }
#endif // ANDROID

    if (SL_RESULT_SUCCESS != (*m_playerObject)->Realize(m_playerObject, SL_BOOLEAN_FALSE)) {
        qWarning() << "Unable to initialize AudioPlayer";
        setError(QAudio::OpenError);
        return false;
    }

    // Buffer interface
    if (SL_RESULT_SUCCESS != (*m_playerObject)->GetInterface(m_playerObject,
                                                             SL_IID_BUFFERQUEUE,
                                                             &m_bufferQueueItf)) {
        setError(QAudio::FatalError);
        return false;
    }

    if (SL_RESULT_SUCCESS != (*m_bufferQueueItf)->RegisterCallback(m_bufferQueueItf,
                                                                   bufferQueueCallback,
                                                                   this)) {
        setError(QAudio::FatalError);
        return false;
    }

    // Play interface
    if (SL_RESULT_SUCCESS != (*m_playerObject)->GetInterface(m_playerObject,
                                                             SL_IID_PLAY,
                                                             &m_playItf)) {
        setError(QAudio::FatalError);
        return false;
    }

    if (SL_RESULT_SUCCESS != (*m_playItf)->RegisterCallback(m_playItf, playCallback, this)) {
        setError(QAudio::FatalError);
        return false;
    }

    if (SL_RESULT_SUCCESS != (*m_playItf)->SetCallbackEventsMask(m_playItf, m_eventMask)) {
        setError(QAudio::FatalError);
        return false;
    }

    // Volume interface
    if (SL_RESULT_SUCCESS != (*m_playerObject)->GetInterface(m_playerObject,
                                                             SL_IID_VOLUME,
                                                             &m_volumeItf)) {
        setError(QAudio::FatalError);
        return false;
    }

    setVolume(m_volume);

    const int lowLatencyBufferSize = QOpenSLESEngine::getLowLatencyBufferSize(m_format);
    const int defaultBufferSize = QOpenSLESEngine::getDefaultBufferSize(m_format);

    if (defaultBufferSize <= 0) {
        qWarning() << "Unable to get minimum buffer size, returned" << defaultBufferSize;
        setError(QAudio::FatalError);
        return false;
    }

    // Buffer size
    if (m_bufferSize <= 0) {
        m_bufferSize = defaultBufferSize;
    } else if (QOpenSLESEngine::supportsLowLatency()) {
        if (m_bufferSize < lowLatencyBufferSize)
            m_bufferSize = lowLatencyBufferSize;
    } else if (m_bufferSize < defaultBufferSize) {
        m_bufferSize = defaultBufferSize;
    }

    if (!m_buffers)
        m_buffers = new char[BUFFER_COUNT * m_bufferSize];

    setError(QAudio::NoError);
    m_startRequiresInit = false;

    return true;
}

void QAndroidAudioSink::destroyPlayer()
{
    if (m_state != QAudio::StoppedState)
        stopPlayer();

    if (m_playerObject) {
        (*m_playerObject)->Destroy(m_playerObject);
        m_playerObject = nullptr;
    }

    if (m_outputMixObject) {
        (*m_outputMixObject)->Destroy(m_outputMixObject);
        m_outputMixObject = nullptr;
    }

    if (!m_pullMode && m_audioSource) {
        m_audioSource->close();
        delete m_audioSource;
        m_audioSource = nullptr;
    }

    delete [] m_buffers;
    m_buffers = nullptr;
    m_processedBytes = 0;
    m_nextBuffer = 0;
    m_availableBuffers.storeRelease(BUFFER_COUNT);
    m_playItf = nullptr;
    m_volumeItf = nullptr;
    m_bufferQueueItf = nullptr;
    m_startRequiresInit = true;
}

void QAndroidAudioSink::stopPlayer()
{
    setState(QAudio::StoppedState);

    if (m_audioSource && !m_pullMode) {
        m_audioSource->close();
        delete m_audioSource;
        m_audioSource = nullptr;
    }

    // We need to change the state manually...
    if (m_playItf)
        (*m_playItf)->SetPlayState(m_playItf, SL_PLAYSTATE_STOPPED);

    if (m_bufferQueueItf && SL_RESULT_SUCCESS != (*m_bufferQueueItf)->Clear(m_bufferQueueItf))
        qWarning() << "Unable to clear buffer";
}

void QAndroidAudioSink::startPlayer()
{
    if (QOpenSLESEngine::printDebugInfo())
        openSlDebugInfo();

    if (SL_RESULT_SUCCESS != (*m_playItf)->SetPlayState(m_playItf, SL_PLAYSTATE_PLAYING)) {
        setError(QAudio::FatalError);
        destroyPlayer();
    }
}

qint64 QAndroidAudioSink::writeData(const char *data, qint64 len)
{
    if (!len)
        return 0;

    if (len > m_bufferSize)
        len = m_bufferSize;

    // Acquire one slot in the buffer
    const int before = m_availableBuffers.fetchAndAddAcquire(-1);

    // If there where no vacant slots, then we just overdrew the buffer account...
    if (before < 1) {
        m_availableBuffers.fetchAndAddRelease(1);
        return 0;
    }

    const int index = m_nextBuffer * m_bufferSize;
    ::memcpy(m_buffers + index, data, len);
    const SLuint32 res = (*m_bufferQueueItf)->Enqueue(m_bufferQueueItf,
                                                      m_buffers + index,
                                                      len);

    // If we where unable to enqueue a new buffer, give back the acquired slot.
    if (res == SL_RESULT_BUFFER_INSUFFICIENT) {
        m_availableBuffers.fetchAndAddRelease(1);
        return 0;
    }

    if (res != SL_RESULT_SUCCESS) {
        setError(QAudio::FatalError);
        destroyPlayer();
        return -1;
    }

    m_processedBytes += len;
    setState(QAudio::ActiveState);
    setError(QAudio::NoError);
    m_nextBuffer = (m_nextBuffer + 1) % BUFFER_COUNT;

    return len;
}

inline void QAndroidAudioSink::setState(QAudio::State state)
{
    if (m_state == state)
        return;

    m_state = state;
    Q_EMIT stateChanged(m_state);
}

inline void QAndroidAudioSink::setError(QAudio::Error error)
{
    if (m_error == error)
        return;

    m_error = error;
    Q_EMIT errorChanged(m_error);
}

inline SLmillibel QAndroidAudioSink::adjustVolume(qreal vol)
{
    if (qFuzzyIsNull(vol))
        return SL_MILLIBEL_MIN;

    if (qFuzzyCompare(vol, qreal(1.0)))
        return 0;

    return QAudio::convertVolume(vol, QAudio::LinearVolumeScale, QAudio::DecibelVolumeScale) * 100;
}

QT_END_NAMESPACE

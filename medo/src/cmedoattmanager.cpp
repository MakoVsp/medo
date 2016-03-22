#include "cmedoattmanager.h"

#include <QUrl>
#include <QMetaType>
#include <QFile>

static qreal getPeakValue(const QAudioFormat &format);
static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);

CMedoAttManager* CMedoAttManager::m_pInstance = 0;

CMedoAttManager::CMedoAttManager(QObject *parent) :
    CMedoListModel(parent)
{
    if (!m_pInstance) {
        m_pInstance = this;
    }

    qRegisterMetaType<CMedoAttManager*>();

    m_pAudioRecorder = new QAudioRecorder();
    m_pAudioProbe = new QAudioProbe();
    m_pPowerManager = new CSystemPowerManager();

    m_nDuration = 0;
    m_nLevel = 0;
    m_bSendRecorder = true;
    m_bRecorderState = true;

    connect(m_pAudioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)),
            this, SLOT(processBuffer(QAudioBuffer)));
    m_pAudioProbe->setSource(m_pAudioRecorder);

    connect(m_pAudioRecorder, SIGNAL(error(QMediaRecorder::Error)),
            this, SLOT(error(QMediaRecorder::Error)));

    connect(m_pAudioRecorder,SIGNAL(statusChanged(QMediaRecorder::Status)),this,SLOT(statusChanged(QMediaRecorder::Status)));

    connect(m_pAudioRecorder,SIGNAL(stateChanged(QMediaRecorder::State)),this,SLOT(stateChanged(QMediaRecorder::State)));

    connect(m_pAudioRecorder, SIGNAL(durationChanged(qint64)), this, SLOT(updateProgress(qint64)));

    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/AAC");
    audioSettings.setQuality(QMultimedia::VeryLowQuality);
    audioSettings.setSampleRate(8000);
    audioSettings.setBitRate(8000);
    audioSettings.setChannelCount(1);
    audioSettings.setEncodingMode(QMultimedia::ConstantBitRateEncoding);

    m_pAudioRecorder->setEncodingSettings(audioSettings);
}

CMedoAttManager::~CMedoAttManager()
{
    m_pPowerManager->releaseWakelock();

    if (m_pAudioRecorder) {
        delete m_pAudioRecorder;
        m_pAudioRecorder = NULL;
    }

    if (m_pAudioProbe) {
        delete m_pAudioProbe;
        m_pAudioProbe = NULL;
    }

    if (m_pPowerManager) {
        delete m_pPowerManager;
        m_pPowerManager = NULL;
    }
}

void CMedoAttManager::startRecorder(const QString &path)
{
    qDebug() << Q_FUNC_INFO;
    m_bStopRecorder = false;
    m_nDuration = 0;
    m_nLevel = 0;
    m_bSendRecorder = true;
    m_sRecorderPath = path;
    setRecorderReady(false);
    m_pAudioRecorder->setOutputLocation(QUrl(path));
    m_pAudioRecorder->record();
    m_pPowerManager->accquireWakelock();
}

void CMedoAttManager::stopRecorder()
{
    if(m_bRecorderState) {
        m_bStopRecorder = false;
        m_pAudioRecorder->stop();
        m_pPowerManager->releaseWakelock();
    } else {
        m_bStopRecorder = true;
    }
}

void CMedoAttManager::cancelRecorder()
{
    if(m_bRecorderState) {
        m_bStopRecorder = false;
        m_bSendRecorder = false;
        m_pAudioRecorder->stop();
        m_pPowerManager->releaseWakelock();
        QFile::remove(m_sRecorderPath);
    } else {
        m_bStopRecorder = true;
    }
}

void CMedoAttManager::updateAttModel(const QString &path)
{
    QDir oDir(path);
    QStringList oList = oDir.entryList(QDir::Files, QDir::Time);
    qDebug() << Q_FUNC_INFO << oList;
}

void CMedoAttManager::processBuffer(const QAudioBuffer &buffer)
{
    QVector<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i) {
        setLevel(qFloor(levels.at(i) * 100));
    }
}

void CMedoAttManager::error(QMediaRecorder::Error error)
{
    qDebug() << Q_FUNC_INFO << error;
    emit recorderError("录音失败");
    m_bSendRecorder = false;
    m_pAudioRecorder->stop();
    m_pPowerManager->releaseWakelock();
    setRecorderReady(true);
}

void CMedoAttManager::stateChanged(QMediaRecorder::State state)
{
    Q_UNUSED(state);
}

void CMedoAttManager::statusChanged(QMediaRecorder::Status status)
{
    qDebug() << Q_FUNC_INFO << status;
    if(status == QMediaRecorder::LoadedStatus)
    {
        if(m_bSendRecorder) {
            if(m_nDuration < 900) {
                QFile::remove(m_sRecorderPath);
                emit recorderTooShortError("录音时间太短");
            } else {
                emit recorderLoadedStatus();
            }
        }

        setRecorderReady(true);
        if(m_bStopRecorder) {
            cancelRecorder();
        }
    } else if(status == QMediaRecorder::UnavailableStatus){
        emit recorderError("录音没有组装");
    } else if(status == QMediaRecorder::UnloadedStatus){
        emit recorderError("录音没有加载");
    } else if(status == QMediaRecorder::RecordingStatus) {
        setRecorderReady(true);
        if(m_bStopRecorder) {
            cancelRecorder();
        }
    }
}

void CMedoAttManager::updateProgress(qint64 duration)
{
    setDuration(duration);
}

CMedoAttManager *CMedoAttManager::instance()
{
    qDebug() << Q_FUNC_INFO;
    return m_pInstance;
}

qint64 CMedoAttManager::duration() const
{
    return m_nDuration;
}

qint64 CMedoAttManager::setDuration(const qint64 &duration)
{
    m_nDuration = duration;
    emit durationChanged(duration);
    return duration;
}

int CMedoAttManager::level() const
{
    return m_nLevel;
}

int CMedoAttManager::setLevel(const int &level)
{
    m_nLevel = level;
    emit levelChanged();
    return level;
}

bool CMedoAttManager::recorderReady() const
{
    return m_bRecorderState;
}

bool CMedoAttManager::setRecorderReady(const bool &recorderReady)
{
    qDebug() << Q_FUNC_INFO << recorderReady;
    m_bRecorderState = recorderReady;
    emit recorderReadyChanged();
    return recorderReady;
}

// This function returns the maximum possible sample value for a given audio format
qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return qreal(0);

    if (format.codec() != "audio/pcm")
        return qreal(0);

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return qreal(INT_MAX);
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
}

// returns the audio level for each channel
QVector<qreal> getBufferLevels(const QAudioBuffer& buffer)
{
    QVector<qreal> values;

    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return values;

    if (buffer.format().codec() != "audio/pcm")
        return values;

    int channelCount = buffer.format().channelCount();
    values.fill(0, channelCount);
    qreal peak_value = getPeakValue(buffer.format());
    if (qFuzzyCompare(peak_value, qreal(0)))
        return values;

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<quint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<quint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<quint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32) {
            values = getBufferLevels(buffer.constData<float>(), buffer.frameCount(), channelCount);
            for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;
        }
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<qint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<qint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<qint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] /= peak_value;
        break;
    }

    return values;
}

template <class T>
QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels)
{
    QVector<qreal> max_values;
    max_values.fill(0, channels);

    for (int i = 0; i < frames; ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(qreal(buffer[i * channels + j]));
            if (value > max_values.at(j))
                max_values.replace(j, value);
        }
    }

    return max_values;
}

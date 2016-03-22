#ifndef CMEDOATTMANAGER_H
#define CMEDOATTMANAGER_H

#include "cmedolistmodel.h"
#include "cmedodbmanager.h"
#include <csystempowermanager.h>

#include <QObject>
#include <QThread>
#include <QAudioRecorder>
#include <QAudioProbe>
#include <QAudioBuffer>
#include <QtCore/qmath.h>

class CMedoAttManager : public CMedoListModel
{
    Q_OBJECT
    Q_PROPERTY(qint64 duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(bool recorderReady READ recorderReady WRITE setRecorderReady NOTIFY recorderReadyChanged)

public:
    explicit CMedoAttManager(QObject *parent = 0);

    ~CMedoAttManager();
    static CMedoAttManager* m_pInstance;

    Q_INVOKABLE void startRecorder(const QString &path);
    Q_INVOKABLE void stopRecorder();
    Q_INVOKABLE void cancelRecorder();

    Q_INVOKABLE void updateAttModel(const QString &path);

signals:
    void durationChanged(qint64 duration);
    void levelChanged();
    void recorderReadyChanged();

    void recorderLoadedStatus();
    void recorderError(QString error);
    void recorderTooShortError(QString error);

public slots:
    void processBuffer(const QAudioBuffer& buffer);

    void error(QMediaRecorder::Error error);
    void stateChanged(QMediaRecorder::State state);

    void statusChanged(QMediaRecorder::Status status);
    void updateProgress(qint64 duration);
    static CMedoAttManager* instance();

private:
    qint64 duration() const;
    qint64 setDuration(const qint64 &duration);

    int level() const;
    int setLevel(const int &level);

    bool recorderReady() const;
    bool setRecorderReady(const bool &recorderReady);

private:
    QAudioRecorder *m_pAudioRecorder;
    QAudioProbe *m_pAudioProbe;
    CSystemPowerManager *m_pPowerManager;

    qint64 m_nDuration;
    int m_nLevel;
    bool m_bSendRecorder;
    QString m_sRecorderPath;
    bool m_bRecorderState;
    bool m_bStopRecorder;

//    QThread m_oThread;
//    QMap<QString, CMedoRecord*> m_oMap;

};

#endif // CMEDOATTMANAGER_H

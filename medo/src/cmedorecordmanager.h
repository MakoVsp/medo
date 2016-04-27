#ifndef CMEDORECORDMANAGER_H
#define CMEDORECORDMANAGER_H

#include "cmedolistmodel.h"
#include "cmedodbmanager.h"

#include <QObject>
#include <QThread>

class CMedoRecordThread;

class CMedoRecordManager : public CMedoListModel
{
    Q_OBJECT
public:
    explicit CMedoRecordManager(QObject *parent = 0);
    ~CMedoRecordManager();

    Q_INVOKABLE void initModel();

    Q_INVOKABLE QString currentTime();

    Q_INVOKABLE void newRecord(const QString &id,
                               const QString &content,
                               const QString &attachment,
                               const QString &date);

    Q_INVOKABLE void updateRecord(const QString &id,
                                  const QString &content,
                                  const QString &attachment,
                                  const QString &date);

    Q_INVOKABLE QString getNewAttPath();

    Q_INVOKABLE void startRecorder(const QString &parentId,const QString &path);

    Q_INVOKABLE void stopRecorder();

    Q_INVOKABLE void cancelRecorder();

    Q_INVOKABLE void renameFile(const QString &oldName, const QString &newName);

    Q_INVOKABLE bool removeFile(const QString &name);

    Q_INVOKABLE void deleteAttachment(const QString &parentId,
                                      const QString &path,
                                      QString name);
    Q_INVOKABLE void deleteRecord(const QString &id);
    Q_INVOKABLE CMedoRecord* getRecord(const QString &id);

signals:
    void sendInitModelToThread();
    void sendStartRecorderToThread(const QString &path);
    void startQueryAttachments(const QString &parentId);

public slots:
    void addRecordToModel(const QString &id,
                          const QString &content,
                          const QString &attachment,
                          const QString &date);

    void addAttachmentToRecord(const QString &parentId,
                              const QString &path,
                              const QString &name);

private:
    void createPath(const QString &path);

    QString getAttName(const QString &path);

    QString randomString();

private:
    QThread m_oThread;
    CMedoRecordThread *m_pThreadObj;
    QMap<QString, CMedoRecord*> m_oMap;
    QString m_sCurentAttFullName;

};

#endif // CMEDORECORDMANAGER_H

#ifndef CMEDORECORDTHREAD_H
#define CMEDORECORDTHREAD_H

#include <QObject>

class CMedoRecordThread : public QObject
{
    Q_OBJECT
public:
    explicit CMedoRecordThread(QObject *parent = 0);

signals:
    void sendRecord(const QString &id,
                    const QString &content,
                    const QString &attachment,
                    const QString &date);

public slots:
    void doInitModel();
    void doStartRecorder(const QString &path);

};

#endif // CMEDORECORDTHREAD_H

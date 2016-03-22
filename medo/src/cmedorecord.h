#ifndef CMEDORECORD_H
#define CMEDORECORD_H

#include <QObject>
//#include <QtCore/QMetaType>

class CMedoRecord : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(QString attachment READ attachment WRITE setAttachment NOTIFY attachmentChanged)
    Q_PROPERTY(QString date READ date WRITE setDate NOTIFY dateChanged)

public:
    CMedoRecord(const QString &id,
                const QString &content,
                const QString &attachment,
                const QString &date,
                QObject *parent = 0);

    QString id() const;
    QString setId(const QString &id);

    QString content() const;
    QString setContent(const QString &content);

    // 附件路径
    QString attachment() const;
    QString setAttachment(const QString &attachment);

    QString date() const;
    QString setDate(const QString &date);

signals:
    void idChanged();
    void contentChanged();
    void attachmentChanged();
    void dateChanged();

private:
    QString m_sId;
    QString m_sContent;
    QString m_sAttachment;
    QString m_sDate;
};

//Q_DECLARE_METATYPE(CMedoRecord)

//typedef QList<CMedoRecord> CMedoRecordList;
//Q_DECLARE_METATYPE (CMedoRecordList);

#endif // CMEDORECORD_H

#ifndef CMEDORECORD_H
#define CMEDORECORD_H

#include <QObject>
#include "cmedodbmanager.h"
//#include <QtCore/QMetaType>
class CMedoListModel;

class CMedoRecord : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(QString attachment READ attachment WRITE setAttachment NOTIFY attachmentChanged)
    Q_PROPERTY(QString date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(CMedoListModel* attachmentList READ attachmentList  NOTIFY attachmentListChanged)

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

    CMedoListModel* attachmentList();
    void addAttachment(const QString &parentId,
                       const QString &path,
                       const QString &name);


signals:
    void idChanged();
    void contentChanged();
    void attachmentChanged();
    void dateChanged();
    void attachmentListChanged();

private:
    QString m_sId;
    QString m_sContent;
    QString m_sAttachment;
    QString m_sDate;
    CMedoListModel* m_attList;
};

//Q_DECLARE_METATYPE(CMedoRecord)

//typedef QList<CMedoRecord> CMedoRecordList;
//Q_DECLARE_METATYPE (CMedoRecordList);

#endif // CMEDORECORD_H

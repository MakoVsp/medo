#ifndef CMEDODBMANAGER_H
#define CMEDODBMANAGER_H

#include "cmedorecord.h"

#include <QObject>
#include <QtSql>
#include <QtCore/QMetaType>

struct DBRecord {
    QString id;
    QString content;
    QString attachment;
    QString date;
};
Q_DECLARE_METATYPE(DBRecord)
typedef QList<DBRecord> DBRecordList;

struct DBAttachment{
    QString parentId;
    QString path;
    QString name;
};
Q_DECLARE_METATYPE(DBAttachment)
typedef QList<DBAttachment> DBAttachmentList;


class CMedoDbManager : public QObject
{
    Q_OBJECT
public:
    void static createMedoDbManager();

    static CMedoDbManager* instance();

    DBRecordList getRecordList();

    void addRecord(const QString &id,
                   QString content,
                   const QString &attachment,
                   const QString &date);

    void updateRecord(const QString &id,
                      QString content,
                      const QString &attachment,
                      const QString &date);
    void deleteRecord(const QString &id);

    void addAttachment(const QString &parentId,
                       const QString &path,
                       QString name);
    void deleteAttachment(const QString &parentId,
                                      const QString &path,
                                      QString name);


    DBAttachmentList getAttachments(const QString &parentId);


public:
    static CMedoDbManager* m_pInstance;

private:
    CMedoDbManager(QObject *parent = 0);
    ~CMedoDbManager();

private:
    QSqlDatabase m_oDb;
    QStringList m_oTables;
    QSqlQuery *m_pQuery;

};

#endif // CMEDODBMANAGER_H

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

class CMedoDbManager : public QObject
{
    Q_OBJECT
public:
    void static createMedoDbManager();

    static CMedoDbManager* instance();

    DBRecordList getRecordList();

    void addRecord(QString content,
                   const QString &attachment,
                   const QString &date);

    void updateRecord(const QString &id,
                      QString content,
                      const QString &attachment,
                      const QString &date);

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

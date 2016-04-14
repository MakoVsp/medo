#include "cmedodbmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <sys/stat.h>
#include <QDebug>

#define DB_PATH "/data/data/com.syberos.medo/data/medo.db"
#define TABLE_NAME "medo"
#define SEARCH_TABLE "SELECT * from sqlite_master where type = 'table'"
#define CREATE_TABLE "CREATE TABLE IF NOT EXISTS %1 (id TEXT primary key NOT NULL UNIQUE, content TEXT, attachment TEXT, date TEXT)"
#define INSERT_RECORD "INSERT INTO '%s' ('id','content', 'attachment', 'date') VALUES('%s','%s','%s','%s')"
#define ORDER_RECORD "SELECT * FROM '%s' ORDER BY date desc, id desc"
#define UPDATE_RECORD "UPDATE '%s' SET 'content'='%s', 'attachment'='%s', 'date'='%s' WHERE id = '%s'"

#define TABLE_ATTACHMENT  "attachment"
#define CREATE_ATTACHMENT  "CREATE TABLE IF NOT EXISTS %1 (parentId TEXT  NOT NULL, path TEXT, name TEXT)"
#define INSERT_ATTACHEMNT "INSERT INTO '%s' ('parentId', 'path', 'name') VALUES('%s','%s','%s')"
#define SEARCH_ATTACHMENT "SELECT * from '%s' where parentId = '%s'"
#define DELETE_ATTACHMENT "DELETE from '%s' where parentId = '%s' and path = '%s' and name = '%s'"


CMedoDbManager* CMedoDbManager::m_pInstance = 0;

void CMedoDbManager::createMedoDbManager()
{

    qDebug() << Q_FUNC_INFO;
    if (!m_pInstance) {
        m_pInstance = new CMedoDbManager();
    } else {
        qDebug() << "alreadly create Medo Db Manager !!!";
    }
}

CMedoDbManager *CMedoDbManager::instance()
{
    qDebug() << Q_FUNC_INFO;
    return m_pInstance;
}

DBRecordList CMedoDbManager::getRecordList()
{
    qDebug() << Q_FUNC_INFO;
    DBRecordList oList;
    QString sStr;
    sStr.sprintf(ORDER_RECORD, TABLE_NAME);
    qDebug() << Q_FUNC_INFO << sStr;

    if (m_pQuery->exec(sStr)) {
        while (m_pQuery->next()) {
            DBRecord oRecord;
            oRecord.id = m_pQuery->value("id").toString();
            oRecord.content = QString::fromUtf8(m_pQuery->value("content").toByteArray());;
            oRecord.attachment = m_pQuery->value("attachment").toString();
            oRecord.date = m_pQuery->value("date").toString();
            oList.append(oRecord);
        }
    } else {
        qDebug() << m_pQuery->lastError().text();
    }

    qDebug() << Q_FUNC_INFO << "oList.size = " << oList.size();
    return oList;
}

void CMedoDbManager::addRecord(const QString &id,
                               QString content,
                               const QString &attachment,
                               const QString &date)
{
    qDebug() << Q_FUNC_INFO << "\r\n" << content << "\r\n" << attachment << "\r\n" << date << "\r\n";
    QString sStr;

    if(content.indexOf("'") >= 0) {
        qDebug() << "Input content contains ', so convert it-------------";
        content.replace("'", "''");
    }
    sStr.sprintf(INSERT_RECORD, TABLE_NAME,id.toLocal8Bit().data(),
                content.toLocal8Bit().data(),
                attachment.toUtf8().data(),
                date.toLocal8Bit().data());
    qDebug() << "sStr = " << sStr;
    if (!m_pQuery->exec(sStr)) {
        qDebug()<<"addRecord() failed!!!!!!!!!!!!!" << m_pQuery->lastError().text();
    }
}

void CMedoDbManager::updateRecord(const QString &id,
                                  QString content,
                                  const QString &attachment,
                                  const QString &date)
{
    qDebug() << Q_FUNC_INFO << "id = " << id;
    if (content.indexOf("'") >= 0) {
        content.replace("'", "''");
    }

    QString sStr;
    sStr.sprintf(UPDATE_RECORD, TABLE_NAME,
                content.toLocal8Bit().data(),
                attachment.toLocal8Bit().data(),
                date.toLocal8Bit().data(),
                id.toLocal8Bit().data());
    qDebug() << "sStr = " << sStr;

    if (!m_pQuery->exec(sStr)) {
        qDebug()<<"updateRecord() failed!!!!!!!!!!!!!" << m_pQuery->lastError().text();
    }
}

void CMedoDbManager::addAttachment(const QString &parentId,
                   const QString &path,
                   QString name)
{
    qDebug() << "CMedoDbManager::addAttachment==========" <<  parentId <<"---" << path << "---" << name;
    QString sStr;

    if(name.indexOf("'") >= 0) {
        qDebug() << "Input content contains ', so convert it-------------";
        name.replace("'", "''");
    }
    sStr.sprintf(INSERT_ATTACHEMNT, TABLE_ATTACHMENT,
                parentId.toUtf8().data(),
                path.toUtf8().data(),
                name.toUtf8().data());
    qDebug() << "sStr = " << sStr;
    if (!m_pQuery->exec(sStr)) {
        qDebug()<<"addAttachment() failed!!!!!!!!!!!!!" << m_pQuery->lastError().text();
    }
}

void CMedoDbManager::deleteAttachment(const QString &parentId,
                                  const QString &path,
                                  QString name)
{
    qDebug() << "CMedoDbManager::deleteAttachment==========" <<  parentId <<"---" << path << "---" << name;
    QString sStr;

    if(name.indexOf("'") >= 0) {
        qDebug() << "Input content contains ', so convert it-------------";
        name.replace("'", "''");
    }
    name = path + "/" + name;
    sStr.sprintf(DELETE_ATTACHMENT, TABLE_ATTACHMENT,
                parentId.toUtf8().data(),
                path.toUtf8().data(),
                name.toUtf8().data());
    qDebug() << "sStr = " << sStr;
    if (!m_pQuery->exec(sStr)) {
        qDebug()<<"deleteAttachment() failed!!!!!!!!!!!!!" << m_pQuery->lastError().text();
    }
}

DBAttachmentList CMedoDbManager::getAttachments(const QString &parentId)
{
    qDebug() << "CMedoDbManager::getAttachments-=======" << parentId;
    DBAttachmentList oList;
    QString sStr;
    sStr.sprintf(SEARCH_ATTACHMENT, TABLE_ATTACHMENT,parentId.toUtf8().data());
    qDebug() << "CMedoDbManager::getAttachments-=======str" << sStr;

    if (m_pQuery->exec(sStr)) {
        while (m_pQuery->next()) {
            DBAttachment attachment;
            attachment.parentId = QString::fromUtf8(m_pQuery->value("parentId").toByteArray());
            attachment.path = QString::fromUtf8(m_pQuery->value("path").toByteArray());
            attachment.name = QString::fromUtf8(m_pQuery->value("name").toByteArray());
            oList.append(attachment);
        }
    } else {
        qDebug() << m_pQuery->lastError().text();
    }

    qDebug() << Q_FUNC_INFO << "oList.size = " << oList.size();
    return oList;
}


CMedoDbManager::CMedoDbManager(QObject *parent) :
    QObject(parent)
{
    qDebug() << Q_FUNC_INFO;
    m_oDb = QSqlDatabase::addDatabase("QSQLITE", "medoDB");
    m_oDb.setDatabaseName(DB_PATH);

    QFileInfo oDbFile = QFileInfo(DB_PATH);
    if (!oDbFile.exists()) {
        qDebug() << "first init and create DB !!!";
        QDir oDir = oDbFile.absoluteDir();
        if (!oDir.exists()) {
            if (oDir.mkdir(oDir.path())) {
                chmod(oDir.path().toLocal8Bit().data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                qDebug() << "success to make db path : " << oDir.path();
            } else {
                qDebug() << "fail to make db path : " << oDir.path();
            }
        }

        qDebug() << "alreadly made db path !!!";

        if (m_oDb.isOpen()) {
            m_oDb.close();
        }

        if (m_oDb.open()) {
            chmod(DB_PATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        } else {
            qDebug() << "fail to open DB !!!";
        }

//        if (oDir.mkdir(oDir.path())) {
//            chmod(oDir.path().toLocal8Bit().data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
//            qDebug() << "success to make db path : " << oDir.path();
//            if (m_oDb.isOpen()) {
//                m_oDb.close();
//            }

//            if (m_oDb.open()) {
//                chmod(DB_PATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
//            } else {
//                qDebug() << "fail to open DB !!!";
//            }
//        } else {
//            qDebug() << "fail to make db path : " << oDir.path();
//        }
    } else {
        qDebug() << "alreadly has DB !!!";
        if (!m_oDb.open()) {
            qDebug() << "fail to open DB !!!";
        }
    }

    m_pQuery = new QSqlQuery(m_oDb);
    if (!m_oTables.empty()) {
        m_oTables.clear();
        QString oStr;
        oStr.sprintf(SEARCH_TABLE);
        if (m_pQuery->exec(oStr)) {
            while (m_pQuery->next()) {
                m_oTables << m_pQuery->value(1).toString();
            }
        } else {
            qDebug() << oStr << " failed!!!!!!" << m_pQuery->lastError().text();
        }
    }

    if(!m_oTables.contains(TABLE_NAME))
    {
        if (m_pQuery->exec(QString(CREATE_TABLE).arg(TABLE_NAME))) {
            m_oTables << TABLE_NAME;
        } else {
            qDebug() << QString("FAIL can't create table %1 !!!").arg(TABLE_NAME) << m_pQuery->lastError().text();
        }
    }

    if(!m_oTables.contains(TABLE_ATTACHMENT))
    {
        if (m_pQuery->exec(QString(CREATE_ATTACHMENT).arg(TABLE_ATTACHMENT))) {
            m_oTables << TABLE_ATTACHMENT;
        } else {
            qDebug() << QString("FAIL can't create table %1 !!!").arg(TABLE_ATTACHMENT) << m_pQuery->lastError().text();
        }
    }
}

CMedoDbManager::~CMedoDbManager()
{
    if (m_pQuery) {
        delete m_pQuery;
        m_pQuery = NULL;
    }
}

#include "cmedorecordmanager.h"
#include "cmedorecord.h"
#include "cmedorecordthread.h"

#include <QDebug>
#include <QMetaType>
#include <QDateTime>
#include <sys/stat.h>
#include <QFileInfo>
#include <QDir>

#define ATTACHMENT_PATH "/data/data/com.syberos.medo/attachment"

CMedoRecordManager::CMedoRecordManager(QObject *parent) :
    CMedoListModel(parent), m_pThreadObj(new CMedoRecordThread(parent))
{
    qRegisterMetaType<CMedoRecordManager*>();
    CMedoDbManager::createMedoDbManager();
    m_pThreadObj->moveToThread(&m_oThread);
    connect(this,SIGNAL(sendInitModelToThread()),m_pThreadObj,SLOT(doInitModel()));
    connect(this,SIGNAL(sendStartRecorderToThread(const QString &)),m_pThreadObj,SLOT(doStartRecorder(const QString &)));
    connect(m_pThreadObj,SIGNAL(sendRecord(const QString &,const QString &,const QString &,const QString &)),
            this,SLOT(addRecordToModel(const QString &,const QString &,const QString &,const QString &)));

    connect(m_pThreadObj,SIGNAL(sendAttachment(const QString &,const QString &,const QString &)),
            this,SLOT(addAttachmentToRecord(const QString &,const QString &,const QString &)));

    connect(this,SIGNAL(startQueryAttachments(const QString &)),m_pThreadObj,SLOT(queryAttachments(const QString &)));



    m_oThread.start();

    createPath(ATTACHMENT_PATH);
}

CMedoRecordManager::~CMedoRecordManager()
{
    m_oThread.quit();
    m_oThread.wait();

    delete m_pThreadObj;
    m_pThreadObj = 0;
}

void CMedoRecordManager::initModel()
{
    qDebug() << Q_FUNC_INFO;
    emit sendInitModelToThread();
}

QString CMedoRecordManager::currentTime()
{
    qDebug() << Q_FUNC_INFO;
    return QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");
}

void CMedoRecordManager::newRecord(const QString &id,
                                   const QString &content,
                                   const QString &attachment,
                                   const QString &date)
{
    qDebug() << Q_FUNC_INFO;
    //QString sNewRecordId = QString::number(_list->count() + 1);

    QString sAttachment;
    if (attachment.isEmpty()) {
        sAttachment = getNewAttPath();
    } else {
        sAttachment = attachment;
    }

    CMedoDbManager::instance()->addRecord(id,content, sAttachment, date);

    CMedoRecord *pRecord = new CMedoRecord(id,
                                           content,
                                           sAttachment,
                                           date,
                                           this);
    addItemBegin(pRecord);
    m_oMap[pRecord->id()] = pRecord;
}

void CMedoRecordManager::updateRecord(const QString &id,
                                      const QString &content,
                                      const QString &attachment,
                                      const QString &date)
{
    qDebug() << Q_FUNC_INFO;
    if (m_oMap.contains(id)) {
        qDebug() << Q_FUNC_INFO << id << "\r\n" << content;
        CMedoDbManager::instance()->updateRecord(id, content, attachment, date);
        m_oMap[id]->setId(id);
        m_oMap[id]->setContent(content);
        m_oMap[id]->setAttachment(attachment);
        m_oMap[id]->setDate(date);

        move(indexOf(m_oMap[id]), 0);

        m_oMap[id]->attachmentList()->reset();
        emit startQueryAttachments(id);
    }
}

QString CMedoRecordManager::getNewAttPath()
{
    qDebug() << Q_FUNC_INFO;
    QString sPath =  QString(ATTACHMENT_PATH) + "/" + randomString();
    return sPath;
}

void CMedoRecordManager::startRecorder(const QString &parentId,const QString &path)
{
    qDebug() << Q_FUNC_INFO <<parentId <<  path;
    createPath(path);
    m_sCurentAttFullName = getAttName(path);
    qDebug() << "startRecorder m_sCurentAttFullName = " << m_sCurentAttFullName;
    emit sendStartRecorderToThread(m_sCurentAttFullName);
    CMedoDbManager::instance()->addAttachment(parentId,path,m_sCurentAttFullName);

}

void CMedoRecordManager::stopRecorder()
{

}

void CMedoRecordManager::cancelRecorder()
{

}

void CMedoRecordManager::renameFile(const QString &oldName, const QString &newName)
{
    qDebug() << Q_FUNC_INFO << newName;
    const QFileInfo oFileInfo(oldName);
    QFile oFile(oFileInfo.absoluteFilePath());
    bool bReturn = oFile.rename(oFileInfo.absolutePath() + QDir::separator() + newName);
    if (!bReturn) {
        qDebug()<<"Rename file failed! error: "<<oFile.errorString();
    }
}

bool CMedoRecordManager::removeFile(const QString &name)
{
    qDebug() << Q_FUNC_INFO << name;
    return QFile::remove(name);
}

void CMedoRecordManager::addRecordToModel(const QString &id,
                                          const QString &content,
                                          const QString &attachment,
                                          const QString &date)
{
    qDebug() << Q_FUNC_INFO;
    if (!m_oMap.contains(id)) {
        qDebug() << Q_FUNC_INFO << id << "\r\n" << content;
        CMedoRecord *pRecord = new CMedoRecord(id,
                                               content,
                                               attachment,
                                               date,
                                               this);
        addItem(pRecord);
        m_oMap[pRecord->id()] = pRecord;
    }
}

void CMedoRecordManager::addAttachmentToRecord(const QString &parentId,
                          const QString &path,
                          const QString &name)
{
    if (m_oMap.contains(parentId)) {
        m_oMap[parentId]->addAttachment(parentId,path,name);
    }
}

void CMedoRecordManager::createPath(const QString &path)
{
    qDebug() << Q_FUNC_INFO;
    QDir oDir(path);
    if (oDir.exists()) {
        qDebug() << "alreadly create PATH !!!";
        return;
    }

    if (oDir.mkdir(oDir.path())) {
        chmod(oDir.path().toLocal8Bit().data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        qDebug() << "success to make PATH : " << oDir.path();
    } else {
        qDebug() << "fail to make PATH : " << oDir.path();
    }
}

QString CMedoRecordManager::getAttName(const QString &path)
{
    qDebug() << Q_FUNC_INFO;
    return path + "/" + QDateTime::currentDateTime().toString("yyyyMMdd_hh_mm_ss") +".aac";
}

QString CMedoRecordManager::randomString()
{
    static unsigned long dwInc = 0;

    QString sRandomStr;
    static const int SIZE_CHAR = 15;
    static const char CCH[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand (rand () + (dwInc++ ));
    for (int i = 0; i < SIZE_CHAR; i++)
    {
        int x = rand() / (RAND_MAX / (sizeof(CCH) - 2));
        sRandomStr = sRandomStr + CCH[x];
    }
    return sRandomStr;
}

#include "cmedorecordthread.h"
#include "cmedorecord.h"
#include "cmedodbmanager.h"
#include "cmedoattmanager.h"

CMedoRecordThread::CMedoRecordThread(QObject *parent) :
    QObject(parent)
{
}

void CMedoRecordThread::doInitModel()
{
    DBRecordList oList = CMedoDbManager::instance()->getRecordList();
    for (int i = 0; i < oList.size(); ++i) {
        emit sendRecord(oList.at(i).id, oList.at(i).content, oList.at(i).attachment, oList.at(i).date);
        qDebug() << Q_FUNC_INFO << oList.at(i).content;
    }
}

void CMedoRecordThread::doStartRecorder(const QString &path)
{
    qDebug() << Q_FUNC_INFO << path;
    CMedoAttManager::instance()->startRecorder(path);
}

void CMedoRecordThread::queryAttachments(const QString &parentId)
{
    DBAttachmentList oList = CMedoDbManager::instance()->getAttachments(parentId);
    for (int i = 0; i < oList.size(); ++i) {
        emit sendAttachment(oList.at(i).parentId, oList.at(i).path, oList.at(i).name);
    }
}

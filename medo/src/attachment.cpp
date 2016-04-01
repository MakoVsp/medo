#include "attachment.h"
#include <QFileInfo>

Attachment::Attachment(const QString &parentId,
                       const QString &path,
                       const QString &name,
                       QObject *parent) :
    QObject(parent),
     m_sParentId(parentId),
     m_sPath(path),
     m_sName(name)
{
}

QString Attachment::parentId()
{
    return m_sParentId;
}

void Attachment::setParentId(const QString &parentId)
{
    if(m_sParentId != parentId) {
        m_sParentId = parentId;
        emit parentIdChanged();
    }
}

QString Attachment::path()
{
    return m_sPath;
}

void Attachment::setPath(const QString &path)
{
    if(m_sPath != path) {
        m_sPath = path;
        emit pathChanged();
    }
}

QString Attachment::name()
{
    QFileInfo info(m_sName);
    return info.fileName();
}

void Attachment::setName(const QString &name)
{
    if(m_sName != name) {
        m_sName = name;
        emit nameChanged();
    }
}

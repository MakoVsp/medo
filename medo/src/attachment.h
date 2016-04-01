#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include <QObject>

class Attachment : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString parentId READ parentId WRITE setParentId NOTIFY parentIdChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit Attachment(const QString &parentId,
                        const QString &path,
                        const QString &name,
                        QObject *parent = 0);
    QString parentId();
    void setParentId(const QString &parentId);

    QString path();
    void setPath(const QString &path);

    QString name();
    void setName(const QString &name);

signals:
    void parentIdChanged();
    void pathChanged();
    void nameChanged();

private:
    QString m_sParentId;
    QString m_sPath;
    QString m_sName;

};

#endif // ATTACHMENT_H

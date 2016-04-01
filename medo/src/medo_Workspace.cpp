#include "medo_Workspace.h"
#include "cmedorecordmanager.h"
#include "cmedoattmanager.h"

#include <QDebug>
#include <QQmlContext>
#include "cmedolistmodel.h"

medo_Workspace::medo_Workspace()
    : CWorkspace()
{
    qmlRegisterType<CMedoListModel>("CMedoListModel", 1, 0, "CMedoListModel");
    m_pMedoRecordManager = QSharedPointer<CMedoRecordManager>(new CMedoRecordManager());
    if (m_pMedoRecordManager.data()) {
        m_pMedoRecordManager.data()->initModel();
    } else {
        qDebug() << "m_pMedoRecordManager init error !!!";
    }

    m_pMedoAttManager = QSharedPointer<CMedoAttManager>(new CMedoAttManager());
    if (!m_pMedoAttManager.data()) {
        qDebug() << "m_pMedoAttManager init error !!!";
    }

    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    m_view->engine()->rootContext()->setContextProperty("medoRecordManager", m_pMedoRecordManager.data());
    m_view->engine()->rootContext()->setContextProperty("medoAttManager", m_pMedoAttManager.data());

    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();
}

void medo_Workspace::onLaunchComplete(Option option, const QStringList& params)
{
    Q_UNUSED(params)

    switch (option) {
    case CWorkspace::HOME:
        qDebug()<< "Start by Home";
        break;
    case CWorkspace::URL:
        break;
    case CWorkspace::EVENT:
        break;
    case CWorkspace::DOCUMENT:
        break;
    default:
        break;
    }
}



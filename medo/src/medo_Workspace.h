#ifndef __MEDO_WORKSPACE__
#define __MEDO_WORKSPACE__

#include <QQuickView>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <SyberosGuiCache>
#include <cgui_application.h>
#include <cworkspace.h>

#include <QScopedPointer>

using namespace SYBEROS;
class CMedoRecordManager;
class CMedoAttManager;

class medo_Workspace : public CWorkspace
{
    Q_OBJECT

private:
    QQuickView *m_view;

public:
    medo_Workspace();

    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);

private:
    QSharedPointer<CMedoRecordManager> m_pMedoRecordManager;
    QSharedPointer<CMedoAttManager> m_pMedoAttManager;
};


#endif //__MEDO_WORKSPACE__


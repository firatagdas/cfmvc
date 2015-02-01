#ifndef CFMVC_H
#define CFMVC_H

#include <QObject>
#include <QMap>
#include <QLoggingCategory>
#include <qqml.h>
#include "cfmvc_global.h"

#ifndef CFMODEL_PACKAGE
#define CFMODEL_PACKAGE "CFMvc"
#endif

#ifndef CFMODEL_PACKAGE_MAJOR
#define CFMODEL_PACKAGE_MAJOR 1
#endif

#ifndef CFMODEL_PACKAGE_MINOR
#define CFMODEL_PACKAGE_MINOR 0
#endif

Q_DECLARE_LOGGING_CATEGORY(LCFMvc)

class CFModel;

class Q_CFMVC_EXPORT CFMvc : public QObject
{
    Q_OBJECT
public:
    CFMvc();
    ~CFMvc();

    static CFMvc *instance();

    bool isRegistered(const char *name);
    Q_INVOKABLE bool isRegistered(const QString &name);
    Q_INVOKABLE bool registerModel(CFModel *model);
    Q_INVOKABLE bool unregisterModel(const QString &name);
    CFModel *model(const QString &name);

private:
    static CFMvc *m_instance;
    QMap<QString, CFModel *> m_modelMap;
};

class Q_CFMVC_EXPORT CFGlobal : public QObject
{
    Q_OBJECT
    Q_ENUMS(PageAnimationType)

public:
    enum PageAnimationType {
        PageAnimationHorizontalSlide,
        PageAnimationVerticalSlide,
        PageAnimationHorizontalPopover,
        PageAnimationVerticalPopover,
        PageAnimationVerticalExpose,
        PageAnimationHorizontalExpose
    };

    CFGlobal(QObject *parent = 0) : QObject(parent) {}
};

QML_DECLARE_TYPE(CFGlobal)
Q_DECLARE_METATYPE(CFGlobal::PageAnimationType)

#endif // CFMVC_H

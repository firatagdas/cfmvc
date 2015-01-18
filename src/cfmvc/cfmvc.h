#ifndef CFMVC_H
#define CFMVC_H

#include <QObject>
#include <QMap>
#include <QLoggingCategory>
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
typedef const char * CFModelId;

class Q_CFMVC_EXPORT CFMvc : public QObject
{
    Q_OBJECT
public:
    CFMvc();
    ~CFMvc();

    static CFMvc *instance();

    bool isRegistered(CFModelId name);
    bool registerModel(CFModel *model);
    bool unregisterModel(CFModelId name);
    CFModel *model(CFModelId name);

Q_SIGNALS:

public Q_SLOTS:

private:
    static CFMvc *m_instance;
    QMap<CFModelId, CFModel *> m_modelMap;
};

#endif // CFMVC_H

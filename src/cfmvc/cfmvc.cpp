#include "cfmvc.h"
#include "cfmodel.h"

Q_LOGGING_CATEGORY(LCFMvc, "LCFMvc")

CFMvc *CFMvc::m_instance = 0;

CFMvc::CFMvc()
    : QObject(0)
{
}

CFMvc::~CFMvc()
{
}

CFMvc *CFMvc::instance()
{
    if (!m_instance)
        m_instance = new CFMvc();

    return m_instance;
}

bool CFMvc::isRegistered(const char *name)
{
    return isRegistered(QLatin1String(name));
}

bool CFMvc::isRegistered(const QString &name)
{
    return m_modelMap.value(name);
}

bool CFMvc::registerModel(CFModel *model)
{
    QString modelId = model->name();
    if (isRegistered(modelId)) {
        qCDebug(LCFMvc) << modelId << "is already registered.";
        return false;
    } else {
        qCDebug(LCFMvc) << modelId << "is registering" << model;
    }

    m_modelMap.insert(modelId, model);
    emit model->registered();

    return true;
}

bool CFMvc::unregisterModel(const QString &name)
{
    if (!isRegistered(name))
        return false;

    CFModel *model = m_modelMap.value(name);
    emit model->unregistered();

    QObject *modelParent = model->parent();
    if (modelParent != 0 && qobject_cast<CFMvc *>(modelParent) == this) {
        model->setParent(0);
        model->deleteLater();
    }

    return true;
}

CFModel *CFMvc::model(const QString &name)
{
    CFModel *model = 0;
    if (isRegistered(name))
        model = m_modelMap.value(name);

    return model;
}



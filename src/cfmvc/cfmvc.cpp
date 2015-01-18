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

bool CFMvc::isRegistered(CFModelId name)
{
    return m_modelMap.contains(name);
}

bool CFMvc::registerModel(CFModel *model)
{
    CFModelId modelId = model->name();
    if (isRegistered(modelId)) {
        qCDebug(LCFMvc) << modelId << "is already registered.";
        return false;
    }

    m_modelMap[modelId] = model;
    emit model->registered();

    return true;
}

bool CFMvc::unregisterModel(CFModelId name)
{
    if (!isRegistered(name))
        return false;

    CFModel *model = 0;
    model = m_modelMap.value(name);
    emit model->unregistered();

    QObject *modelParent = model->parent();
    if (modelParent != 0 && qobject_cast<CFMvc *>(modelParent) == this) {
        model->setParent(0);
        model->deleteLater();
    }

    return true;
}

CFModel *CFMvc::model(CFModelId name)
{
    CFModel *model = 0;
    if (isRegistered(name))
        model = m_modelMap.value(name);

    return model;
}



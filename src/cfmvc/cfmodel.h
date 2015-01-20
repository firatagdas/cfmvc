#ifndef CFMODEL_H
#define CFMODEL_H

#include <QObject>
#include <QJSEngine>
#include <QQmlEngine>
#include <qqml.h>
#include "cfmvc_global.h"
#include "cfmvc.h"

#define CFMVC_MODEL(ModelType) createCFModel_ ## ModelType
#define CFMVC_DEFINE_MODEL_WITH_OPTIONS(ModelType, LazyMode) \
    static inline QObject *CFMVC_MODEL(ModelType)(QQmlEngine *, QJSEngine *) \
    { \
        CFMvc *mvc = CFMvc::instance(); \
        ModelType *model = 0; \
        if (!mvc->isRegistered(#ModelType)) { \
            if ((LazyMode)) { \
                qDebug() << #ModelType << "creating in lazy mode."; \
                model = new ModelType(mvc); \
                model->setParent(mvc); \
                mvc->registerModel(model); \
            } \
        } \
        return model; \
    }

#define CFMVC_DEFINE_LAZY_MODEL(ModelType) CFMVC_DEFINE_MODEL_WITH_OPTIONS(ModelType, true)
#define CFMVC_DEFINE_MODEL(ModelType) CFMVC_DEFINE_MODEL_WITH_OPTIONS(ModelType, false)

#define CFMVC_MODEL_OBJECT(T) \
    Q_PROPERTY(CFModelId name READ name CONSTANT) \
    public: \
        static const char *NAME; \
        CFModelId name() { return NAME; }

#define CFMVC_MODEL_BEGIN(T) \
    CFModelId T::NAME = #T ;

#define CFMVC_MODEL_END(T)

class Q_CFMVC_EXPORT CFModel : public QObject
{
    Q_OBJECT
public:
    CFModel() : QObject(0) {} // QML for meta registration
    explicit CFModel(CFMvc *mvc, const QVariantMap &config = QVariantMap()) // real registration
        : QObject(0)
        , m_mvc(mvc)
    {
        setConfig(config);
    }

    ~CFModel() {}

    void setConfig(const QVariantMap &config) { m_config = config; apply(); }
    QVariant config() { return m_config; }
    virtual void apply() {}

    virtual CFModelId name() = 0;

    CFMvc *mvc() { return m_mvc; }

Q_SIGNALS:
    void registered();
    void applicationLoaded();
    void unregistered();
    void applicationUnloaded();

public Q_SLOTS:

protected:
    CFMvc *m_mvc;
    QVariantMap m_config;
};

template <typename T>
CFModel *CFMvcPreDefinedModelRegister(const QVariant &config = QVariant(), bool autoDestroy = false)
{
    CFMvc *mvc = CFMvc::instance();
    T *model = new T(mvc, config);
    mvc->registerModel(model);
    if (autoDestroy)
        model->setParent(mvc);

    return model;
}

template <typename T>
CFModel *CFMvcModelRegister(QObject *(*callback)(QQmlEngine *, QJSEngine *), const QVariant &config = QVariant(), bool autoDestroy = false, const char *package = CFMODEL_PACKAGE, int major = CFMODEL_PACKAGE_MAJOR, int minor = CFMODEL_PACKAGE_MINOR)
{
    CFModel *model = CFMvcPreDefinedModelRegister<T>(config, autoDestroy);
    qmlRegisterSingletonType<T>(package, major, minor, (const char *) model->name(), callback);

    return model;
}

#endif // CFMODEL_H

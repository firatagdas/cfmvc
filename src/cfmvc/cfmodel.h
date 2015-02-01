#ifndef CFMODEL_H
#define CFMODEL_H

#include <QObject>
#include <QJSEngine>
#include <QQmlEngine>
#include <qqml.h>
#include "cfmvc_global.h"
#include "cfmvc.h"

#define CFMVC_MODEL(T) createCFModel_ ## T
#define CFMVC_DEFINE_MODEL_WITH_OPTIONS(T, LazyMode) \
    static inline QObject *createCFModel_ ## T(QQmlEngine *, QJSEngine *) \
    { \
        CFMvc *mvc = CFMvc::instance(); \
        CFModel *model = 0; \
        if (!mvc->isRegistered(QLatin1String(#T))) { \
            if ((LazyMode)) { \
                qDebug() << #T << "creating in lazy mode."; \
                model = new T(mvc); \
                model->setParent(mvc); \
                mvc->registerModel(model); \
            } else { \
                qWarning() << #T << "is not a registered model."; \
            } \
        } else { \
            model = mvc->model(QLatin1String(#T)); \
        } \
        return model; \
    }

#define CFMVC_DEFINE_LAZY_MODEL(T) CFMVC_DEFINE_MODEL_WITH_OPTIONS(T, true)
#define CFMVC_DEFINE_MODEL(T) CFMVC_DEFINE_MODEL_WITH_OPTIONS(T, false)

#define CFMVC_MODEL_OBJECT(T) \
    Q_PROPERTY(QString name READ name CONSTANT) \
    public: \
        static const char *NAME; \
        QString name() { return QLatin1String(NAME); }

#define CFMVC_MODEL_BEGIN(T) \
    const char *T::NAME = #T ;

#define CFMVC_MODEL_END(T)

#define CFMVC_PRIVATE_GLOBAL(T) T ## _Instance

#define CFMVC_DEFINE_PRIVATE_GLOBAL_INSTANCE(T) \
    typedef QMap<quintptr, T *> CFMVC_PRIVATE_GLOBAL(T); \
    Q_GLOBAL_STATIC(CFMVC_PRIVATE_GLOBAL(T), g_ ## T ## _InstanceList)

#define CFMVC_PRIVATE_GLOBAL_S(T) \
    const CFMVC_PRIVATE_GLOBAL(T) &dGlobal = *g_ ## T ## _InstanceList;

#define CFMVC_PRIVATE_GLOBAL_D(T, ID) \
    CFMVC_PRIVATE_GLOBAL_S(T) \
    T *d = dGlobal[ID]; \
    if (!d) \
        return;

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

    virtual QString name() = 0;

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
CFModel *CFMvcPreDefinedModelRegister(const QVariantMap &config = QVariantMap(), bool autoDestroy = true)
{
    CFMvc *mvc = CFMvc::instance();
    T *model = new T(mvc, config);
    mvc->registerModel(model);
    if (autoDestroy)
        model->setParent(mvc);

    return model;
}

template <typename T>
CFModel *CFMvcModelRegister(QObject *(*callback)(QQmlEngine *, QJSEngine *), const QVariantMap &config = QVariantMap(), bool autoDestroy = true, const char *package = CFMODEL_PACKAGE, int major = CFMODEL_PACKAGE_MAJOR, int minor = CFMODEL_PACKAGE_MINOR)
{
    CFModel *model = CFMvcPreDefinedModelRegister<T>(config, autoDestroy);
    qmlRegisterSingletonType<T>(package, major, minor, model->name().toUtf8(), callback);

    return model;
}

#endif // CFMODEL_H

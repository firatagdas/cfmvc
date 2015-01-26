#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>
#include <CFMvc/cfmodel.h>
#include <CFMvc/cfapimodel.h>
#include <CFMvc/cfassetmodel.h>
#include <CFMvc/cfscreenmodel.h>

CFMVC_DEFINE_MODEL(CFApiModel)
CFMVC_DEFINE_MODEL(CFAssetModel)
CFMVC_DEFINE_MODEL(CFScreenModel)

QT_BEGIN_NAMESPACE

class CFMvcModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("CFMvc"));

        // @uri CFMvc
        qmlRegisterSingletonType<CFApiModel>(uri, 1, 0, "CFApiModel", CFMVC_MODEL(CFApiModel));
        qmlRegisterSingletonType<CFAssetModel>(uri, 1, 0, "CFAssetModel", CFMVC_MODEL(CFAssetModel));
        qmlRegisterSingletonType<CFScreenModel>(uri, 1, 0, "CFScreenModel", CFMVC_MODEL(CFScreenModel));
    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(uri);
        Q_UNUSED(engine);
    }
};

QT_END_NAMESPACE

#include "cfmvc.moc"




#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>

QT_BEGIN_NAMESPACE

class CFMvcModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("CFMvc"));

        // @uri QtWebView
        // qmlRegisterType<C>(uri, 1, 0, "WebView");
    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(uri);
        Q_UNUSED(engine);
    }
};

QT_END_NAMESPACE

#include "cfmvc.moc"




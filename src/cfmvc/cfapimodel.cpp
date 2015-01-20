#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegExp>
#include "cfapimodel.h"
#include "cfapimodel_p.h"

CFMVC_MODEL_BEGIN(CFApiModel)

CFApiModel::CFApiModel(CFMvc *mvc, const QVariantMap &config)
    : CFModel(mvc, config)
    , d_ptr(new CFApiModelPrivate(this))
{
    connect(this, SIGNAL(registered()), this, SLOT(onRegistered()), Qt::DirectConnection);
}

CFApiModel::~CFApiModel()
{
    clearRoute();
    delete d_ptr;
}

void CFApiModel::onRegistered()
{
    Q_D(CFApiModel);
    d->init();
}

bool CFApiModel::addRoute(const QString &name,
                          CFApiModel::RequestMethod requestMethod,
                          const QString &path,
                          const QVariantMap &defaultParams)
{
    Q_D(CFApiModel);
    if (hasRoute(name))
        return false;

    QString urlPath = path;
    if (!urlPath.isEmpty() && urlPath.startsWith(QLatin1Char('/')))
        urlPath = urlPath.mid(1);

    CFApiRoute *route = new CFApiRoute();
    route->name = name;
    route->requestMethod = requestMethod;
    route->path = urlPath;
    route->defaultParams = defaultParams;

    d->m_routes.insert(name, route);

    return true;
}

bool CFApiModel::removeRoute(const QString &name)
{
    Q_D(CFApiModel);
    if (!hasRoute(name))
        return false;

    d->m_routes.remove(name);
    return true;
}

bool CFApiModel::hasRoute(const QString &name) const
{
    Q_D(const CFApiModel);
    return d->m_routes.contains(name);
}

quint64 CFApiModel::route(const QString &name, const QVariantMap &params)
{
    if (!hasRoute(name)) {
        qCDebug(LCFMvc) << "Route is not defined." << name;
        return 0;
    }

    Q_D(CFApiModel);
    if (d->m_url.isEmpty()) {
        qCDebug(LCFMvc) << "URL should be defined.";
        return 0;
    }

    CFApiRoute *route = d->m_routes.value(name);
    QVariantMap mergedParams = route->defaultParams;

    QVariantMap::const_iterator it;
    for (it = params.constBegin(); it != params.constEnd(); ++it)
        mergedParams.insert(it.key(), it.value());

    QString url = buildUrl(d->m_url + route->path, mergedParams);
    return d->request(url, route->requestMethod, mergedParams);
}

void CFApiModel::clearRoute()
{
    Q_D(CFApiModel);

    QMap<QString, CFApiRoute *>::iterator it;
    for (it = d->m_routes.begin(); it != d->m_routes.end(); ++it)
        delete it.value();

    d->m_routes.clear();
}

QString CFApiModel::buildUrl(const QString &url, const QVariantMap &params)
{
    QRegExp regex(QLatin1String("\\{\\{([a-zA-Z0-9_\\-]+)\\}\\}"));
    int pos = 0;
    QString newUrl = url;
    while ((pos = regex.indexIn(url, pos)) != -1) {
        if (params.contains(regex.cap(1)))
            newUrl = newUrl.replace(pos, regex.matchedLength(), params.value(regex.cap(1)).toString());
        else
            newUrl = newUrl.replace(pos, regex.matchedLength(), QLatin1String("Undefined"));

        pos += regex.matchedLength();
    }

    return newUrl;
}

void CFApiModel::setUrl(const QString &url)
{
    Q_D(CFApiModel);
    if (d->m_url != url) {
        d->m_url = url;
        if (!d->m_url.endsWith(QLatin1Char('/')))
            d->m_url += QLatin1Char('/');

        emit urlChanged();
    }
}

QString CFApiModel::url() const
{
    Q_D(const CFApiModel);
    return d->m_url;
}

void CFApiModel::apply()
{
    if (m_config.isEmpty())
        return;

    if (m_config.contains(QLatin1String("url")))
        setUrl(m_config.value(QLatin1String("url")).toString());

    if (m_config.contains(QLatin1String("defaultParams"))
            && m_config.value(QLatin1String("defaultParams")).type() == QVariant::Map)
        setDefaultParams(m_config.value(QLatin1String("defaultParams")).toMap());

    if (m_config.contains(QLatin1String("routes"))
            && m_config.value(QLatin1String("routes")).type() == QVariant::List) {
        QVariantList routes = m_config.value(QLatin1String("routes")).toList();
        int i = 0;
        int len = routes.count();
        QVariantMap info;
        QString name;
        QString path;
        CFApiModel::RequestMethod requestMethod;
        QVariantMap defaultParams;

        for (; i < len; i++) {
            if (routes.at(i).type() != QVariant::Map) {
                qCWarning(LCFMvc) << "Route information should be object.";
                continue;
            }

            info = routes.at(i).toMap();

            // name required
            if (!info.contains(QLatin1String("name")) || info.value(QLatin1String("name")).type() != QVariant::String) {
                qCWarning(LCFMvc) << "Route information requires name.";
                continue;
            }

            name = info.value(QLatin1String("name")).toString();
            path = info.contains(QLatin1String("path")) ? info.value(QLatin1String("path")).toString() : QLatin1String("");
            requestMethod = info.contains(QLatin1String("requestMethod"))
                    ? static_cast<CFApiModel::RequestMethod>(info.value(QLatin1String("requestMethod")).toInt())
                    : CFApiModel::RequestMethodGet;
            defaultParams = info.contains(QLatin1String("defaultParams")) ? info.value(QLatin1String("defaultParams")).toMap() : QVariantMap();

            addRoute(name, requestMethod, path, defaultParams);
        }
    }

}

CFMVC_MODEL_END(CFApiModel)

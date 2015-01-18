#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegExp>
#include "cfapimodel.h"
#include "cfapimodel_p.h"

CFMVC_MODEL_BEGIN(CFApiModel)

CFApiModel::CFApiModel(CFMvc *mvc, const QVariant &config)
    : CFModel(mvc, config)
    , d_ptr(new CFApiModelPrivate(this))
{
    connect(this, SIGNAL(registered()), this, SLOT(onRegistered()), Qt::DirectConnection);
}

CFApiModel::~CFApiModel()
{
}

void CFApiModel::onRegistered()
{
    Q_D(CFApiModel);
    d->init();
}

bool CFApiModel::addRoute(const QString &name,
                          CFApiModel::RequestMethod requestMethod,
                          const QString &path,
                          const QVariantMap &defaultParams,
                          const QStringList &requiredParams)
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
    route->requiredParams = requiredParams;

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

QString CFApiModel::buildUrl(const QString &url, const QVariantMap &params)
{
    QRegExp regex("\\{\\{([a-zA-Z0-9_\\-]+)\\}\\}");
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

CFMVC_MODEL_END(CFApiModel)

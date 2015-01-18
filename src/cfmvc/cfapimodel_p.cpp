#include "cfapimodel_p.h"
#include <QJsonDocument>
#include <QJsonParseError>

static const char CFRequestIdQueryItem[] = "__cf_id";

CFApiModelPrivate::CFApiModelPrivate(CFApiModel *q)
    : QObject()
    , q_ptr(q)
    , m_networkAccessManager(0)
    , m_requestId(0)
    , m_inited(false)
{
}

CFApiModelPrivate::~CFApiModelPrivate()
{
    if (m_inited)
        m_networkAccessManager->deleteLater();
}

void CFApiModelPrivate::init()
{
    if (m_inited)
        return;

    m_networkAccessManager = new QNetworkAccessManager();
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReply(QNetworkReply*)));

    m_inited = true;
}

int CFApiModelPrivate::request(const QString &url, CFApiModel::RequestMethod requestMethod, const QVariantMap &data)
{
    if (!m_inited) {
        qCCritical(LCFMvc) << "model is not inited.";
        return 0;
    }

    QNetworkRequest request;
    QUrl newUrl(url);
    QUrlQuery query;

    m_requestId++;

    bool canPostable = requestMethod == CFApiModel::RequestMethodPost || requestMethod == CFApiModel::RequestMethodPut;
    if (!canPostable)
        query.setQuery(newUrl.query());
    else {
        request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded; charset=UTF-8"));

        QUrlQuery existingQuery(newUrl.query());
        if (existingQuery.hasQueryItem(QLatin1String(CFRequestIdQueryItem)))
            existingQuery.removeQueryItem(QLatin1String(CFRequestIdQueryItem));

        existingQuery.addQueryItem(QLatin1String(CFRequestIdQueryItem), QString::number(m_requestId));
        newUrl.setQuery(existingQuery);
    }

    QVariantMap::const_iterator it;
    for (it = data.constBegin(); it != data.constEnd(); ++it)
        query.addQueryItem(it.key(), it.value().toString());

    if (!canPostable)
        newUrl.setQuery(query);

    request.setUrl(newUrl);
    QNetworkReply *reply = 0;
    switch (requestMethod) {
    case CFApiModel::RequestMethodGet:
        reply = m_networkAccessManager->get(request);
        break;
    case CFApiModel::RequestMethodPost:
        reply = m_networkAccessManager->post(request, query.toString(QUrl::FullyEncoded).toUtf8());
        break;
    case CFApiModel::RequestMethodPut:
        reply = m_networkAccessManager->put(request, query.toString(QUrl::FullyEncoded).toUtf8());
        break;
    case CFApiModel::RequestMethodDelete:
        reply = m_networkAccessManager->deleteResource(request);
        break;
    case CFApiModel::RequestMethodHead:
        reply = m_networkAccessManager->head(request);
        break;
    }

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));

    return 0;
}

quint64 CFApiModelPrivate::parseRequestId(const QUrl &url)
{
    QUrlQuery query(url.query());
    quint64 requestId = 0;

    if (query.hasQueryItem(QLatin1String(CFRequestIdQueryItem))) {
        bool ok = false;
        requestId = query.queryItemValue(CFRequestIdQueryItem).toInt(&ok);
        if (!ok)
            requestId = 0;
    }

    return requestId;
}

void CFApiModelPrivate::onReply(QNetworkReply *reply)
{
    QUrl url = reply->url();
    quint64 requestId = parseRequestId(url);
    QByteArray response = reply->readAll();
    CFApiModel::ApiError error = static_cast<CFApiModel::ApiError>(reply->error());
    QString errorString = reply->errorString();

    qCDebug(LCFMvc) << url
                    << "Request Id" << requestId
                    << "Status Code" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                    << "Content Type" << reply->header(QNetworkRequest::ContentTypeHeader)
                    << "Body" << response
                    << "Error Code" << error
                    << "Error String" << errorString;

    Q_Q(CFApiModel);
    if (error == CFApiModel::NoError) {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qCDebug(LCFMvc) << "JSON Parse error" << parseError.error << parseError.errorString();

            errorString = parseError.errorString();
            error = CFApiModel::JsonParseError;

            emit q->error(requestId, error, errorString);

        } else {
            emit q->reply(requestId, doc.toVariant());
        }
    } else {
        emit q->error(requestId, error, errorString);
    }
}

void CFApiModelPrivate::onError(QNetworkReply::NetworkError networkError)
{
    Q_Q(CFApiModel);

    CFApiModel::ApiError error = static_cast<CFApiModel::ApiError>(networkError);
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    quint64 requestId = parseRequestId(reply->url());

    emit q->error(requestId, error, reply->errorString());
}

void CFApiModelPrivate::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_Q(CFApiModel);

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    quint64 requestId = parseRequestId(reply->url());

    emit q->downloadProgress(requestId, bytesReceived, bytesTotal);
}


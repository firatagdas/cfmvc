#ifndef CFAPIMODEL_H
#define CFAPIMODEL_H

#include <QObject>
#include <QNetworkReply>
#include "cfmodel.h"

class CFApiModelPrivate;

class CFApiModel : public CFModel
{
    Q_OBJECT
    Q_ENUMS(ApiError)
    Q_ENUMS(RequestMethod)

    CFMVC_MODEL_OBJECT(CFApiModel)

    enum RequestMethod {
        RequestMethodGet,
        RequestMethodPost,
        RequestMethodPut,
        RequestMethodHead,
        RequestMethodDelete
    };

    enum ApiError {
        NoError = QNetworkReply::NoError,

        // network layer errors [relating to the destination server] (1-99):
        ConnectionRefusedError = QNetworkReply::ConnectionRefusedError,
        RemoteHostClosedError = QNetworkReply::RemoteHostClosedError,
        HostNotFoundError = QNetworkReply::HostNotFoundError,
        TimeoutError = QNetworkReply::TimeoutError,
        OperationCanceledError = QNetworkReply::OperationCanceledError,
        SslHandshakeFailedError = QNetworkReply::SslHandshakeFailedError,
        TemporaryNetworkFailureError = QNetworkReply::TemporaryNetworkFailureError,
        NetworkSessionFailedError = QNetworkReply::NetworkSessionFailedError,
        BackgroundRequestNotAllowedError = QNetworkReply::BackgroundRequestNotAllowedError,
        UnknownNetworkError = QNetworkReply::UnknownNetworkError,

        // proxy errors (101-199):
        ProxyConnectionRefusedError = QNetworkReply::ProxyConnectionRefusedError,
        ProxyConnectionClosedError = QNetworkReply::ProxyConnectionClosedError,
        ProxyNotFoundError = QNetworkReply::ProxyNotFoundError,
        ProxyTimeoutError = QNetworkReply::ProxyTimeoutError,
        ProxyAuthenticationRequiredError = QNetworkReply::ProxyAuthenticationRequiredError,
        UnknownProxyError = QNetworkReply::UnknownProxyError,

        // content errors (201-299):
        ContentAccessDenied = QNetworkReply::ContentAccessDenied,
        ContentOperationNotPermittedError = QNetworkReply::ContentOperationNotPermittedError,
        ContentNotFoundError = QNetworkReply::ContentNotFoundError,
        AuthenticationRequiredError = QNetworkReply::AuthenticationRequiredError,
        ContentReSendError = QNetworkReply::ContentReSendError,
        ContentConflictError = QNetworkReply::ContentConflictError,
        ContentGoneError = QNetworkReply::ContentGoneError,
        UnknownContentError = QNetworkReply::UnknownContentError,

        // protocol errors
        ProtocolUnknownError = QNetworkReply::ProtocolUnknownError,
        ProtocolInvalidOperationError = QNetworkReply::ProtocolInvalidOperationError,
        ProtocolFailure = QNetworkReply::ProtocolFailure,

        // Server side errors (401-499)
        InternalServerError = QNetworkReply::InternalServerError,
        OperationNotImplementedError = QNetworkReply::OperationNotImplementedError,
        ServiceUnavailableError = QNetworkReply::ServiceUnavailableError,
        UnknownServerError = QNetworkReply::UnknownServerError,

        JsonParseError = 9999
    };

public:
    explicit CFApiModel(CFMvc *mvc, const QVariantMap &config = QVariantMap());
    ~CFApiModel();

    bool addRoute(const QString &name,
                  RequestMethod requestMethod = RequestMethodGet,
                  const QString &path = QString(),
                  const QVariantMap &defaultParams = QVariantMap());
    bool removeRoute(const QString &name);
    bool hasRoute(const QString &name) const;
    void clearRoute();

    quint64 route(const QString &name, const QVariantMap &params = QVariantMap());

    void setDefaultParams(const QVariant &defaultParams);
    QVariant defaultParams() const;

    void setUrl(const QString &url);
    QString url() const;

    void apply();

protected:
    QString buildUrl(const QString &url, const QVariantMap &params);

Q_SIGNALS:
    void urlChanged();

    void downloadProgress(quint64 requestId, qint64 bytesReceived, qint64 bytesTotal);
    void error(quint64 requestId, CFApiModel::ApiError error, const QString &errorString);
    void reply(quint64 requestId, const QVariant &data);

public Q_SLOTS:
    void onRegistered();

private:
    Q_DECLARE_PRIVATE(CFApiModel)
    CFApiModelPrivate *d_ptr;
};

QML_DECLARE_TYPE(CFApiModel)
QML_DECLARE_TYPE(CFApiModel::RequestMethod)

#endif // CFAPIMODEL_H

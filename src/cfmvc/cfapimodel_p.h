#ifndef CFAPIMODEL_P_H
#define CFAPIMODEL_P_H

#include <QObject>
#include "cfapimodel.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

class CFApiModel;

class CFApiRoute
{
public:
    CFApiRoute()
        : requestMethod(CFApiModel::RequestMethodGet)
    {}

    QString name;
    CFApiModel::RequestMethod requestMethod;
    QString path;
    QVariantMap defaultParams;
    QStringList requiredParams;
};

class CFApiModelPrivate : public QObject
{
    Q_OBJECT
public:
    explicit CFApiModelPrivate(CFApiModel *q);
    ~CFApiModelPrivate();

    void init();
    int request(const QString &url, CFApiModel::RequestMethod requestMethod, const QVariantMap &data);

protected:
    quint64 parseRequestId(const QUrl &url);

public Q_SLOTS:
    void onReply(QNetworkReply *reply);
    void onError(QNetworkReply::NetworkError error);
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    Q_DECLARE_PUBLIC(CFApiModel)
    CFApiModel *q_ptr;

    QMap<QString, CFApiRoute *> m_routes;
    QNetworkAccessManager *m_networkAccessManager;
    QString m_url;
    quint64 m_requestId;
    bool m_inited;
};

#endif // CFAPIMODEL_P_H

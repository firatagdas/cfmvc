#ifndef CFSCREENMODEL_H
#define CFSCREENMODEL_H

#include <QObject>
#include "cfmodel.h"

class Q_CFMVC_EXPORT CFScreenModel : public CFModel
{
    Q_OBJECT
    Q_PROPERTY(qreal dpi READ dpi CONSTANT)
    Q_PROPERTY(qreal scaleFactor READ scaleFactor CONSTANT)
    Q_PROPERTY(qreal textScaleFactor READ textScaleFactor WRITE setTextScaleFactor NOTIFY textScaleFactorChanged)

    CFMVC_MODEL_OBJECT(CFScreenModel)

public:
    explicit CFScreenModel(CFMvc *mvc, const QVariantMap &config = QVariantMap());
    ~CFScreenModel();

    Q_INVOKABLE qreal dp(qreal px);
    Q_INVOKABLE qreal sp(qreal px);

    void setTextScaleFactor(qreal textScaleFactor);
    qreal textScaleFactor();

    qreal dpi();
    qreal scaleFactor();

Q_SIGNALS:
    void textScaleFactorChanged();

public Q_SLOTS:
    void onRegistered();

private:
    qreal m_dpi;
    qreal m_scaleFactor;
    qreal m_textScaleFactor;
};

QML_DECLARE_TYPE(CFScreenModel)

#endif // CFSCREENMODEL_H

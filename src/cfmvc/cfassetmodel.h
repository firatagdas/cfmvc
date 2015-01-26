#ifndef CFASSETMODEL_H
#define CFASSETMODEL_H

#include <QObject>

#include "cfmodel.h"

class Q_CFMVC_EXPORT CFAssetModel : public CFModel
{
    Q_OBJECT
    Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY(QString qmlPath READ qmlPath WRITE setQmlPath NOTIFY qmlPathChanged)
    Q_PROPERTY(QString fontPath READ fontPath WRITE setFontPath NOTIFY fontPathChanged)
    Q_PROPERTY(QString translationPath READ translationPath WRITE setTranslationPath NOTIFY translationPathChanged)

    CFMVC_MODEL_OBJECT(CFAssetModel)

public:
    explicit CFAssetModel(CFMvc *mvc, const QVariantMap &config = QVariantMap());
    ~CFAssetModel();

    void setImagePath(const QString &imagePath);
    QString imagePath();

    void setQmlPath(const QString &qmlPath);
    QString qmlPath();

    void setFontPath(const QString &fontPath);
    QString fontPath();

    void setTranslationPath(const QString &translationPath);
    QString translationPath();

    Q_INVOKABLE QString qml(const QString &fileName, bool withProtocol = true);
    Q_INVOKABLE QString image(const QString &fileName, bool withProtocol = true);
    Q_INVOKABLE QString font(const QString &fileName, bool withProtocol = true);
    Q_INVOKABLE QString translation(const QString &fileName, bool withProtocol = true);

    void apply();

Q_SIGNALS:
    void imagePathChanged();
    void qmlPathChanged();
    void fontPathChanged();
    void translationPathChanged();

public Q_SLOTS:

private:
    QString m_imagePath;
    QString m_qmlPath;
    QString m_fontPath;
    QString m_translationPath;
};

QML_DECLARE_TYPE(CFAssetModel)

#endif // CFASSETMODEL_H

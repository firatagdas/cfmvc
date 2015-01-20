#include "cfassetmodel.h"

static inline bool checkPath(QString &path)
{
    if (path.at(0) != QLatin1Char('/')) {
        qCWarning(LCFMvc) << "Path is not absolute." << path;
        return false;
    }

    if (!path.endsWith(QLatin1Char('/')))
        path += QLatin1Char('/');

    return true;
}

static inline QString buildPath(const QString &basePath, const QString &fileName, bool withProtocol)
{
    return QString::fromLatin1("%1%2%3").arg(QLatin1String(withProtocol ? "qrc:" : ":")).arg(basePath).arg(fileName);
}

CFMVC_MODEL_BEGIN(CFAssetModel)

CFAssetModel::CFAssetModel(CFMvc *mvc, const QVariantMap &config)
    : CFModel(mvc, config)
{
}

CFAssetModel::~CFAssetModel()
{
}

void CFAssetModel::setImagePath(const QString &imagePath)
{
    QString path = imagePath;
    if (!checkPath(path))
        return;

    if (m_imagePath != path) {
        m_imagePath = path;
        emit imagePathChanged();
    }
}

QString CFAssetModel::imagePath()
{
    return m_imagePath;
}

void CFAssetModel::setQmlPath(const QString &qmlPath)
{
    QString path = qmlPath;
    if (!checkPath(path))
        return;

    if (m_qmlPath != path) {
        m_qmlPath = path;
        emit qmlPathChanged();
    }
}

QString CFAssetModel::qmlPath()
{
    return m_qmlPath;
}

void CFAssetModel::setFontPath(const QString &fontPath)
{
    QString path = fontPath;
    if (!checkPath(path))
        return;

    if (m_fontPath != path) {
        m_fontPath = path;
        emit fontPathChanged();
    }
}

QString CFAssetModel::fontPath()
{
    return m_fontPath;
}

void CFAssetModel::setTranslationPath(const QString &translationPath)
{
    QString path = translationPath;
    if (!checkPath(path))
        return;

    if (m_translationPath != path) {
        m_translationPath = path;
        emit translationPathChanged();
    }
}

QString CFAssetModel::translationPath()
{
    return m_translationPath;
}

QString CFAssetModel::image(const QString &fileName, bool withProtocol)
{
    return buildPath(m_imagePath, fileName, withProtocol);
}

QString CFAssetModel::qml(const QString &fileName, bool withProtocol)
{
    return buildPath(m_qmlPath, fileName, withProtocol);
}

QString CFAssetModel::font(const QString &fileName, bool withProtocol)
{
    return buildPath(m_fontPath, fileName, withProtocol);
}

QString CFAssetModel::translation(const QString &fileName, bool withProtocol)
{
    return buildPath(m_translationPath, fileName, withProtocol);
}

void CFAssetModel::apply()
{
    if (!m_config.isEmpty())
        return;

    if (m_config.contains(QLatin1String("imagePath")))
        setImagePath(m_config.value(QLatin1String("imagePath")).toString());

    if (m_config.contains(QLatin1String("qmlPath")))
        setQmlPath(m_config.value(QLatin1String("qmlPath")).toString());

    if (m_config.contains(QLatin1String("fontPath")))
        setFontPath(m_config.value(QLatin1String("fontPath")).toString());

    if (m_config.contains(QLatin1String("translationPath")))
        setTranslationPath(m_config.value(QLatin1String("translationPath")).toString());
}

CFMVC_MODEL_END(CFAssetModel)


#include "cfscreenmodel.h"
#include <QScreen>
#include <QGuiApplication>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif

CFMVC_MODEL_BEGIN(CFScreenModel)

CFScreenModel::CFScreenModel(CFMvc *mvc, const QVariantMap &config)
    : CFModel(mvc, config)
    , m_dpi(0.)
    , m_actualDpi(0.)
    , m_scaleFactor(1.)
    , m_actualScaleFactor(0.)
    , m_textScaleFactor(1.)
{
    connect(this, SIGNAL(registered()), this, SLOT(onRegistered()), Qt::DirectConnection);
}

CFScreenModel::~CFScreenModel()
{
}

void CFScreenModel::onRegistered()
{
    int dpi = 0;
    int actualDpi = 0;

    QScreen *screen = qApp->primaryScreen();

#if defined(Q_OS_OSX) || defined(Q_OS_WIN)
    dpi = screen->logicalDotsPerInch() * screen->devicePixelRatio();
    actualDpi = dpi;
#elif defined(Q_OS_ANDROID)
    // https://bugreports.qt-project.org/browse/QTBUG-35701
    // recalculate dpi for Android
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject resources = activity.callObjectMethod("getResources", "()Landroid/content/res/Resources;");
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();

        dpi = screen->physicalDotsPerInch() * screen->devicePixelRatio();
    } else {
        QAndroidJniObject displayMetrics = resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();

            dpi = screen->physicalDotsPerInch() * screen->devicePixelRatio();
        } else {
            dpi = displayMetrics.getField<int>("densityDpi");
        }
    }
    actualDpi = dpi;
#elif defined(Q_OS_IOS)
    dpi = screen->physicalDotsPerInch();
    actualDpi = screen->physicalDotsPerInch() * screen->devicePixelRatio();
#else
    dpi = screen->physicalDotsPerInch() * screen->devicePixelRatio();
    actualDpi = dpi;
#endif

    m_dpi = dpi;
    m_actualDpi = actualDpi;
    m_scaleFactor = (int) (m_dpi / 160);
    m_actualScaleFactor = (int) (m_actualDpi / 160);

    qCDebug(LCFMvc) << "DPI" << m_dpi
                    << "Actual DPI" << m_actualDpi
                    << "Scale" << m_scaleFactor
                    << "Actual Scale" << m_actualScaleFactor
                    << "PhysicalDotsPerInch" << screen->physicalDotsPerInch()
                    << "LogicalDotsPerInch" << screen->logicalDotsPerInch()
                    << "Device Pixel Ratio" << screen->devicePixelRatio();
}

qreal CFScreenModel::dpi()
{
    return m_dpi;
}

qreal CFScreenModel::actualDpi()
{
    return m_actualDpi;
}

qreal CFScreenModel::scaleFactor()
{
    return m_scaleFactor;
}

qreal CFScreenModel::actualScaleFactor()
{
    return m_actualScaleFactor;
}

void CFScreenModel::setTextScaleFactor(qreal textScaleFactor)
{
    if (textScaleFactor != m_textScaleFactor) {
        m_textScaleFactor = textScaleFactor;
        emit textScaleFactorChanged();
    }
}

qreal CFScreenModel::textScaleFactor()
{
    return m_textScaleFactor;
}

qreal CFScreenModel::dp(qreal px)
{
    return px * m_scaleFactor;
}

qreal CFScreenModel::sp(qreal px)
{
    return px * m_scaleFactor * m_textScaleFactor;
}

CFMVC_MODEL_END(CFScreenModel)


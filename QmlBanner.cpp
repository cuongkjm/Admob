#include "QmlBanner.h"

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
#include "Platform/Ios/QtAdmobBannerIosDelegateImpl.h"
#endif

#include <QSize>
#include <QtGlobal>

namespace {
QSize bannerSizeFor(QmlBanner::BannerSizes size)
{
    switch (size) {
    case QmlBanner::FLUID:
    case QmlBanner::BANNER:
    case QmlBanner::SEARCH:
    case QmlBanner::SMART_BANNER:
        return {320, 50};
    case QmlBanner::FULL_BANNER:
        return {468, 60};
    case QmlBanner::LARGE_BANNER:
        return {320, 100};
    case QmlBanner::LEADERBOARD:
        return {728, 90};
    case QmlBanner::MEDIUM_RECTANGLE:
        return {300, 250};
    case QmlBanner::WIDE_SKYSCRAPER:
        return {160, 600};
    }

    return {320, 50};
}
}

#ifdef Q_OS_ANDROID
#include "ActiveRegistry.h"

#include <QCoreApplication>
#include <QJniEnvironment>
#include <QMetaObject>
#include <QPointer>

namespace {
template <typename Object, typename Callback>
void dispatchToQt(jlong nativePointer, Callback callback)
{
    if (!ActiveRegistry::contains(nativePointer)) {
        return;
    }

    auto* object = reinterpret_cast<Object*>(nativePointer);
    QPointer<Object> guard(object);
    QMetaObject::invokeMethod(object, [guard, nativePointer, callback]() {
        if (!guard || !ActiveRegistry::contains(nativePointer)) {
            return;
        }
        callback(guard.data());
    }, Qt::QueuedConnection);
}
}

extern "C" {
JNIEXPORT void JNICALL Java_com_qtadmob_AdMobBanner_BannerLoaded(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt<QmlBanner>(nativePointer, [](QmlBanner* banner) { emit banner->bannerLoaded(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobBanner_BannerFailedToLoad(JNIEnv*, jobject, jlong nativePointer, jint errorCode)
{
    dispatchToQt<QmlBanner>(nativePointer, [errorCode](QmlBanner* banner) { emit banner->bannerFailedToLoad(errorCode); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobBanner_BannerOpened(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt<QmlBanner>(nativePointer, [](QmlBanner* banner) { emit banner->bannerOpened(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobBanner_BannerLeftApplication(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt<QmlBanner>(nativePointer, [](QmlBanner* banner) { emit banner->bannerLeftApplication(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobBanner_BannerClosed(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt<QmlBanner>(nativePointer, [](QmlBanner* banner) { emit banner->bannerClosed(); });
}
}
#endif

QmlBanner::QmlBanner()
{
    setSize(bannerSizeFor(m_BannerSize));

#ifdef Q_OS_ANDROID
    ActiveRegistry::registerInstance(this);

    QJniObject activity(QNativeInterface::QAndroidApplication::context());
    if (activity.isValid()) {
        m_JavaAd = QJniObject("com/qtadmob/AdMobBanner",
                              "(Landroid/app/Activity;J)V",
                              activity.object<jobject>(),
                              reinterpret_cast<jlong>(this));
        if (m_JavaAd.isValid()) {
            m_JavaAd.callMethod<void>("initializeBanner");
            syncGeometry();
            syncVisibility();
        }
    }
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    m_Admob = new QtAdmobBannerIosDelegateImpl();
    m_Admob->setQtAdmobBannerIos(this);
#endif

    connect(this, &QQuickItem::xChanged, this, &QmlBanner::syncGeometry);
    connect(this, &QQuickItem::yChanged, this, &QmlBanner::syncGeometry);
    connect(this, &QQuickItem::visibleChanged, this, &QmlBanner::syncVisibility);
}

QmlBanner::~QmlBanner()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("destroy");
    }
    ActiveRegistry::unregisterInstance(this);
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    if (m_Admob) {
        m_Admob->setQtAdmobBannerIos(nullptr);
        delete m_Admob;
        m_Admob = nullptr;
    }
#endif
}

void QmlBanner::setUnitId(const QString& unitId)
{
    m_UnitId = unitId;
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        QJniObject value = QJniObject::fromString(unitId);
        m_JavaAd.callMethod<void>("setBannerUnitId", "(Ljava/lang/String;)V", value.object<jstring>());
    }
#elif _WIN32
    Q_UNUSED(unitId)
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    if (m_Admob) {
        m_Admob->setUnitId(unitId);
    }
#endif
}

void QmlBanner::setBannerSize(BannerSizes size)
{
    m_BannerSize = size;
    setSize(bannerSizeFor(size));
    syncGeometry();
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("setBannerSize", "(I)V", static_cast<jint>(size));
    }
#elif _WIN32
    Q_UNUSED(size)
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    if (m_Admob) {
        m_Admob->setBannerSize(static_cast<QtAdmobBannerIosDelegateImpl::BannerSizes>(size));
    }
#endif
}

int QmlBanner::getAdBannerWidth()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        return m_JavaAd.callMethod<jint>("getAdBannerWidth");
    }
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    if (m_Admob) {
        return m_Admob->getAdBannerWidth();
    }
#endif

    return 0;
}

int QmlBanner::getAdBannerHeight()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        return m_JavaAd.callMethod<jint>("getAdBannerHeight");
    }
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    if (m_Admob) {
        return m_Admob->getAdBannerHeight();
    }
#endif

    return 0;
}

void QmlBanner::syncGeometry()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("setX", "(I)V", static_cast<jint>(qRound(x())));
        m_JavaAd.callMethod<void>("setY", "(I)V", static_cast<jint>(qRound(y())));
    }
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    if (m_Admob) {
        m_Admob->setX(qRound(x()));
        m_Admob->setY(qRound(y()));
    }
#endif
}

void QmlBanner::syncVisibility()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("setVisible", "(Z)V", static_cast<jboolean>(isVisible()));
    }
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    if (m_Admob) {
        m_Admob->setVisible(isVisible());
    }
#endif
}

void QmlBanner::setTestDeviceId(const QString &testDeviceId)
{
    m_TestDeviceId = testDeviceId;
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        QJniObject value = QJniObject::fromString(testDeviceId);
        m_JavaAd.callMethod<void>("setTestDeviceId", "(Ljava/lang/String;)V", value.object<jstring>());
    }
#elif _WIN32
    Q_UNUSED(testDeviceId)
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    if (m_Admob) {
        m_Admob->setTestDeviceId(testDeviceId);
    }
#endif
}

void QmlBanner::loadBanner()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("loadBanner");
        emit widthChanged();
        emit heightChanged();
    }
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    if (m_Admob) {
        m_Admob->loadBanner();
    }
#endif
}

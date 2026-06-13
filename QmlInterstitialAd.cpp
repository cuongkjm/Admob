#include "QmlInterstitialAd.h"

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
#include "Platform/Ios/QtAdmobInterstitialIosDelegateImpl.h"
#endif

#ifdef Q_OS_ANDROID
#include "ActiveRegistry.h"

#include <QCoreApplication>
#include <QJniEnvironment>
#include <QMetaObject>
#include <QPointer>

namespace {
template <typename Callback>
void dispatchToQt(jlong nativePointer, Callback callback)
{
    if (!ActiveRegistry::contains(nativePointer)) {
        return;
    }

    auto* object = reinterpret_cast<QmlInterstitialAd*>(nativePointer);
    QPointer<QmlInterstitialAd> guard(object);
    QMetaObject::invokeMethod(object, [guard, nativePointer, callback]() {
        if (!guard || !ActiveRegistry::contains(nativePointer)) {
            return;
        }
        callback(guard.data());
    }, Qt::QueuedConnection);
}
}

extern "C" {
JNIEXPORT void JNICALL Java_com_qtadmob_AdMobInterstitial_InterstitialAdLoaded(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlInterstitialAd* ad) { emit ad->interstitialAdLoaded(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobInterstitial_InterstitialAdClosed(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlInterstitialAd* ad) { emit ad->interstitialAdClosed(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobInterstitial_InterstitialAdFailedToLoad(JNIEnv*, jobject, jlong nativePointer, jint errorCode)
{
    dispatchToQt(nativePointer, [errorCode](QmlInterstitialAd* ad) { emit ad->interstitialAdFailedToLoad(errorCode); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobInterstitial_InterstitialAdOpened(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlInterstitialAd* ad) { emit ad->interstitialAdOpened(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobInterstitial_InterstitialAdLeftApplication(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlInterstitialAd* ad) { emit ad->interstitialAdLeftApplication(); });
}
}
#endif

QmlInterstitialAd::QmlInterstitialAd()
{
#ifdef Q_OS_ANDROID
    ActiveRegistry::registerInstance(this);

    QJniObject activity(QNativeInterface::QAndroidApplication::context());
    if (activity.isValid()) {
        m_JavaAd = QJniObject("com/qtadmob/AdMobInterstitial",
                              "(Landroid/app/Activity;J)V",
                              activity.object<jobject>(),
                              reinterpret_cast<jlong>(this));
    }
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    m_AdmobInterstitial = new QtAdmobInterstitialIosDelegateImpl();
    m_AdmobInterstitial->setQtAdmobInterstitialIos(this);
#endif
}

QmlInterstitialAd::~QmlInterstitialAd()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("destroy");
    }
    ActiveRegistry::unregisterInstance(this);
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (m_AdmobInterstitial) {
        m_AdmobInterstitial->setQtAdmobInterstitialIos(nullptr);
        delete m_AdmobInterstitial;
        m_AdmobInterstitial = nullptr;
    }
#endif
}

void QmlInterstitialAd::setInterstitialAdUnitId(const QString &unitId)
{
    m_UnitId = unitId;
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        QJniObject value = QJniObject::fromString(unitId);
        m_JavaAd.callMethod<void>("setInterstitialAdUnitId", "(Ljava/lang/String;)V", value.object<jstring>());
    }
#elif _WIN32
    Q_UNUSED(unitId)
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (m_AdmobInterstitial) {
        m_AdmobInterstitial->setInterstitialAdUnitId(unitId);
    }
#endif
}

void QmlInterstitialAd::setInterstitialAdTestDeviceId(const QString &testDeviceId)
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

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (m_AdmobInterstitial) {
        m_AdmobInterstitial->setInterstitialAdTestDeviceId(testDeviceId);
    }
#endif
}

void QmlInterstitialAd::loadInterstitialAd()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("loadInterstitialAd");
    }
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (m_AdmobInterstitial) {
        m_AdmobInterstitial->loadInterstitialAd();
    }
#endif
}

void QmlInterstitialAd::showInterstitialAd()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("showInterstitialAd");
    }
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (m_AdmobInterstitial) {
        m_AdmobInterstitial->showInterstitialAd();
    }
#endif
}

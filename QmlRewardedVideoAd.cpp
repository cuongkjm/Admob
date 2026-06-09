#include "QmlRewardedVideoAd.h"

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
#include "Platform/Ios/QtAdmobRewardVideoDelegateImpl.h"
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

    auto* object = reinterpret_cast<QmlRewardedVideoAd*>(nativePointer);
    QPointer<QmlRewardedVideoAd> guard(object);
    QMetaObject::invokeMethod(object, [guard, nativePointer, callback]() {
        if (!guard || !ActiveRegistry::contains(nativePointer)) {
            return;
        }
        callback(guard.data());
    }, Qt::QueuedConnection);
}
}

extern "C" {
JNIEXPORT void JNICALL Java_com_qtadmob_AdMobRewardedVideo_Rewarded(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlRewardedVideoAd* ad) { emit ad->rewarded(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobRewardedVideo_RewardedVideoAdClosed(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlRewardedVideoAd* ad) { emit ad->rewardedVideoAdClosed(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobRewardedVideo_RewardedVideoAdFailedToLoad(JNIEnv*, jobject, jlong nativePointer, jint errorCode)
{
    dispatchToQt(nativePointer, [errorCode](QmlRewardedVideoAd* ad) { emit ad->rewardedVideoAdFailedToLoad(errorCode); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobRewardedVideo_RewardedVideoAdLeftApplication(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlRewardedVideoAd* ad) { emit ad->rewardedVideoAdLeftApplication(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobRewardedVideo_RewardedVideoAdLoaded(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlRewardedVideoAd* ad) { emit ad->rewardedVideoAdLoaded(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobRewardedVideo_RewardedVideoAdOpened(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlRewardedVideoAd* ad) { emit ad->rewardedVideoAdOpened(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobRewardedVideo_RewardedVideoCompleted(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlRewardedVideoAd* ad) { emit ad->rewardedVideoCompleted(); });
}

JNIEXPORT void JNICALL Java_com_qtadmob_AdMobRewardedVideo_RewardedVideoStarted(JNIEnv*, jobject, jlong nativePointer)
{
    dispatchToQt(nativePointer, [](QmlRewardedVideoAd* ad) { emit ad->rewardedVideoStarted(); });
}
}
#endif

QmlRewardedVideoAd::QmlRewardedVideoAd()
{
#ifdef Q_OS_ANDROID
    ActiveRegistry::registerInstance(this);

    QJniObject activity(QNativeInterface::QAndroidApplication::context());
    if (activity.isValid()) {
        m_JavaAd = QJniObject("com/qtadmob/AdMobRewardedVideo",
                              "(Landroid/app/Activity;J)V",
                              activity.object<jobject>(),
                              reinterpret_cast<jlong>(this));
    }
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    m_QtAdmobRewardVideo = new QtAdmobRewardVideoDelegateImpl();
    m_QtAdmobRewardVideo->setQtAdmobRewardVideoIos(this);
#endif
}

QmlRewardedVideoAd::~QmlRewardedVideoAd()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("destroy");
    }
    ActiveRegistry::unregisterInstance(this);
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (m_QtAdmobRewardVideo) {
        m_QtAdmobRewardVideo->setQtAdmobRewardVideoIos(nullptr);
        delete m_QtAdmobRewardVideo;
        m_QtAdmobRewardVideo = nullptr;
    }
#endif
}

void QmlRewardedVideoAd::setUnitId(const QString &unitId)
{
    m_UnitId = unitId;
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        QJniObject value = QJniObject::fromString(unitId);
        m_JavaAd.callMethod<void>("setUnitId", "(Ljava/lang/String;)V", value.object<jstring>());
    }
#elif _WIN32
    Q_UNUSED(unitId)
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (m_QtAdmobRewardVideo) {
        m_QtAdmobRewardVideo->setUnitId(unitId);
    }
#endif
}

void QmlRewardedVideoAd::setTestDeviceId(const QString &testDeviceId)
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
    if (m_QtAdmobRewardVideo) {
        m_QtAdmobRewardVideo->setTestDeviceId(testDeviceId);
    }
#endif
}

void QmlRewardedVideoAd::loadRewardedVideoAd()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("loadRewardedVideoAd");
    }
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (m_QtAdmobRewardVideo) {
        m_QtAdmobRewardVideo->loadRewardedVideoAd();
    }
#endif
}

void QmlRewardedVideoAd::show()
{
#ifdef Q_OS_ANDROID
    if (m_JavaAd.isValid()) {
        m_JavaAd.callMethod<void>("show");
    }
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (m_QtAdmobRewardVideo) {
        m_QtAdmobRewardVideo->show();
    }
#endif
}

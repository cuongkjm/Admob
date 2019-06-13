#include "QmlInterstitialAd.h"
#include "QtAdmobInterstitialIosDelegateImpl.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef Q_OS_ANDROID
// Listener when Java calls onBannerLoaded signal
JNIEXPORT void JNICALL Java_com_gmail_manhcuong5993_QtAdMobActivity_InterstitialAdLoaded(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    // Emit to QML app by calling bannerLoaded signal
    emit QmlInterstitialAd::Instances()->interstitialAdLoaded();
}

JNIEXPORT void JNICALL Java_com_gmail_manhcuong5993_QtAdMobActivity_InterstitialAdClosed(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    // Emit to QML app by calling bannerLoaded signal
    emit QmlInterstitialAd::Instances()->interstitialAdClosed();
}

JNIEXPORT void JNICALL Java_com_gmail_manhcuong5993_QtAdMobActivity_InterstitialAdFailedToLoad(JNIEnv *env, jobject thiz, jint errorCode)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    // Emit to QML app by calling bannerLoaded signal
    emit QmlInterstitialAd::Instances()->interstitialAdFailedToLoad(errorCode);
}

JNIEXPORT void JNICALL Java_com_gmail_manhcuong5993_QtAdMobActivity_InterstitialAdOpened(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    // Emit to QML app by calling bannerLoaded signal
    emit QmlInterstitialAd::Instances()->interstitialAdOpened();
}

JNIEXPORT void JNICALL Java_com_gmail_manhcuong5993_QtAdMobActivity_InterstitialAdLeftApplication(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    // Emit to QML app by calling bannerLoaded signal
    emit QmlInterstitialAd::Instances()->interstitialAdLeftApplication();
}
#endif

#ifdef __cplusplus
}
#endif

static QmlInterstitialAd *mQmlInterstitialAd = nullptr;

QmlInterstitialAd::QmlInterstitialAd()
{
#ifdef Q_OS_ANDROID
    // Update global instance
    mQmlInterstitialAd = this;

    // Create Android Activity on Qt
    QPlatformNativeInterface* interface = QGuiApplication::platformNativeInterface();
    jobject activity = (jobject)interface->nativeResourceForIntegration("QtActivity");
    if (activity)
    {
        m_Activity = new QAndroidJniObject(activity);
    }

    // Call InitializeBanner method of Java
    m_Activity->callMethod<void>("InitializeInterstitialAd");
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    m_AdmobInterstitial = new QtAdmobInterstitialIosDelegateImpl();
    m_AdmobInterstitial->setQtAdmobInterstitialIos(this);
#endif
}

QmlInterstitialAd *QmlInterstitialAd::Instances()
{
    return mQmlInterstitialAd;
}

void QmlInterstitialAd::setInterstitialAdUnitId(const QString &unitId)
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        QAndroidJniObject param1 = QAndroidJniObject::fromString(unitId);
        // Call SetBannerUnitId method of Java
        m_Activity->callMethod<void>("SetInterstitialAdUnitId", "(Ljava/lang/String;)V", param1.object<jstring>());
    }
#elif _WIN32
    Q_UNUSED(unitId)
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
   m_AdmobInterstitial->setInterstitialAdUnitId(unitId);
#endif
}

void QmlInterstitialAd::setInterstitialAdTestDeviceId(const QString &testDeviceId)
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        QAndroidJniObject param1 = QAndroidJniObject::fromString(testDeviceId);
        // Call SetBannerUnitId method of Java
        m_Activity->callMethod<void>("SetInterstitialAdTestDeviceId", "(Ljava/lang/String;)V", param1.object<jstring>());
    }
#elif _WIN32
    Q_UNUSED(testDeviceId)
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
   m_AdmobInterstitial->setInterstitialAdTestDeviceId(testDeviceId);
#endif
}

void QmlInterstitialAd::loadInterstitialAd()
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        // Call LoadBanner method of Java
        m_Activity->callMethod<void>("LoadInterstitialAd");
    }
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    m_AdmobInterstitial->loadInterstitialAd();
#endif
}

void QmlInterstitialAd::showInterstitialAd()
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        // Call ShowInterstitialAd method of Java
        m_Activity->callMethod<void>("ShowInterstitialAd");
    }
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
   m_AdmobInterstitial->showInterstitialAd();
#endif
}

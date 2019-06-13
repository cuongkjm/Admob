#include "QmlBanner.h"
#include "QtAdmobBannerIosDelegateImpl.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef Q_OS_ANDROID
// Listener when Java calls onBannerLoaded signal
JNIEXPORT void JNICALL Java_com_gmail_manhcuong5993_QtAdMobActivity_BannerLoaded(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    // Emit to QML app by calling bannerLoaded signal
    emit QmlBanner::Instances()->bannerLoaded();
}

JNIEXPORT void JNICALL Java_com_gmail_manhcuong5993_QtAdMobActivity_BannerFailedToLoad(JNIEnv *env, jobject thiz, jint errorCode)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    // Emit to QML app by calling bannerLoaded signal
    emit QmlBanner::Instances()->bannerFailedToLoad(errorCode);
}

JNIEXPORT void JNICALL Java_com_gmail_manhcuong5993_QtAdMobActivity_BannerOpened(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    // Emit to QML app by calling bannerLoaded signal
    emit QmlBanner::Instances()->bannerOpened();
}

JNIEXPORT void JNICALL Java_com_gmail_manhcuong5993_QtAdMobActivity_BannerLeftApplication(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    // Emit to QML app by calling bannerLoaded signal
    emit QmlBanner::Instances()->bannerLeftApplication();
}

JNIEXPORT void JNICALL Java_com_gmail_manhcuong5993_QtAdMobActivity_BannerClosed(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    // Emit to QML app by calling bannerLoaded signal
    emit QmlBanner::Instances()->bannerClosed();
}
#endif

#ifdef __cplusplus
}
#endif

// Global variable to keep instance of class
static QmlBanner *mQMLBanner = nullptr;

// Implement Instances() method
QmlBanner* QmlBanner::Instances()
{
    return mQMLBanner;
}

// Implement initialize method
QmlBanner::QmlBanner()
{
#ifdef Q_OS_ANDROID
    // Update global instance
    mQMLBanner = this;

    // Create Android Activity on Qt
    QPlatformNativeInterface* interface = QGuiApplication::platformNativeInterface();
    jobject activity = (jobject)interface->nativeResourceForIntegration("QtActivity");
    if (activity)
    {
        m_Activity = new QAndroidJniObject(activity);
    }

    // Call InitializeBanner method of Java
    m_Activity->callMethod<void>("InitializeBanner");
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    m_Admob = new QtAdmobBannerIosDelegateImpl();
    m_Admob->setQtAdmobBannerIos(this);
#endif
}

void QmlBanner::setUnitId(const QString& unitId)
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        QAndroidJniObject param1 = QAndroidJniObject::fromString(unitId);
        // Call SetBannerUnitId method of Java
        m_Activity->callMethod<void>("SetBannerUnitId", "(Ljava/lang/String;)V", param1.object<jstring>());
    }
#elif _WIN32
    Q_UNUSED(unitId)
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    m_Admob->setUnitId(unitId);
#endif
}

void QmlBanner::setBannerSize(BannerSizes size)
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        // Call SetBannerSize method of Java
        m_Activity->callMethod<void>("SetBannerSize", "(I)V", (int)size);
    }
#elif _WIN32
    Q_UNUSED(size)
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    m_Admob->setBannerSize((QtAdmobBannerIosDelegateImpl::BannerSizes) size);
#endif
}

int QmlBanner::getAdBannerWidth()
{
    int width = 0;
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        // Call GetAdBannerWidth method of Java
        width = m_Activity->callMethod<jint>("GetAdBannerWidth");
    }
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    width = m_Admob->getAdBannerWidth();
#endif
    return width;
}

int QmlBanner::getAdBannerHeight()
{
    int height = 0;
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        // Call GetAdBannerWidth method of Java
        height = m_Activity->callMethod<jint>("GetAdBannerHeight");
    }
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    height = m_Admob->getAdBannerHeight();
#endif

    return height;
}

void QmlBanner::setX(const int &x)
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        // Call SetX method of Java
        m_Activity->callMethod<void>("SetX", "(I)V", x);
    }
#elif _WIN32
    Q_UNUSED(x)
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    m_Admob->setX(x);
#endif
}

void QmlBanner::setY(const int &y)
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        // Call SetY method of Java
        m_Activity->callMethod<void>("SetY", "(I)V", y);
    }
#elif _WIN32
    Q_UNUSED(y)
#endif
#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    m_Admob->setY(y);
#endif
}

void QmlBanner::setVisible(const bool &visible)
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        // Call SetVisible method of Java
        m_Activity->callMethod<void>("SetVisible", "(Z)V", visible);
    }
#elif _WIN32
    Q_UNUSED(visible)
#endif
#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    m_Admob->setVisible(visible);
#endif
}

void QmlBanner::setTestDeviceId(const QString &testDeviceId)
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        QAndroidJniObject param1 = QAndroidJniObject::fromString(testDeviceId);
        // Call SetBannerUnitId method of Java
        m_Activity->callMethod<void>("SetBannerTestDeviceId", "(Ljava/lang/String;)V", param1.object<jstring>());
    }
#elif _WIN32
    Q_UNUSED(testDeviceId)
#endif
#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    m_Admob->setTestDeviceId(testDeviceId);
#endif
}

void QmlBanner::loadBanner()
{
#ifdef Q_OS_ANDROID
    if(m_Activity != nullptr)
    {
        // Call LoadBanner method of Java
        m_Activity->callMethod<void>("LoadBanner");
        emit widthChanged();
        emit heightChanged();
    }
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    m_Admob->loadBanner();
#endif
}

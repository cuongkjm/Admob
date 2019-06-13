#ifndef QMLINTERSTITIALAD_H
#define QMLINTERSTITIALAD_H

#include <QObject>
#include <QGuiApplication>
#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <qpa/qplatformnativeinterface.h>

class QAndroidJniObject;
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
class QtAdmobInterstitialIosDelegateImpl;
#endif

class QmlInterstitialAd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString unitId WRITE setInterstitialAdUnitId)
    Q_PROPERTY(QString testDeviceId WRITE setInterstitialAdTestDeviceId)
public:
    QmlInterstitialAd();

    // Add static method to get instance of class
    static QmlInterstitialAd* Instances();

    // Writing method for unitId
    void setInterstitialAdUnitId(const QString& unitId);
    void setInterstitialAdTestDeviceId(const QString &testDeviceId);
signals:
    void interstitialAdLoaded();
    void interstitialAdClosed();
    void interstitialAdFailedToLoad(int errorCode);
    void interstitialAdOpened();
    void interstitialAdLeftApplication();

public slots:
    // loadInterstitialAd method for load InterstitialAd
    void loadInterstitialAd();

    // Show InterstitialAd
    void showInterstitialAd();

private:
#ifdef Q_OS_ANDROID
    QAndroidJniObject* m_Activity;
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    QtAdmobInterstitialIosDelegateImpl* m_AdmobInterstitial;
#endif
};

#endif // QMLINTERSTITIALAD_H

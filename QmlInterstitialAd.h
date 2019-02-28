#ifndef QMLINTERSTITIALAD_H
#define QMLINTERSTITIALAD_H

#include <QObject>
#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <qpa/qplatformnativeinterface.h>
#endif
#include <QGuiApplication>

class QAndroidJniObject;

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
};

#endif // QMLINTERSTITIALAD_H

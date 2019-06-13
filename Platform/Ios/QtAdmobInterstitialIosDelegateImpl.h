#ifndef QTADMOBINTERSTITIALIOSDELEGATEIMPL_H
#define QTADMOBINTERSTITIALIOSDELEGATEIMPL_H

#include <QString>

class QmlInterstitialAd;

class QtAdmobInterstitialIosDelegateImpl
{
public:
    QtAdmobInterstitialIosDelegateImpl();
    ~QtAdmobInterstitialIosDelegateImpl();

    void setQtAdmobInterstitialIos(QmlInterstitialAd* qtAdmobIntersitialIos);
    void setInterstitialAdUnitId(const QString& unitId);
    void setInterstitialAdTestDeviceId(const QString& testDeviceId);

    void loadInterstitialAd();
    void showInterstitialAd();
    
    //signals
    void interstitialAdLoaded();
    void interstitialAdClosed();
    void interstitialAdFailedToLoad(int errorCode);
    void interstitialAdOpened();
    void interstitialAdLeftApplication();

private:
    void* self;
    QmlInterstitialAd* m_QtAdmobInterstitialIos;
};

#endif // QTADMOBINTERSTITIALIOSDELEGATEIMPL_H

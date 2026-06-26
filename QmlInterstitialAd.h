#ifndef QMLINTERSTITIALAD_H
#define QMLINTERSTITIALAD_H

#include <QObject>

#ifdef Q_OS_ANDROID
#include <QJniObject>
#endif

#if defined(Q_OS_IOS)
class QtAdmobInterstitialIosDelegateImpl;
#endif

class QmlInterstitialAd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString unitId MEMBER m_UnitId WRITE setInterstitialAdUnitId)
    Q_PROPERTY(QString testDeviceId MEMBER m_TestDeviceId WRITE setInterstitialAdTestDeviceId)

public:
    QmlInterstitialAd();
    ~QmlInterstitialAd() override;

    void setInterstitialAdUnitId(const QString& unitId);
    void setInterstitialAdTestDeviceId(const QString &testDeviceId);

signals:
    void interstitialAdLoaded();
    void interstitialAdClosed();
    void interstitialAdFailedToLoad(int errorCode);
    void interstitialAdOpened();
    void interstitialAdLeftApplication();

public slots:
    void loadInterstitialAd();
    void showInterstitialAd();

private:
    QString m_UnitId;
    QString m_TestDeviceId;

#ifdef Q_OS_ANDROID
    QJniObject m_JavaAd;
#endif

#if defined(Q_OS_IOS)
    QtAdmobInterstitialIosDelegateImpl* m_AdmobInterstitial = nullptr;
#endif
};

#endif // QMLINTERSTITIALAD_H

#ifndef QMLREWARDEDVIDEOAD_H
#define QMLREWARDEDVIDEOAD_H

#include <QObject>

#ifdef Q_OS_ANDROID
#include <QJniObject>
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
class QtAdmobRewardVideoDelegateImpl;
#endif

class QmlRewardedVideoAd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString unitId MEMBER m_UnitId WRITE setUnitId)
    Q_PROPERTY(QString testDeviceId MEMBER m_TestDeviceId WRITE setTestDeviceId)

public:
    QmlRewardedVideoAd();
    ~QmlRewardedVideoAd() override;

    void setUnitId(const QString& unitId);
    void setTestDeviceId(const QString &testDeviceId);

signals:
    void rewarded();
    void rewardedVideoAdClosed();
    void rewardedVideoAdFailedToLoad(int errorCode);
    void rewardedVideoAdLeftApplication();
    void rewardedVideoAdLoaded();
    void rewardedVideoAdOpened();
    void rewardedVideoCompleted();
    void rewardedVideoStarted();

public slots:
    void loadRewardedVideoAd();
    void show();

private:
    QString m_UnitId;
    QString m_TestDeviceId;

#ifdef Q_OS_ANDROID
    QJniObject m_JavaAd;
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    QtAdmobRewardVideoDelegateImpl* m_QtAdmobRewardVideo = nullptr;
#endif
};

#endif // QMLREWARDEDVIDEOAD_H

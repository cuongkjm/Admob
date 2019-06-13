#ifndef QTADMOBREWARDVIDEODELEGATEIMPL_H
#define QTADMOBREWARDVIDEODELEGATEIMPL_H

#include <QString>

class QmlRewardedVideoAd;

class QtAdmobRewardVideoDelegateImpl
{
public:
    QtAdmobRewardVideoDelegateImpl();
    ~QtAdmobRewardVideoDelegateImpl();

    void setQtAdmobRewardVideoIos(QmlRewardedVideoAd *qtAdmobRewardVideoIos);

    void setUnitId(const QString& unitId);
    void setTestDeviceId(const QString &testDeviceId);

    //public slots:
    void loadRewardedVideoAd();
    void show();

    //signals:
    void rewarded();
    void rewardedVideoAdClosed();
    void rewardedVideoAdFailedToLoad(int errorCode);
    void rewardedVideoAdLeftApplication();
    void rewardedVideoAdLoaded();
    void rewardedVideoAdOpened();
    void rewardedVideoCompleted();
    void rewardedVideoStarted();

private:
    void* self;
    QmlRewardedVideoAd* m_qtAdmobRewardVideoIos;
};

#endif // QTADMOBREWARDVIDEODELEGATEIMPL_H

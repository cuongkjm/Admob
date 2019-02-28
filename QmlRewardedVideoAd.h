#ifndef QMLREWARDEDVIDEOAD_H
#define QMLREWARDEDVIDEOAD_H

#include <QObject>
#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QtAndroid>
#include <qpa/qplatformnativeinterface.h>
#endif
#include <QGuiApplication>

class QmlRewardedVideoAd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString unitId WRITE setUnitId)
    Q_PROPERTY(QString testDeviceId WRITE setTestDeviceId)
public:
    QmlRewardedVideoAd();
    static QmlRewardedVideoAd* Instances();
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
#ifdef Q_OS_ANDROID
    QAndroidJniObject* m_Activity;
#endif
};

#endif // QMLREWARDEDVIDEOAD_H

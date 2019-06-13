#ifndef QMLBANNER_H
#define QMLBANNER_H

#include <QObject>
#include <QQuickItem>
#include <QGuiApplication>
#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <qpa/qplatformnativeinterface.h>

class QAndroidJniObject;
#endif
#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
class QtAdmobBannerIosDelegateImpl;
#endif

class QmlBanner : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString unitId WRITE setUnitId)
    Q_PROPERTY(BannerSizes bannerSize WRITE setBannerSize)
    Q_PROPERTY(int width READ getAdBannerWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ getAdBannerHeight NOTIFY heightChanged)
    Q_PROPERTY(int x WRITE setX)
    Q_PROPERTY(int y WRITE setY)
    Q_PROPERTY(bool visible WRITE setVisible)
    Q_PROPERTY(QString testDeviceId WRITE setTestDeviceId)

public:
    // Banner sizes
    enum BannerSizes
    {
        BANNER = 0,
        FLUID,
        FULL_BANNER,
        LARGE_BANNER,
        LEADERBOARD,
        MEDIUM_RECTANGLE,
        SEARCH,
        SMART_BANNER,
        WIDE_SKYSCRAPER
    };
    Q_ENUM(BannerSizes)

    QmlBanner();

    // Add static method to get instance of class
    static QmlBanner* Instances();

    // Writing method for unitId
    void setUnitId(const QString& unitId);

    // Setting method for size of banner
    void setBannerSize(BannerSizes size);

    int getAdBannerWidth();
    int getAdBannerHeight();
    void setX(const int& x);
    void setY(const int& y);
    void setVisible(const bool &visible);
    void setTestDeviceId(const QString &testDeviceId);

public slots:
    // LoadBanner method for loading banner
    void loadBanner();

signals:
    // Emit to QML app when an ad finishes loading
    void bannerLoaded();
    void bannerFailedToLoad(int errorCode);
    void bannerOpened();
    void bannerLeftApplication();
    void bannerClosed();

    // Emit to QML app when load Banner (call loadBanner() method)
    void widthChanged();
    void heightChanged();

private:
#ifdef Q_OS_ANDROID
    QAndroidJniObject* m_Activity;
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    QtAdmobBannerIosDelegateImpl *m_Admob;
#endif
};

#endif // QMLBANNER_H

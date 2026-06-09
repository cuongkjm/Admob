#ifndef QMLBANNER_H
#define QMLBANNER_H

#include <QQuickItem>

#ifdef Q_OS_ANDROID
#include <QJniObject>
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
class QtAdmobBannerIosDelegateImpl;
#endif

class QmlBanner : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString unitId MEMBER m_UnitId WRITE setUnitId)
    Q_PROPERTY(BannerSizes bannerSize MEMBER m_BannerSize WRITE setBannerSize)
    Q_PROPERTY(int width READ getAdBannerWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ getAdBannerHeight NOTIFY heightChanged)
    Q_PROPERTY(int x MEMBER m_X WRITE setX)
    Q_PROPERTY(int y MEMBER m_Y WRITE setY)
    Q_PROPERTY(bool visible MEMBER m_Visible WRITE setVisible)
    Q_PROPERTY(QString testDeviceId MEMBER m_TestDeviceId WRITE setTestDeviceId)

public:
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
    ~QmlBanner() override;

    void setUnitId(const QString& unitId);
    void setBannerSize(BannerSizes size);

    int getAdBannerWidth();
    int getAdBannerHeight();
    void setX(const int& x);
    void setY(const int& y);
    void setVisible(const bool &visible);
    void setTestDeviceId(const QString &testDeviceId);

public slots:
    void loadBanner();

signals:
    void bannerLoaded();
    void bannerFailedToLoad(int errorCode);
    void bannerOpened();
    void bannerLeftApplication();
    void bannerClosed();
    void widthChanged();
    void heightChanged();

private:
    QString m_UnitId;
    BannerSizes m_BannerSize = BANNER;
    int m_X = 0;
    int m_Y = 0;
    bool m_Visible = true;
    QString m_TestDeviceId;

#ifdef Q_OS_ANDROID
    QJniObject m_JavaAd;
#endif

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    QtAdmobBannerIosDelegateImpl *m_Admob = nullptr;
#endif
};

#endif // QMLBANNER_H

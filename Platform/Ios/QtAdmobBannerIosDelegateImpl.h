#ifndef QTADMOBBANNERIOSIMPL_H
#define QTADMOBBANNERIOSIMPL_H

#include <QString>

class QmlBanner;

class QtAdmobBannerIosDelegateImpl
{
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
    QtAdmobBannerIosDelegateImpl(void);
    ~QtAdmobBannerIosDelegateImpl(void);
    void setQtAdmobBannerIos(QmlBanner *QtAdmobBannerIos);
    
    void setX(const int &x);
    void setY(const int &y);
    void setUnitId(const QString &unitId);
    void loadBanner();
    void setBannerSize(BannerSizes size);
    int getAdBannerWidth();
    int getAdBannerHeight();
    void setVisible(const bool &visible);
    void setTestDeviceId(const QString &deviceId);

    //functions for emit signals
    void bannerLoaded();
    void bannerFailedToLoad(int errorCode);
    void bannerOpened();
    void bannerLeftApplication();
    void bannerClosed();

private:
    void* self;
    int m_X;
    int m_Y;
    QmlBanner* m_QtAdmobBannerIos;
};

#endif // QTADMOBBANNERIOSIMPL_H

#import "QtAdmobBannerIosDelegate.h"
#include "QmlBanner.h"

namespace {
UIViewController* rootViewController()
{
    UIApplication *application = [UIApplication sharedApplication];
    NSArray *windows = [application windows];
    UIViewController * __block controller = nil;
    [windows enumerateObjectsUsingBlock:^(UIWindow * _Nonnull window, NSUInteger, BOOL * _Nonnull stop) {
        controller = [window rootViewController];
        *stop = (controller != nil);
    }];
    return controller;
}
}

@implementation QtAdmobBannerIosDelegate

-(id) init: (QtAdmobBannerIosDelegateImpl*) handler
{
    self = [super init];
    if (self)
    {
        _handler = handler;
        _bannerView = [[GADBannerView alloc] initWithAdSize:GADAdSizeLargeBanner];
        _bannerView.delegate = self;
        _bannerView.rootViewController = rootViewController();
        [_bannerView.rootViewController.view addSubview:_bannerView];
        _request = [GADRequest request];
    }
    return self;
}

-(void) dealloc
{
    _bannerView.delegate = nil;
    [_bannerView removeFromSuperview];
    _handler = nil;
    _request = nil;
    _bannerView = nil;
    [super dealloc];
}

-(void) setPosition: (const int &) x : (const int &) y
{
    CGFloat yOffset = [UIApplication sharedApplication].statusBarFrame.size.height;
    CGRect frame = _bannerView.frame;
    frame.origin = CGPointMake(x, y + yOffset);
    _bannerView.frame = frame;
}

-(void) setUnitId:(const QString &)unitId
{
    _bannerView.adUnitID = [NSString stringWithUTF8String:unitId.toUtf8().data()];
}

-(void) loadBanner
{
    [_bannerView loadRequest:_request];
}

-(void) setBannerSize:(QtAdmobBannerIosDelegateImpl::BannerSizes)size
{
    GADAdSize newSize = GADAdSizeBanner;
    switch (size) {
    case QtAdmobBannerIosDelegateImpl::BANNER:
        newSize = GADAdSizeBanner;
        break;
    case QtAdmobBannerIosDelegateImpl::FLUID:
        newSize = GADAdSizeFluid;
        break;
    case QtAdmobBannerIosDelegateImpl::FULL_BANNER:
        newSize = GADAdSizeFullBanner;
        break;
    case QtAdmobBannerIosDelegateImpl::LARGE_BANNER:
        newSize = GADAdSizeLargeBanner;
        break;
    case QtAdmobBannerIosDelegateImpl::LEADERBOARD:
        newSize = GADAdSizeLeaderboard;
        break;
    case QtAdmobBannerIosDelegateImpl::MEDIUM_RECTANGLE:
        newSize = GADAdSizeMediumRectangle;
        break;
    case QtAdmobBannerIosDelegateImpl::SMART_BANNER:
        newSize = GADLargeAnchoredAdaptiveBannerAdSizeWithWidth(_bannerView.rootViewController.view.frame.size.width);
        break;
    case QtAdmobBannerIosDelegateImpl::WIDE_SKYSCRAPER:
        newSize = GADAdSizeSkyscraper;
        break;
    default:
        break;
    }

    _bannerView.adSize = newSize;
}

- (int) getAdBannerWidth
{
    return _bannerView.adSize.size.width;
}

- (int) getAdBannerHeight
{
    return _bannerView.adSize.size.height;
}

- (void) setVisible:(const bool &)visible
{
    _bannerView.hidden = !visible;
}

- (void) setTestDeviceId:(const QString &) deviceId
{
    NSString *identifier = [NSString stringWithUTF8String:deviceId.toUtf8().data()];
    [GADMobileAds sharedInstance].requestConfiguration.testDeviceIdentifiers = @[identifier];
}

- (void)bannerViewDidReceiveAd:(nonnull GADBannerView *)bannerView
{
    Q_UNUSED(bannerView);
    _handler->bannerLoaded();
}

- (void)bannerView:(nonnull GADBannerView *)bannerView didFailToReceiveAdWithError:(nonnull NSError *)error
{
    Q_UNUSED(bannerView);
    _handler->bannerFailedToLoad(static_cast<int>(error.code));
}

- (void)bannerViewDidDismissScreen:(nonnull GADBannerView *)bannerView
{
    Q_UNUSED(bannerView);
    _handler->bannerClosed();
}

- (void)bannerViewWillDismissScreen:(nonnull GADBannerView *)bannerView
{
    Q_UNUSED(bannerView);
    _handler->bannerClosed();
}

- (void)bannerViewWillPresentScreen:(nonnull GADBannerView *)bannerView
{
    Q_UNUSED(bannerView);
    _handler->bannerOpened();
}

QtAdmobBannerIosDelegateImpl::QtAdmobBannerIosDelegateImpl(void)
{
    self = [[QtAdmobBannerIosDelegate alloc] init:this];
    m_X = 0;
    m_Y = 0;
}

QtAdmobBannerIosDelegateImpl::~QtAdmobBannerIosDelegateImpl(void)
{
    [(__bridge id)self dealloc];
}

void QtAdmobBannerIosDelegateImpl::setX(const int &x)
{
    m_X = x;
    [(id)self setPosition:m_X :m_Y];
}

void QtAdmobBannerIosDelegateImpl::setY(const int &y)
{
    m_Y = y;
    [(id)self setPosition:m_X :m_Y];
}

void QtAdmobBannerIosDelegateImpl::setUnitId(const QString &unitId)
{
    [(id)self setUnitId:unitId];
}

void QtAdmobBannerIosDelegateImpl::loadBanner()
{
    [(id) self loadBanner];
}

void QtAdmobBannerIosDelegateImpl::setBannerSize(BannerSizes size)
{
    [(id) self setBannerSize:size];
}

int QtAdmobBannerIosDelegateImpl::getAdBannerWidth()
{
    return [(id) self getAdBannerWidth];
}

int QtAdmobBannerIosDelegateImpl::getAdBannerHeight()
{
    return [(id) self getAdBannerHeight];
}

void QtAdmobBannerIosDelegateImpl::setTestDeviceId(const QString &deviceId)
{
    [(id) self setTestDeviceId:deviceId];
}

void QtAdmobBannerIosDelegateImpl::setVisible(const bool &visible)
{
    [(id) self setVisible:visible];
}

void QtAdmobBannerIosDelegateImpl::bannerLoaded()
{
    if (m_QtAdmobBannerIos) m_QtAdmobBannerIos->bannerLoaded();
}

void QtAdmobBannerIosDelegateImpl::bannerFailedToLoad(int errorCode)
{
    if (m_QtAdmobBannerIos) m_QtAdmobBannerIos->bannerFailedToLoad(errorCode);
}

void QtAdmobBannerIosDelegateImpl::bannerOpened()
{
    if (m_QtAdmobBannerIos) m_QtAdmobBannerIos->bannerOpened();
}

void QtAdmobBannerIosDelegateImpl::bannerLeftApplication()
{
    if (m_QtAdmobBannerIos) m_QtAdmobBannerIos->bannerLeftApplication();
}

void QtAdmobBannerIosDelegateImpl::bannerClosed()
{
    if (m_QtAdmobBannerIos) m_QtAdmobBannerIos->bannerClosed();
}

void QtAdmobBannerIosDelegateImpl::setQtAdmobBannerIos(QmlBanner *QtAdmobBannerIos)
{
    m_QtAdmobBannerIos = QtAdmobBannerIos;
}

@end

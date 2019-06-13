#import "QtAdmobBannerIosDelegate.h"
#include "QmlBanner.h"

@implementation QtAdmobBannerIosDelegate

-(id) init: (QtAdmobBannerIosDelegateImpl*) handler
{
    self = [super init];
    if (self)
    {
        _handler = handler;
        _testDevices = [[NSMutableArray alloc] initWithArray:@[kGADSimulatorID]];
        _bannerView = [[GADBannerView alloc] initWithAdSize:kGADAdSizeLargeBanner];
        _bannerView.delegate = self;
        
        UIApplication *application = [UIApplication sharedApplication];
        NSArray *windows = [application windows];
        UIViewController * __block rootViewController = nil;
        [windows enumerateObjectsUsingBlock:^(UIWindow * _Nonnull window, NSUInteger, BOOL * _Nonnull stop) {
            rootViewController = [window rootViewController];
            *stop = (rootViewController != nil);
        }];
        UIView *view = rootViewController.view;
        
        _bannerView.rootViewController = rootViewController;
        _bannerView.autoloadEnabled = YES;
        [view addSubview:_bannerView];
        
        _request = [GADRequest request];
        _request.testDevices = _testDevices;
    }
    return self;
}

-(void) dealloc
{
    [super dealloc];
    _handler = nil;
    _request = nil;
    _bannerView = nil;
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
    GADAdSize newSize = kGADAdSizeBanner;
    switch (size) {
    case QtAdmobBannerIosDelegateImpl::BANNER:
        newSize = kGADAdSizeBanner;
        break;
    case QtAdmobBannerIosDelegateImpl::FLUID:
        newSize = kGADAdSizeFluid;
        break;
    case QtAdmobBannerIosDelegateImpl::FULL_BANNER:
        newSize = kGADAdSizeFullBanner;
        break;
    case QtAdmobBannerIosDelegateImpl::LARGE_BANNER:
        newSize = kGADAdSizeLargeBanner;
        break;
    case QtAdmobBannerIosDelegateImpl::LEADERBOARD:
        newSize = kGADAdSizeLeaderboard;
        break;
    case QtAdmobBannerIosDelegateImpl::MEDIUM_RECTANGLE:
        newSize = kGADAdSizeMediumRectangle;
        break;
    case QtAdmobBannerIosDelegateImpl::SMART_BANNER:
        newSize = kGADAdSizeSmartBannerPortrait;
        break;
    case QtAdmobBannerIosDelegateImpl::WIDE_SKYSCRAPER:
        newSize = kGADAdSizeSkyscraper;
        break;

    default:
        break;
    }
    
    _bannerView.adSize = newSize;
}

- (int) getAdBannerWidth
{
    CGSize size = _bannerView.adSize.size;
    return size.width;
}

- (int) getAdBannerHeight
{
    CGSize size = _bannerView.adSize.size;
    return size.height;
}

- (void) setVisible:(const bool &)visible
{
    _bannerView.hidden = !visible;
}

- (void) setTestDeviceId:(const QString &) deviceId
{
    [_request.testDevices arrayByAddingObject:[NSString stringWithUTF8String:deviceId.toUtf8().data()]];
}

- (void)adViewDidReceiveAd:(nonnull GADBannerView *)view {
    Q_UNUSED(view);
    _handler->bannerLoaded();
}

- (void)adView:(nonnull GADBannerView *)view didFailToReceiveAdWithError:(nonnull GADRequestError *)error {
    Q_UNUSED(view);
    _handler->bannerFailedToLoad(static_cast<int>(error.code));
}

- (void)adViewWillLeaveApplication:(nonnull GADBannerView *)view {
    Q_UNUSED(view);
    _handler->bannerLeftApplication();
}

- (void)adViewDidDismissScreen:(nonnull GADBannerView *)view {
    Q_UNUSED(view);
    _handler->bannerClosed();
}

- (void)adViewWillDismissScreen:(nonnull GADBannerView *)view {
    Q_UNUSED(view);
    _handler->bannerClosed();
}

- (void)adViewWillPresentScreen:(nonnull GADBannerView *)view {
    Q_UNUSED(view);
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
    int width = [(id) self getAdBannerWidth];
    return width;
}

int QtAdmobBannerIosDelegateImpl::getAdBannerHeight()
{
    int height = [(id) self getAdBannerHeight];
    return height;
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
    m_QtAdmobBannerIos->bannerLoaded();
}

void QtAdmobBannerIosDelegateImpl::bannerFailedToLoad(int errorCode)
{
    m_QtAdmobBannerIos->bannerFailedToLoad(errorCode);
}

void QtAdmobBannerIosDelegateImpl::bannerOpened()
{
    m_QtAdmobBannerIos->bannerOpened();
}

void QtAdmobBannerIosDelegateImpl::bannerLeftApplication()
{
    m_QtAdmobBannerIos->bannerLeftApplication();
}

void QtAdmobBannerIosDelegateImpl::bannerClosed()
{
    m_QtAdmobBannerIos->bannerClosed();
}

void QtAdmobBannerIosDelegateImpl::setQtAdmobBannerIos(QmlBanner *QtAdmobBannerIos)
{
    m_QtAdmobBannerIos = QtAdmobBannerIos;
}


@end

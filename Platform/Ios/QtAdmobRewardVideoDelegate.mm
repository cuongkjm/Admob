#include "QtAdmobRewardVideoDelegate.h"
#include "QtAdmobRewardVideoDelegateImpl.h"
#include "QmlRewardedVideoAd.h"

namespace {
UIViewController* rootViewController()
{
    UIApplication *application = [UIApplication sharedApplication];
    UIWindow *keyWindow = application.keyWindow;
    if (!keyWindow) {
        for (UIWindow *window in application.windows) {
            if (!window.hidden) {
                keyWindow = window;
                break;
            }
        }
    }

    UIViewController *controller = keyWindow.rootViewController;
    while (controller.presentedViewController) {
        controller = controller.presentedViewController;
    }
    return controller;
}
}

@implementation QtAdmobRewardVideoDelegate

- (id)init:(QtAdmobRewardVideoDelegateImpl *)handler {
    self = [super init];
    if (self)
    {
        _handler = handler;
        self.request = [GADRequest request];
    }
    return self;
}

- (void)dealloc {
    self.rewardedAd.fullScreenContentDelegate = nil;
    _handler = nullptr;
    self.request = nil;
    self.rewardedAd = nil;
    [super dealloc];
}

- (void)setUnitId:(const QString &)unitId {
    self.adUnitId = [NSString stringWithUTF8String:unitId.toUtf8().data()];
}

- (void)setTestDeviceId:(const QString &)testDeviceid {
    NSString *identifier = [NSString stringWithUTF8String:testDeviceid.toUtf8().data()];
    [GADMobileAds sharedInstance].requestConfiguration.testDeviceIdentifiers = @[identifier];
}

- (void)loadRewardedVideoAd
{
    if (!self.adUnitId) {
        return;
    }

    [GADRewardedAd loadWithAdUnitID:self.adUnitId request:self.request completionHandler:^(GADRewardedAd *ad, NSError *error) {
        if (error) {
            _handler->rewardedVideoAdFailedToLoad(static_cast<int>(error.code));
            return;
        }
        self.rewardedAd = ad;
        self.rewardedAd.fullScreenContentDelegate = self;
        _handler->rewardedVideoAdLoaded();
    }];
}

- (void)showVideo {
    if (!self.rewardedAd) {
        return;
    }

    UIViewController *controller = rootViewController();
    if (!controller) {
        _handler->rewardedVideoAdFailedToLoad(0);
        return;
    }

    [self.rewardedAd presentFromRootViewController:controller userDidEarnRewardHandler:^{
        _handler->rewarded();
        _handler->rewardedVideoCompleted();
    }];
}

- (void)ad:(nonnull id<GADFullScreenPresentingAd>)ad didFailToPresentFullScreenContentWithError:(nonnull NSError *)error {
    Q_UNUSED(ad);
    _handler->rewardedVideoAdFailedToLoad(static_cast<int>(error.code));
}

- (void)adWillPresentFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    Q_UNUSED(ad);
    _handler->rewardedVideoAdOpened();
    _handler->rewardedVideoStarted();
}

- (void)adWillDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    Q_UNUSED(ad);
    _handler->rewardedVideoAdClosed();
}

- (void)adDidDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    Q_UNUSED(ad);
    _handler->rewardedVideoAdClosed();
    self.rewardedAd = nil;
}

QtAdmobRewardVideoDelegateImpl::QtAdmobRewardVideoDelegateImpl() {
    self = [[QtAdmobRewardVideoDelegate alloc] init:this];
}

QtAdmobRewardVideoDelegateImpl::~QtAdmobRewardVideoDelegateImpl() {
    [(__bridge id)self dealloc];
}

void QtAdmobRewardVideoDelegateImpl::setQtAdmobRewardVideoIos(QmlRewardedVideoAd *qtAdmobRewardVideoIos) {
    m_qtAdmobRewardVideoIos = qtAdmobRewardVideoIos;
}

void QtAdmobRewardVideoDelegateImpl::setUnitId(const QString &unitId) {
    [(id) self setUnitId:unitId];
}

void QtAdmobRewardVideoDelegateImpl::setTestDeviceId(const QString &testDeviceId) {
    [(id) self setTestDeviceId:testDeviceId];
}

void QtAdmobRewardVideoDelegateImpl::loadRewardedVideoAd() {
    [(id) self loadRewardedVideoAd];
}

void QtAdmobRewardVideoDelegateImpl::show() {
    [(id) self showVideo];
}

void QtAdmobRewardVideoDelegateImpl::rewarded()
{
    if (m_qtAdmobRewardVideoIos) m_qtAdmobRewardVideoIos->rewarded();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoAdClosed()
{
    if (m_qtAdmobRewardVideoIos) m_qtAdmobRewardVideoIos->rewardedVideoAdClosed();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoAdFailedToLoad(int errorCode)
{
    if (m_qtAdmobRewardVideoIos) m_qtAdmobRewardVideoIos->rewardedVideoAdFailedToLoad(errorCode);
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoAdLeftApplication()
{
    if (m_qtAdmobRewardVideoIos) m_qtAdmobRewardVideoIos->rewardedVideoAdLeftApplication();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoAdLoaded()
{
    if (m_qtAdmobRewardVideoIos) m_qtAdmobRewardVideoIos->rewardedVideoAdLoaded();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoAdOpened()
{
    if (m_qtAdmobRewardVideoIos) m_qtAdmobRewardVideoIos->rewardedVideoAdOpened();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoCompleted()
{
    if (m_qtAdmobRewardVideoIos) m_qtAdmobRewardVideoIos->rewardedVideoCompleted();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoStarted()
{
    if (m_qtAdmobRewardVideoIos) m_qtAdmobRewardVideoIos->rewardedVideoStarted();
}

@end

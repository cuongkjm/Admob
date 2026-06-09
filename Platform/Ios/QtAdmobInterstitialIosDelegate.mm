#import "QtAdmobInterstitialIosDelegate.h"
#include "QmlInterstitialAd.h"

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

@implementation QtAdmobInterstitialIosDelegate

- (id)init:(QtAdmobInterstitialIosDelegateImpl *)handler {
    self = [super init];
    if (self)
    {
        _handler = handler;
        _request = [GADRequest request];
    }
    return self;
}

- (void)dealloc {
    _interstitialAd.fullScreenContentDelegate = nil;
    _handler = nullptr;
    _request = nil;
    _interstitialAd = nil;
    [super dealloc];
}

- (void)showInterstitialAd {
    if (!_interstitialAd) {
        return;
    }
    [_interstitialAd presentFromRootViewController:rootViewController()];
}

- (void)loadInterstitialAd {
    if (!_unitAdmobId) {
        return;
    }

    [GADInterstitialAd loadWithAdUnitID:_unitAdmobId request:_request completionHandler:^(GADInterstitialAd *ad, NSError *error) {
        if (error) {
            _handler->interstitialAdFailedToLoad(static_cast<int>(error.code));
            return;
        }
        _interstitialAd = ad;
        _interstitialAd.fullScreenContentDelegate = self;
        _handler->interstitialAdLoaded();
    }];
}

- (void)setInterstitialAdTestDeviceId:(const QString &)testDeviceId {
    NSString *identifier = [NSString stringWithUTF8String:testDeviceId.toUtf8().data()];
    [GADMobileAds sharedInstance].requestConfiguration.testDeviceIdentifiers = @[identifier];
}

- (void)setInterstitialAdUnitId:(const QString &)unitId {
    _unitAdmobId = [NSString stringWithUTF8String:unitId.toUtf8().data()];
}

- (void)ad:(nonnull id<GADFullScreenPresentingAd>)ad didFailToPresentFullScreenContentWithError:(nonnull NSError *)error {
    Q_UNUSED(ad);
    _handler->interstitialAdFailedToLoad(static_cast<int>(error.code));
}

- (void)adWillPresentFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    Q_UNUSED(ad);
    _handler->interstitialAdOpened();
}

- (void)adWillDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    Q_UNUSED(ad);
    _handler->interstitialAdClosed();
}

- (void)adDidDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    Q_UNUSED(ad);
    _handler->interstitialAdClosed();
    _interstitialAd = nil;
}

void QtAdmobInterstitialIosDelegateImpl::setQtAdmobInterstitialIos(QmlInterstitialAd *qtAdmobIntersitialIos)
{
    m_QtAdmobInterstitialIos = qtAdmobIntersitialIos;
}

void QtAdmobInterstitialIosDelegateImpl::setInterstitialAdUnitId(const QString &unitId)
{
    [(id) self setInterstitialAdUnitId:unitId];
}

void QtAdmobInterstitialIosDelegateImpl::setInterstitialAdTestDeviceId(const QString &testDeviceId) {
    [(id) self setInterstitialAdTestDeviceId:testDeviceId];
}

void QtAdmobInterstitialIosDelegateImpl::loadInterstitialAd() {
    [(id) self loadInterstitialAd];
}

void QtAdmobInterstitialIosDelegateImpl::showInterstitialAd() {
    [(id) self showInterstitialAd];
}

QtAdmobInterstitialIosDelegateImpl::~QtAdmobInterstitialIosDelegateImpl() {
    [(__bridge id)self dealloc];
}

QtAdmobInterstitialIosDelegateImpl::QtAdmobInterstitialIosDelegateImpl() {
    self = [[QtAdmobInterstitialIosDelegate alloc] init:this];
}

void QtAdmobInterstitialIosDelegateImpl::interstitialAdLeftApplication() {
    if (m_QtAdmobInterstitialIos) m_QtAdmobInterstitialIos->interstitialAdLeftApplication();
}

void QtAdmobInterstitialIosDelegateImpl::interstitialAdOpened() {
    if (m_QtAdmobInterstitialIos) m_QtAdmobInterstitialIos->interstitialAdOpened();
}

void QtAdmobInterstitialIosDelegateImpl::interstitialAdFailedToLoad(int errorCode) {
    if (m_QtAdmobInterstitialIos) m_QtAdmobInterstitialIos->interstitialAdFailedToLoad(errorCode);
}

void QtAdmobInterstitialIosDelegateImpl::interstitialAdClosed() {
    if (m_QtAdmobInterstitialIos) m_QtAdmobInterstitialIos->interstitialAdClosed();
}

void QtAdmobInterstitialIosDelegateImpl::interstitialAdLoaded() {
    if (m_QtAdmobInterstitialIos) m_QtAdmobInterstitialIos->interstitialAdLoaded();
}

@end

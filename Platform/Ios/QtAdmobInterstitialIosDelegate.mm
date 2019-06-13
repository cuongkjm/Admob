#import "QtAdmobInterstitialIosDelegate.h"
#include "QmlInterstitialAd.h"

@implementation QtAdmobInterstitialIosDelegate

- (id)init:(QtAdmobInterstitialIosDelegateImpl *)handler {
    self = [super init];
    if (self)
    {
        _handler = handler;
        _testDevices = [[NSMutableArray alloc] initWithArray:@[kGADSimulatorID]];
        [_interstitialView release];
        _interstitialView = [GADInterstitial alloc];
        _interstitialView.delegate = self;
        _request = [GADRequest alloc];
        _request.testDevices = _testDevices;
    }
    return self;
}

- (void)dealloc {
    _interstitialView.delegate = nil;
    _handler = nullptr;
    _testDevices = nil;
    _request = nil;
    [super dealloc];
}

- (void)showInterstitialAd {
    UIApplication *application = [UIApplication sharedApplication];
    NSArray *windows = [application windows];
    UIViewController * __block rootViewController = nil;
    [windows enumerateObjectsUsingBlock:^(UIWindow * _Nonnull window, NSUInteger, BOOL * _Nonnull stop) {
        rootViewController = [window rootViewController];
        *stop = (rootViewController != nil);
    }];
    
    [_interstitialView presentFromRootViewController:rootViewController];
}

- (void)loadInterstitialAd {
    if ([_interstitialView hasBeenUsed]) {
        [_interstitialView release];
        _interstitialView = [[GADInterstitial alloc] initWithAdUnitID:_unitAdmobId];
        _interstitialView.delegate = self;
    }
    [_interstitialView loadRequest:_request];
}

- (void)setInterstitialAdTestDeviceId:(const QString &)testDeviceId {
    [_request.testDevices arrayByAddingObject:[NSString stringWithUTF8String:testDeviceId.toUtf8().data()]];
}

- (void)setInterstitialAdUnitId:(const QString &)unitId {
    _unitAdmobId = [NSString stringWithUTF8String:unitId.toUtf8().data()];
    [_interstitialView initWithAdUnitID:_unitAdmobId];
}

- (void)interstitial:(GADInterstitial *)ad didFailToReceiveAdWithError:(GADRequestError *)error {
    Q_UNUSED(ad);
    _handler->interstitialAdFailedToLoad(static_cast<int>(error.code));
}

- (void)interstitialDidFailToPresentScreen:(GADInterstitial *)ad {
    Q_UNUSED(ad);
}

- (void)interstitialWillLeaveApplication:(GADInterstitial *)ad {
    Q_UNUSED(ad);
    _handler->interstitialAdLeftApplication();
}

- (void)interstitialWillPresentScreen:(GADInterstitial *)ad {
    Q_UNUSED(ad);
    _handler->interstitialAdOpened();
}

- (void)interstitialWillDismissScreen:(GADInterstitial *)ad {
    Q_UNUSED(ad);
    _handler->interstitialAdClosed();
}

- (void)interstitialDidDismissScreen:(GADInterstitial *)ad {
    Q_UNUSED(ad);
    _handler->interstitialAdClosed();
}

- (void)interstitialDidReceiveAd:(GADInterstitial *)ad {
    Q_UNUSED(ad);
    _handler->interstitialAdLoaded();
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
    [(id) self setInterstitialAdUnitId:testDeviceId];
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
    m_QtAdmobInterstitialIos->interstitialAdLeftApplication();
}


void QtAdmobInterstitialIosDelegateImpl::interstitialAdOpened() { 
    m_QtAdmobInterstitialIos->interstitialAdOpened();
}


void QtAdmobInterstitialIosDelegateImpl::interstitialAdFailedToLoad(int errorCode) { 
    m_QtAdmobInterstitialIos->interstitialAdFailedToLoad(errorCode);
}


void QtAdmobInterstitialIosDelegateImpl::interstitialAdClosed() { 
    m_QtAdmobInterstitialIos->interstitialAdClosed();
}


void QtAdmobInterstitialIosDelegateImpl::interstitialAdLoaded() { 
    m_QtAdmobInterstitialIos->interstitialAdLoaded();
}

@end

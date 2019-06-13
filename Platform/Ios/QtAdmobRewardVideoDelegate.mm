#include "QtAdmobRewardVideoDelegate.h"
#include "QtAdmobRewardVideoDelegateImpl.h"
#include "QmlRewardedVideoAd.h"

@implementation QtAdmobRewardVideoDelegate

- (id)init:(QtAdmobRewardVideoDelegateImpl *)handler {
    self = [super init];
    if (self)
    {
        _handler = handler;
        _testDevices = [[NSMutableArray alloc] initWithArray:@[kGADSimulatorID]];
        //_rewardVideoView = [GADRewardBasedVideoAd sharedInstance];
        //_rewardVideoView.delegate = self;
        [GADRewardBasedVideoAd sharedInstance].delegate = self;
        _request = [GADRequest alloc];
        _request.testDevices = _testDevices;
    }
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)setUnitId:(const QString &)unitId {
    _adUnitId = [NSString stringWithUTF8String:unitId.toUtf8().data()];
}

- (void)setTestDeviceId:(const QString &)testDeviceid {
    [_request.testDevices arrayByAddingObject:[NSString stringWithUTF8String:testDeviceid.toUtf8().data()]];
}

- (void)loadRewardedVideoAd
{
    [[GADRewardBasedVideoAd sharedInstance] loadRequest:_request withAdUnitID:_adUnitId];
}

- (void)showVideo {
    UIApplication *application = [UIApplication sharedApplication];
    NSArray *windows = [application windows];
    UIViewController * __block rootViewController = nil;
    [windows enumerateObjectsUsingBlock:^(UIWindow * _Nonnull window, NSUInteger, BOOL * _Nonnull stop) {
        rootViewController = [window rootViewController];
        *stop = (rootViewController != nil);
    }];
    
    [[GADRewardBasedVideoAd sharedInstance] presentFromRootViewController:rootViewController];
}

- (void)rewardBasedVideoAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd didFailToLoadWithError:(NSError *)error {
    Q_UNUSED(rewardBasedVideoAd);
    _handler->rewardedVideoAdFailedToLoad(static_cast<int>(error.code));
}

- (void)rewardBasedVideoAdWillLeaveApplication:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    Q_UNUSED(rewardBasedVideoAd);
    _handler->rewardedVideoAdLeftApplication();
}

- (void)rewardBasedVideoAdDidCompletePlaying:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    Q_UNUSED(rewardBasedVideoAd);
    _handler->rewardedVideoCompleted();
}

- (void)rewardBasedVideoAdMetadataDidChange:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    Q_UNUSED(rewardBasedVideoAd);
}

- (void)rewardBasedVideoAdDidStartPlaying:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    Q_UNUSED(rewardBasedVideoAd);
    _handler->rewardedVideoStarted();
}

- (void)rewardBasedVideoAdDidReceiveAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    Q_UNUSED(rewardBasedVideoAd);
    _handler->rewardedVideoAdLoaded();
}

- (void)rewardBasedVideoAdDidClose:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    Q_UNUSED(rewardBasedVideoAd);
    _handler->rewardedVideoAdClosed();
}

- (void)rewardBasedVideoAdDidOpen:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    Q_UNUSED(rewardBasedVideoAd);
    _handler->rewardedVideoAdOpened();
}

- (void)rewardBasedVideoAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd didRewardUserWithReward:(GADAdReward *)reward {
    Q_UNUSED(rewardBasedVideoAd);
    Q_UNUSED(reward);
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
    m_qtAdmobRewardVideoIos->rewarded();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoAdClosed()
{
    m_qtAdmobRewardVideoIos->rewardedVideoAdClosed();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoAdFailedToLoad(int errorCode)
{
    m_qtAdmobRewardVideoIos->rewardedVideoAdFailedToLoad(errorCode);
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoAdLeftApplication()
{
    m_qtAdmobRewardVideoIos->rewardedVideoAdLeftApplication();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoAdLoaded()
{
    m_qtAdmobRewardVideoIos->rewardedVideoAdLoaded();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoAdOpened()
{
    m_qtAdmobRewardVideoIos->rewardedVideoAdOpened();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoCompleted()
{
    m_qtAdmobRewardVideoIos->rewardedVideoCompleted();
}

void QtAdmobRewardVideoDelegateImpl::rewardedVideoStarted()
{
    m_qtAdmobRewardVideoIos->rewardedVideoStarted();
}

@end

#ifndef QTADMOBREWARDVIDEODELEGATE_H
#define QTADMOBREWARDVIDEODELEGATE_H

#include "QtAdmobRewardVideoDelegateImpl.h"
#include <GoogleMobileAds/GADRewardBasedVideoAd.h>
#include <GoogleMobileAds/GADMobileAds.h>
#include <QString>

@interface QtAdmobRewardVideoDelegate : NSObject<GADRewardBasedVideoAdDelegate>

//@property (nonatomic, strong) GADRewardBasedVideoAd* rewardVideoView;
@property (nonatomic, strong) NSMutableArray* testDevices;
@property (nonatomic, assign) QtAdmobRewardVideoDelegateImpl *handler;
@property (nonatomic, strong) GADRequest *request;
@property (nonatomic, strong) NSString* adUnitId;

- (id) init: (QtAdmobRewardVideoDelegateImpl*) handler;
- (void) dealloc;
- (void) setUnitId: (const QString &) unitId;
- (void) setTestDeviceId: (const QString &) testDeviceid;
- (void) loadRewardedVideoAd;
- (void) showVideo;

- (void) rewardBasedVideoAdDidOpen:(GADRewardBasedVideoAd *)rewardBasedVideoAd;
- (void) rewardBasedVideoAdDidClose:(GADRewardBasedVideoAd *)rewardBasedVideoAd;
- (void) rewardBasedVideoAdDidReceiveAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd;
- (void) rewardBasedVideoAdDidStartPlaying:(GADRewardBasedVideoAd *)rewardBasedVideoAd;
- (void) rewardBasedVideoAdMetadataDidChange:(GADRewardBasedVideoAd *)rewardBasedVideoAd;
- (void) rewardBasedVideoAdDidCompletePlaying:(GADRewardBasedVideoAd *)rewardBasedVideoAd;
- (void) rewardBasedVideoAdWillLeaveApplication:(GADRewardBasedVideoAd *)rewardBasedVideoAd;
- (void) rewardBasedVideoAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd didFailToLoadWithError:(NSError *)error;
- (void) rewardBasedVideoAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd didRewardUserWithReward:(GADAdReward *)reward;
@end

#endif // QTADMOBREWARDVIDEODELEGATE_H

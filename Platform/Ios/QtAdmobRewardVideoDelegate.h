#ifndef QTADMOBREWARDVIDEODELEGATE_H
#define QTADMOBREWARDVIDEODELEGATE_H

#include "QtAdmobRewardVideoDelegateImpl.h"
#include <GoogleMobileAds/GADRewardedAd.h>
#include <GoogleMobileAds/GADMobileAds.h>
#include <QString>

@interface QtAdmobRewardVideoDelegate : NSObject<GADFullScreenContentDelegate>

@property (nonatomic, strong, nullable) GADRewardedAd* rewardedAd;
@property (nonatomic, assign, nullable) QtAdmobRewardVideoDelegateImpl *handler;
@property (nonatomic, strong, nullable) GADRequest *request;
@property (nonatomic, strong, nullable) NSString* adUnitId;

- (nonnull id) init: (QtAdmobRewardVideoDelegateImpl *_Nonnull) handler;
- (void) dealloc;
- (void) setUnitId: (const QString &) unitId;
- (void) setTestDeviceId: (const QString &) testDeviceid;
- (void) loadRewardedVideoAd;
- (void) showVideo;

- (void)ad:(nonnull id<GADFullScreenPresentingAd>)ad
    didFailToPresentFullScreenContentWithError:(nonnull NSError *)error;
- (void)adWillPresentFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad;
- (void)adWillDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad;
- (void)adDidDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad;

@end

#endif // QTADMOBREWARDVIDEODELEGATE_H

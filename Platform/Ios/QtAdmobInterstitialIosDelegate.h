#ifndef QTADMOBINTERSTITIALIOSDELEGATE_H
#define QTADMOBINTERSTITIALIOSDELEGATE_H

#include "QtAdmobInterstitialIosDelegateImpl.h"
#include <GoogleMobileAds/GADInterstitialAd.h>
#include <GoogleMobileAds/GADMobileAds.h>
#include <QString>

@interface QtAdmobInterstitialIosDelegate : NSObject<GADFullScreenContentDelegate>

@property (nonatomic, strong, nullable) GADInterstitialAd* interstitialAd;
@property (nonatomic, assign, nullable) QtAdmobInterstitialIosDelegateImpl *handler;
@property (nonatomic, strong, nullable) GADRequest *request;
@property (nonatomic, strong, nullable) NSString *unitAdmobId;

- (nonnull id) init: (QtAdmobInterstitialIosDelegateImpl *_Nonnull) handler;
- (void) dealloc;
- (void) setInterstitialAdUnitId: (const QString &) unitId;
- (void) setInterstitialAdTestDeviceId: (const QString &) testDeviceId;
- (void) loadInterstitialAd;
- (void) showInterstitialAd;

- (void)ad:(nonnull id<GADFullScreenPresentingAd>)ad
    didFailToPresentFullScreenContentWithError:(nonnull NSError *)error;
- (void)adWillPresentFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad;
- (void)adWillDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad;
- (void)adDidDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad;

@end

#endif // QTADMOBINTERSTITIALIOSDELEGATE_H

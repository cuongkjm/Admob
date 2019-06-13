#ifndef QTADMOBINTERSTITIALIOSDELEGATE_H
#define QTADMOBINTERSTITIALIOSDELEGATE_H

#include "QtAdmobInterstitialIosDelegateImpl.h"
#include <GoogleMobileAds/GADInterstitial.h>
#include <GoogleMobileAds/GADMobileAds.h>
#include <QString>

@interface QtAdmobInterstitialIosDelegate : NSObject<GADInterstitialDelegate>

@property (nonatomic, strong) GADInterstitial* interstitialView;
@property (nonatomic, strong) NSMutableArray* testDevices;
@property (nonatomic, assign) QtAdmobInterstitialIosDelegateImpl *handler;
@property (nonatomic, strong) GADRequest *request;
@property (nonatomic, strong) NSString *unitAdmobId;

- (id) init: (QtAdmobInterstitialIosDelegateImpl*) handler;
- (void) dealloc;
- (void) setInterstitialAdUnitId: (const QString &) unitId;
- (void) setInterstitialAdTestDeviceId: (const QString &) testDeviceId;
- (void) loadInterstitialAd;
- (void) showInterstitialAd;

- (void) interstitialDidReceiveAd:(GADInterstitial *)ad;
- (void) interstitialDidDismissScreen:(GADInterstitial *)ad;
- (void) interstitialWillDismissScreen:(GADInterstitial *)ad;
- (void) interstitialWillPresentScreen:(GADInterstitial *)ad;
- (void) interstitialWillLeaveApplication:(GADInterstitial *)ad;
- (void) interstitialDidFailToPresentScreen:(GADInterstitial *)ad;
- (void) interstitial:(GADInterstitial *)ad didFailToReceiveAdWithError:(GADRequestError *)error;

@end

#endif // QTADMOBINTERSTITIALIOSDELEGATE_H

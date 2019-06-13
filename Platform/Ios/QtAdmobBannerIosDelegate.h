#ifndef QTADMOBBANNERIOSDELEGATE_H
#define QTADMOBBANNERIOSDELEGATE_H

#import "QtAdmobBannerIosDelegateImpl.h"
#include <GoogleMobileAds/GADBannerView.h>
#include <GoogleMobileAds/GADAdSize.h>
#include <GoogleMobileAds/GADMobileAds.h>
#include <QString>

@interface QtAdmobBannerIosDelegate : NSObject<GADBannerViewDelegate>

@property (nonatomic, strong) GADBannerView* bannerView;
@property (nonatomic, strong) NSMutableArray* testDevices;
@property (nonatomic, assign) QtAdmobBannerIosDelegateImpl *handler;
@property (nonatomic, strong) GADRequest *request;

-(id) init: (QtAdmobBannerIosDelegateImpl*) handler;
-(void) dealloc;
-(void) setPosition: (const int &) x : (const int &) y;
-(void) setUnitId: (const QString &) unitId;
-(void) loadBanner;
-(void) setBannerSize: (QtAdmobBannerIosDelegateImpl::BannerSizes) size;
-(int) getAdBannerWidth;
-(int) getAdBannerHeight;
-(void) setVisible: (const bool &) visible;
-(void) setTestDeviceId: (const QString &) deviceId;

//function from GADBannerViewDelegate
- (void)adViewDidReceiveAd:(nonnull GADBannerView *)view;
- (void)adView:(nonnull GADBannerView *)view
didFailToReceiveAdWithError:(nonnull GADRequestError *)error;
- (void)adViewWillPresentScreen:(nonnull GADBannerView *)view;
- (void)adViewWillDismissScreen:(nonnull GADBannerView *)view;
- (void)adViewDidDismissScreen:(nonnull GADBannerView *)view;
- (void)adViewWillLeaveApplication:(nonnull GADBannerView *)view;

@end

#endif // QTADMOBBANNERIOSDELEGATE_H

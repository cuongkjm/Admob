#ifndef QTADMOBBANNERIOSDELEGATE_H
#define QTADMOBBANNERIOSDELEGATE_H

#import "QtAdmobBannerIosDelegateImpl.h"
#include <GoogleMobileAds/GADBannerView.h>
#include <GoogleMobileAds/GADAdSize.h>
#include <GoogleMobileAds/GADMobileAds.h>
#include <QString>

@interface QtAdmobBannerIosDelegate : NSObject<GADBannerViewDelegate>

@property (nonatomic, strong, nullable) GADBannerView* bannerView;
@property (nonatomic, assign, nullable) QtAdmobBannerIosDelegateImpl *handler;
@property (nonatomic, strong, nullable) GADRequest *request;

-(nonnull id) init: (QtAdmobBannerIosDelegateImpl *_Nonnull) handler;
-(void) dealloc;
-(void) setPosition: (const int &) x : (const int &) y;
-(void) setUnitId: (const QString &) unitId;
-(void) loadBanner;
-(void) setBannerSize: (QtAdmobBannerIosDelegateImpl::BannerSizes) size;
-(int) getAdBannerWidth;
-(int) getAdBannerHeight;
-(void) setVisible: (const bool &) visible;
-(void) setTestDeviceId: (const QString &) deviceId;

- (void)bannerViewDidReceiveAd:(nonnull GADBannerView *)bannerView;
- (void)bannerView:(nonnull GADBannerView *)bannerView
    didFailToReceiveAdWithError:(nonnull NSError *)error;
- (void)bannerViewWillPresentScreen:(nonnull GADBannerView *)bannerView;
- (void)bannerViewWillDismissScreen:(nonnull GADBannerView *)bannerView;
- (void)bannerViewDidDismissScreen:(nonnull GADBannerView *)bannerView;

@end

#endif // QTADMOBBANNERIOSDELEGATE_H

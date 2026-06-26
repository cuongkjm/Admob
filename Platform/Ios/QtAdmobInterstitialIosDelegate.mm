#import "QtAdmobInterstitialIosDelegate.h"
#include "QmlInterstitialAd.h"

#include <QMetaObject>
#include <QPointer>

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

@implementation QtAdmobInterstitialIosDelegate

- (id)init:(QtAdmobInterstitialIosDelegateImpl *)handler {
    self = [super init];
    if (self)
    {
        _handler = handler;
        self.request = [GADRequest request];
    }
    return self;
}

- (void)dealloc {
    self.interstitialAd.fullScreenContentDelegate = nil;
    _handler = nullptr;
    self.request = nil;
    self.interstitialAd = nil;
    [super dealloc];
}

- (void)showInterstitialAd {
    if (!self.interstitialAd) {
        if (_handler) _handler->interstitialAdFailedToLoad(0);
        return;
    }

    UIViewController *controller = rootViewController();
    if (!controller) {
        if (_handler) _handler->interstitialAdFailedToLoad(0);
        return;
    }

    [self.interstitialAd presentFromRootViewController:controller];
}

- (void)loadInterstitialAd {
    if (!self.unitAdmobId) {
        return;
    }

    [GADInterstitialAd loadWithAdUnitID:self.unitAdmobId request:self.request completionHandler:^(GADInterstitialAd *ad, NSError *error) {
        if (!_handler) {
            return;
        }
        if (error) {
            _handler->interstitialAdFailedToLoad(static_cast<int>(error.code));
            return;
        }
        self.interstitialAd = ad;
        self.interstitialAd.fullScreenContentDelegate = self;
        _handler->interstitialAdLoaded();
    }];
}

- (void)setInterstitialAdTestDeviceId:(const QString &)testDeviceId {
    NSString *identifier = [NSString stringWithUTF8String:testDeviceId.toUtf8().data()];
    [GADMobileAds sharedInstance].requestConfiguration.testDeviceIdentifiers = @[identifier];
}

- (void)setInterstitialAdUnitId:(const QString &)unitId {
    self.unitAdmobId = [NSString stringWithUTF8String:unitId.toUtf8().data()];
}

- (void)ad:(nonnull id<GADFullScreenPresentingAd>)ad didFailToPresentFullScreenContentWithError:(nonnull NSError *)error {
    Q_UNUSED(ad);
    if (_handler) _handler->interstitialAdFailedToLoad(static_cast<int>(error.code));
}

- (void)adWillPresentFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    Q_UNUSED(ad);
    if (_handler) _handler->interstitialAdOpened();
}

- (void)adDidRecordClick:(nonnull id<GADFullScreenPresentingAd>)ad {
    Q_UNUSED(ad);
    if (_handler) _handler->interstitialAdLeftApplication();
}

- (void)adWillDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    Q_UNUSED(ad);
}

- (void)adDidDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    Q_UNUSED(ad);
    if (_handler) _handler->interstitialAdClosed();
    self.interstitialAd = nil;
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
    QPointer<QmlInterstitialAd> guard(m_QtAdmobInterstitialIos);
    if (guard) {
        QMetaObject::invokeMethod(guard.data(), [guard]() {
            if (guard) emit guard->interstitialAdLeftApplication();
        }, Qt::QueuedConnection);
    }
}

void QtAdmobInterstitialIosDelegateImpl::interstitialAdOpened() {
    QPointer<QmlInterstitialAd> guard(m_QtAdmobInterstitialIos);
    if (guard) {
        QMetaObject::invokeMethod(guard.data(), [guard]() {
            if (guard) emit guard->interstitialAdOpened();
        }, Qt::QueuedConnection);
    }
}

void QtAdmobInterstitialIosDelegateImpl::interstitialAdFailedToLoad(int errorCode) {
    QPointer<QmlInterstitialAd> guard(m_QtAdmobInterstitialIos);
    if (guard) {
        QMetaObject::invokeMethod(guard.data(), [guard, errorCode]() {
            if (guard) emit guard->interstitialAdFailedToLoad(errorCode);
        }, Qt::QueuedConnection);
    }
}

void QtAdmobInterstitialIosDelegateImpl::interstitialAdClosed() {
    QPointer<QmlInterstitialAd> guard(m_QtAdmobInterstitialIos);
    if (guard) {
        QMetaObject::invokeMethod(guard.data(), [guard]() {
            if (guard) emit guard->interstitialAdClosed();
        }, Qt::QueuedConnection);
    }
}

void QtAdmobInterstitialIosDelegateImpl::interstitialAdLoaded() {
    QPointer<QmlInterstitialAd> guard(m_QtAdmobInterstitialIos);
    if (guard) {
        QMetaObject::invokeMethod(guard.data(), [guard]() {
            if (guard) emit guard->interstitialAdLoaded();
        }, Qt::QueuedConnection);
    }
}

@end

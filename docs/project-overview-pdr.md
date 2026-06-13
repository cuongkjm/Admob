# Project Overview & Product Development Requirements

## 1. Product Overview

QtAdMob is a Qt 6 CMake library that exposes Google AdMob banner, interstitial, and rewarded ads to C++/QML apps.

Real ads are supported on Android and iOS. Windows, macOS desktop, and Linux are supported as compile-safe no-op targets so one shared Qt/QML source tree can build across the main OS families.

## 2. Target Users

Qt/QML developers who want one declarative API for mobile ads while keeping desktop builds simple and portable.

## 3. Supported Ad Formats

- `QmlBanner`: banner ads with unit id, size, position, visibility, width, and height.
- `QmlInterstitialAd`: full-screen interstitial ads with load/show lifecycle signals.
- `QmlRewardedVideoAd`: rewarded ads with reward and lifecycle signals.

## 4. Functional Requirements

### Banner Ads
- **F-BAN-01**: Set AdMob unit id and test device id.
- **F-BAN-02**: Support standard banner size enum values.
- **F-BAN-03**: Set banner position and visibility.
- **F-BAN-04**: Expose read-only native banner width and height.
- **F-BAN-05**: Emit mobile lifecycle signals from real platform SDK callbacks.
- **F-BAN-06**: Desktop `loadBanner()` is safe no-op and emits no fake success.

### Interstitial Ads
- **F-INT-01**: Set AdMob unit id and test device id.
- **F-INT-02**: Load and show interstitial ads on Android/iOS.
- **F-INT-03**: Emit mobile lifecycle signals from real platform SDK callbacks.
- **F-INT-04**: Desktop load/show methods are safe no-op and emit no fake success.

### Rewarded Ads
- **F-REW-01**: Set AdMob unit id and test device id.
- **F-REW-02**: Load and show rewarded ads on Android/iOS.
- **F-REW-03**: Emit reward only when the native mobile SDK reports a real reward.
- **F-REW-04**: Desktop load/show methods are safe no-op and emit no fake reward.

## 5. Non-Functional Requirements

### Portability
- **NF-PORT-01**: Build as a Qt 6 CMake submodule through `add_subdirectory()` or `FetchContent`.
- **NF-PORT-02**: Support Android, iOS, Windows, macOS desktop, and Linux from one source tree.
- **NF-PORT-03**: Desktop builds require Qt 6 only, with no Android/iOS/AdMob SDK dependency.

### Integration
- **NF-DEV-01**: Expose one namespaced target: `QtAdMob::qtadmob`.
- **NF-DEV-02**: Android consumers call `qtadmob_configure_android_target(myapp)` after linking.
- **NF-DEV-03**: iOS consumers provide Google Mobile Ads SDK through SPM, CocoaPods, or `GOOGLE_MOBILE_ADS_IOS_ROOT`; manual SDK `13.5.0` is validated.
- **NF-DEV-04**: Android consumers can merge QtAdMob package sources through `qtadmob_configure_android_target(myapp)`.

### Safety
- **NF-SAFE-01**: Android callbacks validate native pointers before dispatch.
- **NF-SAFE-02**: Signal emissions from JNI are queued back to Qt.
- **NF-SAFE-03**: Destructors detach native callbacks before wrapper deletion.

## 6. Validation State

- macOS desktop build passed with no-op methods.
- Android `arm64-v8a` build passed with Qt 6 JNI and `com.qtadmob` package sources.
- iOS build passed with Google Mobile Ads iOS SDK `13.5.0` from `GOOGLE_MOBILE_ADS_IOS_ROOT`.
- Non-blocking iOS warnings remain for deprecated `UIApplication.windows` and `statusBarFrame` in banner delegate code.

## Unresolved Questions
- None.

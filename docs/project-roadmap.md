# Project Roadmap & Milestones

## 1. Current Platform Support

| Feature / Format | Android | iOS | Windows | macOS Desktop | Linux | Status |
| --- | --- | --- | --- | --- | --- | --- |
| `QmlBanner` | Real ads | Real ads | No-op | No-op | No-op | Implemented |
| `QmlInterstitialAd` | Real ads | Real ads | No-op | No-op | No-op | Implemented |
| `QmlRewardedVideoAd` | Real ads | Real ads | No-op | No-op | No-op | Implemented |
| Qt 6 CMake target | Yes | Yes | Yes | Yes | Yes | Implemented |
| Android package helper | Yes | No-op | No-op | No-op | No-op | Implemented |

## 2. Completed Milestones

### Milestone 1: Original Mobile Wrapper
- Added QML-facing banner, interstitial, and rewarded ad wrappers.
- Added Android and iOS platform bridges for Google Mobile Ads.

### Milestone 2: Qt 6 CMake Modernization
- Standardized integration through root `CMakeLists.txt`.
- Added `QtAdMob::qtadmob` target for submodule consumption.
- Migrated Android JNI wrappers to Qt 6 `QJniObject`.
- Replaced Android singleton callback routing with native-pointer callbacks.
- Added `ActiveRegistry` pointer validation and queued Qt signal dispatch.
- Replaced legacy custom Android activity with `com.qtadmob` Java helpers.

### Milestone 3: Cross-Platform Compile Portability
- Documented Android, iOS, Windows, macOS desktop, and Linux behavior.
- Desktop platforms compile public API as no-op because AdMob has no desktop SDK.
- Documented official Google Mobile Ads iOS SDK acquisition paths.

### Milestone 4: iOS SDK 13.5 Modernization
- Modernized iOS interstitial/rewarded APIs with `GADInterstitialAd` and `GADRewardedAd`.
- Adopted `GADFullScreenContentDelegate` and `GADBannerViewDelegate` callback paths.
- Validated iOS build with Google Mobile Ads iOS SDK `13.5.0` via `GOOGLE_MOBILE_ADS_IOS_ROOT`.
- Tracked remaining non-blocking banner warnings for deprecated `UIApplication.windows` and `statusBarFrame`.

### Milestone 5: Build Validation
- Validated macOS desktop CMake build with no-op ad methods.
- Validated Android `arm64-v8a` build with Qt 6 JNI and `com.qtadmob` package sources.
- Validated iOS CMake build with manual Google Mobile Ads iOS SDK path.

## 3. Future Roadmap

### Phase 1: Consumer App Packaging Test
- Validate Android Gradle packaging inside a real Qt app target.
- Confirm `QT_ANDROID_PACKAGE_SOURCE_DIR` merge behavior when consumer app already has package sources.

### Phase 2: Banner Improvements
- Replace deprecated iOS banner positioning APIs with scene/safe-area aware APIs.
- Add adaptive banner sizing when needed by apps.
- Recheck orientation and safe-area behavior across Android and iOS.

## Unresolved Questions
- Which Windows/Linux CI runners should be used for validated desktop builds?

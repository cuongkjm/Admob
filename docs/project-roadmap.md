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
- Replaced QMake integration with root `CMakeLists.txt`.
- Added `QtAdMob::qtadmob` target for submodule consumption.
- Migrated Android JNI wrappers to Qt 6 `QJniObject`.
- Replaced Android singleton callback routing with native-pointer callbacks.
- Added `ActiveRegistry` pointer validation and queued Qt signal dispatch.
- Replaced legacy custom Android activity with `com.qtadmob` Java helpers.

### Milestone 3: Cross-Platform Compile Portability
- Documented Android, iOS, Windows, macOS desktop, and Linux behavior.
- Desktop platforms compile public API as no-op because AdMob has no desktop SDK.
- Documented official Google Mobile Ads iOS SDK acquisition paths.

## 3. Future Roadmap

### Phase 1: Modernize iOS SDK APIs
- Replace legacy iOS rewarded/interstitial APIs with current Google Mobile Ads iOS SDK classes.
- Revalidate iOS build with a real `GoogleMobileAds.xcframework` path.

### Phase 2: Consumer App Packaging Test
- Validate Android Gradle packaging inside a real Qt app target.
- Confirm `QT_ANDROID_PACKAGE_SOURCE_DIR` merge behavior when consumer app already has package sources.

### Phase 3: Banner Improvements
- Add adaptive banner sizing when needed by apps.
- Recheck orientation and safe-area behavior across Android and iOS.

## Unresolved Questions
- Which Windows/Linux CI runners should be used for validated desktop builds?

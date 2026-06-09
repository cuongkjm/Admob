# Project Overview & Product Development Requirements (PDR)

This document provides a high-level overview of the Qt AdMob library and specifies its product development requirements.

## 1. Product Overview

The **Qt AdMob Library** is a cross-platform integration wrapper that allows C++ and QML-based Qt applications to easily serve advertisements using the **Google Mobile Ads SDK (AdMob)** on both Android and iOS devices.

### Purpose
The primary purpose is to provide a unified, clean, and declarative QML API that abstracts the complex underlying platform-specific integration patterns (Android JNI/Java and iOS Objective-C++) for Google Mobile Ads.

### Supported Ad Formats
1. **Banner Ads (`QmlBanner`)**: Rectangular image or text ads that occupy a portion of an app's layout. Features customizable sizing, positioning, and visibility control.
2. **Interstitial Ads (`QmlInterstitialAd`)**: Full-screen ads that cover the interface of their host app until closed by the user. Best displayed during natural transition points in the app flow.
3. **Rewarded Video Ads (`QmlRewardedVideoAd`)**: Full-screen video ads that reward users for watching.

### Target Audience
Qt and QML mobile application developers looking for a robust, performant, and easy-to-use AdMob wrapper without needing to write platform-specific JNI or Objective-C glue code themselves.

---

## 2. Product Requirements (PDR)

### Functional Requirements

#### Banner Ads (`QmlBanner`)
- **F-BAN-01**: Must allow setting an AdMob Unit ID (`unitId`) and a Test Device ID (`testDeviceId`).
- **F-BAN-02**: Must support multiple standard banner sizes via a `BannerSizes` enumeration (e.g., `BANNER`, `FLUID`, `FULL_BANNER`, `LARGE_BANNER`, `LEADERBOARD`, `MEDIUM_RECTANGLE`, `SMART_BANNER`, `WIDE_SKYSCRAPER`).
- **F-BAN-03**: Must allow dynamic setting of positions (`x` and `y` coordinates) on the screen.
- **F-BAN-04**: Must automatically handle status bar height offsets on Android and iOS so ads do not overlap system status bars unless specified.
- **F-BAN-05**: Must support visibility toggles (`visible` property).
- **F-BAN-06**: Must expose read-only actual `width` and `height` properties in pixels, notifying QML when they change.
- **F-BAN-07**: Must emit signals for Lifecycle Events:
  - `bannerLoaded()`
  - `bannerFailedToLoad(int errorCode)`
  - `bannerOpened()`
  - `bannerClosed()`
  - `bannerLeftApplication()`

#### Interstitial Ads (`QmlInterstitialAd`)
- **F-INT-01**: Must support setting an AdMob Unit ID (`unitId`) and a Test Device ID (`testDeviceId`).
- **F-INT-02**: Must provide a `loadInterstitialAd()` slot to fetch the ad asynchronously in the background.
- **F-INT-03**: Must provide a `showInterstitialAd()` slot to present the loaded ad on screen.
- **F-INT-04**: Must emit signals for Lifecycle Events:
  - `interstitialAdLoaded()`
  - `interstitialAdFailedToLoad(int errorCode)`
  - `interstitialAdOpened()`
  - `interstitialAdClosed()`
  - `interstitialAdLeftApplication()`

#### Rewarded Video Ads (`QmlRewardedVideoAd`)
- **F-REW-01**: Must support setting an AdMob Unit ID (`unitId`) and a Test Device ID (`testDeviceId`).
- **F-REW-02**: Must provide a `loadRewardedVideoAd()` slot to fetch the ad asynchronously.
- **F-REW-03**: Must provide a `show()` slot to display the video ad.
- **F-REW-04**: Must trigger rewards accurately upon user completion.
- **F-REW-05**: Must emit signals for Lifecycle Events:
  - `rewardedVideoAdLoaded()`
  - `rewardedVideoAdFailedToLoad(int errorCode)`
  - `rewardedVideoAdOpened()`
  - `rewardedVideoAdClosed()`
  - `rewardedVideoAdLeftApplication()`
  - `rewardedVideoStarted()`
  - `rewardedVideoCompleted()`
  - `rewarded()` (emitted when user successfully qualifies for reward)

### Non-Functional Requirements

#### Performance & Responsiveness
- **NF-PER-01**: All ad-loading requests must execute asynchronously. Main UI thread (QML rendering) must not block or stutter during network fetches.
- **NF-PER-02**: Platform-specific UI operations (like creating and adding `AdView` container overlays) must run on the appropriate platform thread (e.g., `runOnUiThread` in Android).

#### Developer Integration Experience
- **NF-DEV-01**: Installation must be achievable by including a single project include (`Admob.pri`) in the main `.pro` file of a Qt project.
- **NF-DEV-02**: Access to the ad managers must be clean, leveraging singleton-like access patterns (`Instances()` static methods) or declarative QML item instantiations.

#### Compatibility & Maintainability
- **NF-CMP-01**: Must support Android (minimum SDK level compatible with Google Mobile Ads) and iOS (iOS 10+ target).
- **NF-CMP-02**: Library should keep dependencies minimal, utilizing Qt's native `androidextras` on Android, and linking directly to official Google Mobile Ads SDK framework binaries on iOS.

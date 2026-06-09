# Project Roadmap & Milestones

This document captures the implementation status, release history, and long-term roadmap for the Qt AdMob Integration library.

## 1. Project Implementation Status

### Platform Support

| Feature / Format | Android OS Support | iOS Support | Status |
|:---|:---:|:---:|:---:|
| **Banner Ads** (`QmlBanner`) | Included | Included | Completed |
| **Interstitial Ads** (`QmlInterstitial`) | Included | Included | Completed |
| **Rewarded Video Ads** (`QmlRewardedVideo`) | Included | Included | Completed |
| **Auto offset logic** (status bar height) | Included | Included | Completed |
| **Integration Config** (`Admob.pri`) | Ready | Ready | Completed |

---

## 2. Completed Milestones

### Milestone 1: Core Core Architecture & Android Support
- Established the base `QmlBanner`, `QmlInterstitialAd`, and `QmlRewardedVideoAd` classes.
- Created `QtAdMobActivity.java` to interface with the Google Mobile Ads SDK for Android.
- Implemented core JNI bridging bindings and lifecycle signal mapping.
- Added automatic status bar height calculation on Android to resolve overlapping layouts.

### Milestone 2: iOS Support Integration
- Added Objective-C++ delegates (`QtAdmobBannerIosDelegate`, etc.) to map platform-specific events to C++ signals.
- Configured link flags inside `Admob.pri` to include the required iOS framework SDK binaries (`GoogleMobileAds`, `GoogleAppMeasurement`, `GoogleUtilities`, and `nanopb`).
- Integrated default iOS configuration properties via `Info.plist` (such as the default `GADApplicationIdentifier`).

---

## 3. Future Roadmap

As mobile development frameworks progress, we intend to implement the following upgrades:

### Phase 1: Modernize SDK Implementations (High Priority)
- **Upgrade Google Mobile Ads SDK**: The legacy iOS implementation uses `GADRewardBasedVideoAdDelegate` (which has been deprecated in newer AdMob SDK releases). Update rewarded implementation to use the modern `GADRewardedAd` API.
- **Support Android User Messaging Platform (UMP)**: Integrate compliance checks for GDPR / CMP and User Consent forms as mandated by Google policies.

### Phase 2: Migrate to Qt 6 (Medium Priority)
- **CMake Compilation Build Support**: Traditional `.pro`/`.pri` configurations are being deprecated in modern Qt build steps. Create `CMakeLists.txt` build files to support modern Qt 6 compiler workflows.
- **Migrate Android Extras JNI calls**: Replace legacy `QAndroidJniObject` calls with Qt 6's unified `QJniObject` declarations.

### Phase 3: Developer Quality of Life (QoL) Enhancements
- **Dynamic Orientation Handling**: Improve banner redraw boundaries when devices rotate between landscape and portrait.
- **Adaptive Banner Sizing Support**: Introduce AdMob Adaptive Banners to dynamically size banners to best fit screen aspects.

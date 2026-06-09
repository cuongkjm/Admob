# Codebase Summary

This document maps out the AdMob Qt integration library codebase, describing the files, their sizes, their core responsibilities, and how they fit together.

## 1. Directory Structure Overview

The repository is structured logically to separate C++ QML wrapper bindings from the platform-specific native implementations:

```
.
├── Admob.pri                    # Qt Project include file containing compile config
├── QmlBanner.h / .cpp           # C++ Wrapper for Banner Ads
├── QmlInterstitialAd.h / .cpp   # C++ Wrapper for Interstitial Ads
├── QmlRewardedVideoAd.h / .cpp  # C++ Wrapper for Rewarded Video Ads
├── Platform/
│   ├── Android/
│   │   └── src/org/qtproject/
│   │       └── QtAdMobActivity.java # Subclassed QtActivity implementing AdMob SDK on Android
│   └── Ios/
│       ├── Info.plist           # iOS Configuration Plist with AdMob GADApplicationIdentifier
│       ├── QtAdmobBannerIosDelegate.h / .mm
│       ├── QtAdmobBannerIosDelegateImpl.h
│       ├── QtAdmobInterstitialIosDelegate.h / .mm
│       ├── QtAdmobInterstitialIosDelegateImpl.h
│       ├── QtAdmobRewardVideoDelegate.h / .mm
│       └── QtAdmobRewardVideoDelegateImpl.h
└── docs/                        # Technical Documentation
```

---

## 2. File Directory & Responsibility Matrix

| File Path | Language | Approximate LOC | Core Responsibility |
|:---|:---|:---:|:---|
| `Admob.pri` | QMake | 66 | Sets compiler flags, framework search paths, links Google Mobile Ads framework for iOS, includes `androidextras` for Android, and defines header/source inclusion files. |
| `QmlBanner.h` / `QmlBanner.cpp` | C++ / Qt | 334 | Wraps Banner functionality. Exposes QML-bindable properties (`unitId`, `bannerSize`, `x`, `y`, `visible`, etc.) and handles direct Android JNI / iOS ObjC delegate execution. |
| `QmlInterstitialAd.h` / `QmlInterstitialAd.cpp` | C++ / Qt | 210 | Wraps Interstitial Ads. Handles loading and showing interstitial ads on both OS layers. |
| `QmlRewardedVideoAd.h` / `QmlRewardedVideoAd.cpp` | C++ / Qt | 234 | Wraps Rewarded Video Ads. Communicates rewards and lifecycle states to QML. |
| `Platform/Android/src/org/qtproject/QtAdMobActivity.java` | Java | 511 | Android Main Activity. Directly integrates Google Play Services Mobile Ads. Handles JNI callbacks, status bar height calculation, and main thread UI scheduling for Android views. |
| `Platform/Ios/QtAdmobBannerIosDelegate.h` / `...Delegate.mm` | Objective-C++ | 280 | Implements `GADBannerViewDelegate`. Connects a native `GADBannerView` to the root view controller of the Qt iOS application. |
| `Platform/Ios/QtAdmobBannerIosDelegateImpl.h` | C++ | 51 | Pure C++ header representing the internal bridge context between `QmlBanner` and `QtAdmobBannerIosDelegate`. |
| `Platform/Ios/QtAdmobInterstitialIosDelegate.h` / `...Delegate.mm` | Objective-C++ | 186 | Implements `GADInterstitialDelegate` for lifecycle updates and manages native iOS interstitial loading and playback. |
| `Platform/Ios/QtAdmobInterstitialIosDelegateImpl.h` | C++ | 33 | Bridge representation for the C++/Objective-C boundary on iOS Interstitial ads. |
| `Platform/Ios/QtAdmobRewardVideoDelegate.h` / `...Delegate.mm` | Objective-C++ | 198 | Implements `GADRewardBasedVideoAdDelegate` (legacy style) to coordinate rewarded video actions on iOS. |
| `Platform/Ios/QtAdmobRewardVideoDelegateImpl.h` | C++ | 38 | Bridge representation for C++/Objective-C boundary on iOS Rewarded video ads. |
| `Platform/Ios/Info.plist` | XML | 44 | Project configuration profile containing GADApplicationIdentifier to supply AdMob Application ID on iOS execution. |

---

## 3. Modularization Assessment

- **File Sizes**: The majority of implementation files conform perfectly to size constraints (<200 LOC per file in C++ headers and delegates, around 200-300 LOC for cpp implementation files).
- **Core Activity Complexity**: The largest file in the repository is `QtAdMobActivity.java` (~511 LOC). Because Java integration with Qt on Android relies on subclassing the main `QtActivity`, consolidating banner, interstitial, and rewarded features into this single activity ensures JNI methods can resolve their native bindings reliably against a single activity instance. This layout represents standard practice for older Qt-Android JNI structures.

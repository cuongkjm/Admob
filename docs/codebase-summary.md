# Codebase Summary

This document maps the current QtAdMob Qt 6 CMake codebase.

## 1. Directory Structure

```text
.
├── CMakeLists.txt
├── ActiveRegistry.h
├── QmlBanner.h / QmlBanner.cpp
├── QmlInterstitialAd.h / QmlInterstitialAd.cpp
├── QmlRewardedVideoAd.h / QmlRewardedVideoAd.cpp
├── Platform/
│   ├── Android/
│   │   ├── proguard-rules.pro
│   │   └── src/com/qtadmob/
│   │       ├── AdMobBanner.java
│   │       ├── AdMobController.java
│   │       ├── AdMobInterstitial.java
│   │       └── AdMobRewardedVideo.java
│   └── Ios/
│       ├── Info.plist
│       ├── QtAdmobBannerIosDelegate.*
│       ├── QtAdmobInterstitialIosDelegate.*
│       └── QtAdmobRewardVideoDelegate.*
└── docs/
```

## 2. Responsibility Matrix

| Path | Responsibility |
| --- | --- |
| `CMakeLists.txt` | Defines `QtAdMob::qtadmob`, Qt 6 dependencies, Android package helper, and iOS framework linkage. |
| `ActiveRegistry.h` | Tracks live Android wrapper pointers to block late JNI callbacks after destruction. |
| `QmlBanner.*` | QML banner API, property state, Android/iOS bridge calls, desktop no-op behavior. |
| `QmlInterstitialAd.*` | QML interstitial API, lifecycle signals, Android/iOS bridge calls, desktop no-op behavior. |
| `QmlRewardedVideoAd.*` | QML rewarded API, reward/lifecycle signals, Android/iOS bridge calls, desktop no-op behavior. |
| `Platform/Android/src/com/qtadmob/` | Java AdMob helpers using modern load/show APIs and native-pointer callbacks. |
| `Platform/Android/proguard-rules.pro` | Keeps JNI classes and native method names safe during release minification. |
| `Platform/Ios/` | Objective-C++ delegates for Google Mobile Ads iOS SDK integration. |

## 3. Platform Summary

| Platform | Code Path |
| --- | --- |
| Android | Real AdMob via Java/JNI and Google Mobile Ads Android SDK. |
| iOS | Real AdMob via Objective-C++ and Google Mobile Ads iOS SDK. |
| Windows | Compiles public API with no-op ad methods. |
| macOS desktop | Compiles public API with no-op ad methods. |
| Linux | Compiles public API with no-op ad methods. |

## 4. Build Model

The repository is intended to be consumed by `add_subdirectory(path/to/Admob)` or `FetchContent_MakeAvailable(QtAdMob)`. It avoids QMake, absolute local paths, and parent-project global CMake settings.

## Unresolved Questions
- None.

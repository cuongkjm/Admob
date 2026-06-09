# Qt AdMob Integration Library

A cross-platform C++ and QML library for integrating Google AdMob into Qt 6 mobile applications on Android and iOS.

---

## Supported Ad Formats

- **Banner Ads (`QmlBanner`)**: Rectangular, docked, or custom positioned overlay ads.
- **Interstitial Ads (`QmlInterstitialAd`)**: Full-screen interstitial ads with lifecycle callback states.
- **Rewarded Video Ads (`QmlRewardedVideoAd`)**: Rewarded ads that notify QML after users earn rewards.

---

## Requirements

- Qt 6 with CMake.
- Android: Google Mobile Ads SDK available to the Android Gradle build.
- iOS: Google Mobile Ads SDK installed through Swift Package Manager, CocoaPods, or a local `GoogleMobileAds.framework` / `GoogleMobileAds.xcframework` path.
- Desktop: Windows, macOS desktop, and Linux compile with no-op ad methods because Google AdMob has no desktop SDK.

---

## Platform Behavior

| Platform | Behavior | Extra SDK Required |
| --- | --- | --- |
| Android | Real AdMob integration through Java/JNI | Android SDK/NDK + Google Mobile Ads Android SDK |
| iOS | Real AdMob integration through Objective-C++ | Google Mobile Ads iOS SDK |
| Windows | Compiles, ad methods are no-op | None |
| macOS desktop | Compiles, ad methods are no-op | None |
| Linux | Compiles, ad methods are no-op | None |

Desktop no-op methods do not emit fake success callbacks.

---

## CMake Integration

Add this repository as a CMake submodule or FetchContent dependency, then link the target:

```cmake
add_subdirectory(path/to/Admob)

qt_add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE QtAdMob::qtadmob)

qtadmob_configure_android_target(myapp)
```

`qtadmob_configure_android_target()` is a no-op outside Android. On Android it provides the Java package sources and `proguard-rules.pro` to Qt Android deployment through `QT_ANDROID_PACKAGE_SOURCE_DIR`.

Android apps still need an AdMob application id in their manifest metadata:

```xml
<meta-data
    android:name="com.google.android.gms.ads.APPLICATION_ID"
    android:value="ca-app-pub-xxxxxxxxxxxxxxxx~yyyyyyyyyy" />
```

No custom `QtActivity` subclass is required.

## Google Mobile Ads iOS SDK

Get the iOS SDK from Google's official Google Mobile Ads iOS documentation: https://developers.google.com/admob/ios/quick-start. This library is validated with manual Google Mobile Ads iOS SDK `13.5.0`. Use one of these app-level integration paths:

- **Swift Package Manager**: add `https://github.com/googleads/swift-package-manager-google-mobile-ads.git` in Xcode.
- **CocoaPods**: add `pod 'Google-Mobile-Ads-SDK'` to the app `Podfile`.
- **Local framework**: use downloaded `GoogleMobileAds.framework` or `GoogleMobileAds.xcframework` for direct CMake builds.

For direct iOS CMake builds, pass a local path when it is not already provided by the app build system. The path can be the directory containing `GoogleMobileAds.framework`, the framework itself, or `GoogleMobileAds.xcframework`:

```cmake
set(GOOGLE_MOBILE_ADS_IOS_ROOT "/path/to/GoogleMobileAds.xcframework")
```

If the SDK is missing, iOS builds should fail clearly at compile/link time instead of silently disabling real ads.

Current iOS bridge uses Google Mobile Ads iOS SDK `13.5.0` APIs: `GADInterstitialAd`, `GADRewardedAd`, `GADFullScreenContentDelegate`, and `GADBannerViewDelegate`.

---

## Validation Status

Validated builds from current implementation work:

- macOS desktop CMake build passes with no-op ad methods.
- Android `arm64-v8a` CMake build passes with Qt 6 JNI code and `com.qtadmob` package sources.
- iOS CMake build passes with `GOOGLE_MOBILE_ADS_IOS_ROOT=/Users/cuongkjm/cuongkjm/Apps/GoogleMobileAdsSdkiOS-13.5.0`.

Known non-blocking iOS warnings remain in `Platform/Ios/QtAdmobBannerIosDelegate.mm` for deprecated `UIApplication.windows` and `statusBarFrame`.

---

## Quick Start (QML Declarative Syntax)

### Banner Ad Example

```qml
import QtQuick 2.12
import AdMob 1.0

Item {
    width: 640
    height: 480

    QmlBanner {
        id: banner
        unitId: "ca-app-pub-3940256099942544/6300978111"
        bannerSize: QmlBanner.BANNER
        visible: true
        x: 0
        y: parent.height - height

        Component.onCompleted: loadBanner()

        onBannerLoaded: console.log("Banner loaded")
        onBannerFailedToLoad: console.log("Failed to load banner: " + errorCode)
    }
}
```

### Rewarded Video Example

```qml
import QtQuick 2.12

Item {
    QmlRewardedVideoAd {
        id: rewardedVideo
        unitId: "ca-app-pub-3940256099942544/5224354917"

        onRewardedVideoAdLoaded: show()
        onRewarded: console.log("Grant reward")
    }

    Component.onCompleted: rewardedVideo.loadRewardedVideoAd()
}
```

---

## Project Structure

```
.
├── CMakeLists.txt
├── QmlBanner.h / .cpp
├── QmlInterstitialAd.h / .cpp
├── QmlRewardedVideoAd.h / .cpp
├── ActiveRegistry.h
├── Platform/
│   ├── Android/
│   │   ├── proguard-rules.pro
│   │   └── src/com/qtadmob/
│   └── Ios/
└── docs/
```

---

## Technical Documentation Reference

1. **[Product Requirements & Overview](./docs/project-overview-pdr.md)**: Features, requirements, and target goals.
2. **[System Architecture](./docs/system-architecture.md)**: Layered C++, Android JNI, and iOS ObjC++ bridge architecture.
3. **[Code Standards & JNI Protocols](./docs/code-standards.md)**: ARC/no-ARC rules, JNI reference management, and formatting.
4. **[Codebase Reference Summary](./docs/codebase-summary.md)**: Code files overview and responsibility matrix.
5. **[Development Roadmap](./docs/project-roadmap.md)**: Current features and upcoming updates.
6. **[Design Principles](./docs/design-system/design-principles.md)**: Declarative bindings and non-blocking asynchronous APIs.

---

## Demos & Community References

- **Demo Sandbox Application**: [cuongkjm/TestAdmob](https://github.com/cuongkjm/TestAdmob)
- **Issue Tracker & Feature Requests**: [GitHub Issues](https://github.com/cuongkjm/Admob/issues)

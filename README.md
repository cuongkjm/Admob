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

For direct iOS CMake builds, pass the local Google Mobile Ads framework root when it is not already provided by the app build system:

```cmake
set(GOOGLE_MOBILE_ADS_IOS_ROOT "/path/to/GoogleMobileAds.xcframework")
```

Swift Package Manager or CocoaPods integration in the app target is usually easier than manually managing the framework path.

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

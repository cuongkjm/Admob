# Deployment & Integration Guide

This guide covers QtAdMob integration as a Qt 6 CMake submodule for Android, iOS, and desktop builds.

## 1. CMake Setup

Add the repository to your app build, link the target, then call the platform helpers on your Qt app target:

```cmake
add_subdirectory(path/to/Admob)

qt_add_executable(myapp
    main.cpp
    qml.qrc
)

target_link_libraries(myapp PRIVATE QtAdMob::qtadmob)

if(ANDROID)
    set_property(TARGET myapp PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
endif()

if(IOS)
    set(QT_NO_SET_DEFAULT_IOS_LAUNCH_SCREEN ON)
    set(QTADMOB_IOS_APPLICATION_ID "ca-app-pub-xxxxxxxxxxxxxxxx~yyyyyyyyyy")
endif()

qtadmob_configure_android_target(myapp)
qtadmob_configure_ios_target(myapp)
```

`qtadmob_configure_android_target()` is a no-op outside Android. On Android it merges QtAdMob Java sources and `proguard-rules.pro` into the app `QT_ANDROID_PACKAGE_SOURCE_DIR`. If the app already has an `android/` package source directory, set it before calling the helper.

`qtadmob_configure_ios_target()` is a no-op outside iOS Xcode builds. On iOS it requires `QTADMOB_IOS_APPLICATION_ID` and writes `GADApplicationIdentifier` into the generated app `Info.plist`.

## 2. Platform Behavior

| Platform | Behavior | Required SDK |
| --- | --- | --- |
| Android | Real ads through Java/JNI | Android SDK/NDK + Google Mobile Ads Android SDK |
| iOS | Real ads through Objective-C++ | Google Mobile Ads iOS SDK |
| Windows | Compile no-op | Qt 6 only |
| macOS desktop | Compile no-op | Qt 6 only |
| Linux | Compile no-op | Qt 6 only |

Desktop no-op methods keep shared QML source compilable and do not emit fake success callbacks.

## 3. Android Deployment

### AndroidManifest.xml

Add the AdMob application id inside the app manifest:

```xml
<meta-data
    android:name="com.google.android.gms.ads.APPLICATION_ID"
    android:value="ca-app-pub-xxxxxxxxxxxxxxxx~yyyyyyyyyy" />
```

The working TestAdmob sample uses Google sample app id `ca-app-pub-3940256099942544~3347511713`. No custom `QtActivity` subclass is required. QtAdMob provides helper classes under `com.qtadmob`.

### Gradle Dependency

Ensure the app Android build includes Google Mobile Ads SDK:

```groovy
dependencies {
    implementation 'com.google.android.gms:play-services-ads:23.+'
}
```

Use the version required by your app policy and Google Play requirements.

## 4. iOS Deployment

Set the app id before calling the helper:

```cmake
set(QTADMOB_IOS_APPLICATION_ID "ca-app-pub-3940256099942544~1458002511")
qtadmob_configure_ios_target(myapp)
```

The helper generates an app `Info.plist` with:

```xml
<key>GADApplicationIdentifier</key>
<string>ca-app-pub-xxxxxxxxxxxxxxxx~yyyyyyyyyy</string>
```

If your app owns its launch screen, set `QT_NO_SET_DEFAULT_IOS_LAUNCH_SCREEN ON` before finalizing the Qt target. The working TestAdmob sample does this before `qt_finalize_executable()`.

Install Google Mobile Ads iOS SDK using one of these official paths. This library is validated with manual Google Mobile Ads iOS SDK `13.5.0`.

- Swift Package Manager: `https://github.com/googleads/swift-package-manager-google-mobile-ads.git`
- CocoaPods: `pod 'Google-Mobile-Ads-SDK'`
- Manual/local framework: downloaded `GoogleMobileAds.framework` or `GoogleMobileAds.xcframework`

For direct CMake builds where the app does not provide the SDK, pass the directory containing `GoogleMobileAds.framework`, the framework itself, or `GoogleMobileAds.xcframework`:

```cmake
set(GOOGLE_MOBILE_ADS_IOS_ROOT "/path/to/GoogleMobileAds.xcframework")
```

Missing SDK should fail iOS compile/link clearly because real iOS ads require Google's framework.

Current iOS bridge is validated against Google Mobile Ads iOS SDK `13.5.0`. Non-blocking compile warnings remain for deprecated `UIApplication.windows` and `statusBarFrame` in `Platform/Ios/QtAdmobBannerIosDelegate.mm`.

## 5. QML Usage

Register QML types in C++ before loading QML:

```cpp
qmlRegisterType<QmlBanner>("AdMob", 1, 0, "QmlBanner");
qmlRegisterType<QmlInterstitialAd>("AdMob", 1, 0, "QmlInterstitialAd");
qmlRegisterType<QmlRewardedVideoAd>("AdMob", 1, 0, "QmlRewardedVideoAd");
```

Import and use the objects from QML:

```qml
import QtQuick
import QtQuick.Controls
import AdMob 1.0

Item {
    readonly property bool isIos: Qt.platform.os === "ios"
    readonly property string bannerId: isIos ? "ca-app-pub-3940256099942544/2934735716" : "ca-app-pub-3940256099942544/6300978111"
    readonly property string interstitialId: isIos ? "ca-app-pub-3940256099942544/4411468910" : "ca-app-pub-3940256099942544/1033173712"
    readonly property string rewardedId: isIos ? "ca-app-pub-3940256099942544/1712485313" : "ca-app-pub-3940256099942544/5224354917"

    QmlBanner { id: banner; unitId: bannerId; bannerSize: QmlBanner.BANNER; visible: true; testDeviceId: "41E647017EBEBB0650DAE627391B7A43" }
    QmlInterstitialAd { id: interstitial; unitId: interstitialId; testDeviceId: banner.testDeviceId; onInterstitialAdLoaded: showInterstitial.enabled = true }
    QmlRewardedVideoAd { id: rewarded; unitId: rewardedId; testDeviceId: banner.testDeviceId; onRewardedVideoAdLoaded: showRewarded.enabled = true; onRewarded: console.log("Grant reward") }

    Button { id: showInterstitial; enabled: false; text: "Show interstitial"; onClicked: { enabled = false; interstitial.showInterstitialAd() } }
    Button { id: showRewarded; enabled: false; text: "Show rewarded"; onClicked: { enabled = false; rewarded.show() } }

    Component.onCompleted: {
        banner.loadBanner()
        interstitial.loadInterstitialAd()
        rewarded.loadRewardedVideoAd()
    }
}
```

Use Google sample ad ids during development. Load each ad on startup or before needed. Show interstitial/rewarded only after loaded callbacks. Set `testDeviceId` to your device id for test traffic.

Desktop builds keep shared QML compilable. Ad methods are no-op and do not emit fake success callbacks.

## 6. Validation Commands

### macOS Desktop

```bash
cmake -S . -B build/macos -DCMAKE_PREFIX_PATH="$HOME/Qt/6.10.0/macos"
cmake --build build/macos
```

### Android arm64

```bash
cmake -S . -B build/android-arm64 \
  -DCMAKE_TOOLCHAIN_FILE="$HOME/Qt/6.10.0/android_arm64_v8a/lib/cmake/Qt6/qt.toolchain.cmake" \
  -DQT_HOST_PATH="$HOME/Qt/6.10.0/macos" \
  -DANDROID_SDK_ROOT="/Users/cuongkjm/Qt/android_sdk" \
  -DANDROID_NDK_ROOT="/Users/cuongkjm/Qt/android_sdk/ndk/android-ndk-r28c"
cmake --build build/android-arm64
```

### iOS

```bash
cmake -S . -B build/ios \
  -DCMAKE_TOOLCHAIN_FILE="$HOME/Qt/6.9.3/ios/lib/cmake/Qt6/qt.toolchain.cmake" \
  -DQT_HOST_PATH="$HOME/Qt/6.9.3/macos" \
  -DGOOGLE_MOBILE_ADS_IOS_ROOT="/path/to/GoogleMobileAds.xcframework"
cmake --build build/ios
```

## Unresolved Questions
- None.

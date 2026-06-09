# Deployment & Integration Guide

This guide covers QtAdMob integration as a Qt 6 CMake submodule for Android, iOS, and desktop builds.

## 1. CMake Setup

Add the repository to your app build and link the target:

```cmake
add_subdirectory(path/to/Admob)

qt_add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE QtAdMob::qtadmob)

qtadmob_configure_android_target(myapp)
```

`qtadmob_configure_android_target()` is a no-op outside Android. On Android it sets `QT_ANDROID_PACKAGE_SOURCE_DIR` so Qt deployment sees the Java sources and `proguard-rules.pro`.

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

No custom `QtActivity` subclass is required. QtAdMob provides Java helper classes under `com.qtadmob`.

### Gradle Dependency

Ensure the app Android build includes Google Mobile Ads SDK:

```groovy
dependencies {
    implementation 'com.google.android.gms:play-services-ads:23.+'
}
```

Use the version required by your app policy and Google Play requirements.

## 4. iOS Deployment

Add `GADApplicationIdentifier` to the app `Info.plist`:

```xml
<key>GADApplicationIdentifier</key>
<string>ca-app-pub-xxxxxxxxxxxxxxxx~yyyyyyyyyy</string>
```

Install Google Mobile Ads iOS SDK using one of these official paths. This library is validated with manual Google Mobile Ads iOS SDK `13.5.0`.

- Swift Package Manager: `https://github.com/googleads/swift-package-manager-google-mobile-ads.git`
- CocoaPods: `pod 'Google-Mobile-Ads-SDK'`
- Manual/local framework: downloaded `GoogleMobileAds.framework` or `GoogleMobileAds.xcframework`

For direct CMake builds where the app does not provide the SDK, pass the directory containing `GoogleMobileAds.framework`, the framework itself, or `GoogleMobileAds.xcframework`:

```cmake
set(GOOGLE_MOBILE_ADS_IOS_ROOT "/path/to/GoogleMobileAds.xcframework")
```

Missing SDK should fail iOS compile/link clearly because real iOS ads require Google's framework.

## 5. Validation Commands

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

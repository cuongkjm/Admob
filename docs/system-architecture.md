# System Architecture

QtAdMob exposes one Qt 6 CMake library target that bridges QML ad components to native mobile AdMob SDKs while keeping desktop builds compilable as no-op.

## 1. Layers

1. **QML API Layer**: `QmlBanner`, `QmlInterstitialAd`, and `QmlRewardedVideoAd` expose properties, slots, and signals.
2. **C++ Wrapper Layer**: Stores QML-facing state, routes platform calls, and protects callback lifetime.
3. **Native Mobile Layer**: Android uses Java/JNI helpers; iOS uses Objective-C++ delegates.
4. **Desktop No-Op Layer**: Windows, macOS desktop, and Linux compile the same API without mobile SDK dependencies.

## 2. Architecture Diagram

```text
QML components
  │
  ▼
C++ wrappers: QmlBanner / QmlInterstitialAd / QmlRewardedVideoAd
  ├─ Android: QJniObject → com.qtadmob Java helpers → Google Mobile Ads Android SDK
  ├─ iOS: Objective-C++ delegates → Google Mobile Ads iOS SDK
  └─ Desktop: no-op methods, stored properties, zero ad SDK linkage
```

## 3. Android Bridge

Android uses Qt 6 `QJniObject` and native callbacks that include a `jlong nativePointer`.

- Java helpers live under `Platform/Android/src/com/qtadmob/`.
- Each Java ad object stores its C++ wrapper pointer.
- Native callbacks validate the pointer with `ActiveRegistry`.
- Signals are marshaled back to Qt using `QMetaObject::invokeMethod(..., Qt::QueuedConnection)`.
- Java `destroy()` clears the native pointer before queued UI cleanup.

This avoids legacy singleton routing and lets multiple QML ad objects exist independently.

## 4. iOS Bridge

The iOS path compiles Objective-C++ `.mm` delegate files only for iOS builds.

- C++ wrappers own delegate implementation objects.
- Delegates call back to the owning wrapper instance.
- Destructors clear delegate back-pointers before deletion.
- Google Mobile Ads SDK must be provided by SPM, CocoaPods, or `GOOGLE_MOBILE_ADS_IOS_ROOT`.
- Interstitial and rewarded delegates use SDK `13.5.0` `GADInterstitialAd`, `GADRewardedAd`, and `GADFullScreenContentDelegate` APIs.
- Banner delegate uses `GADBannerViewDelegate`; deprecated `UIApplication.windows` and `statusBarFrame` warnings are known and non-blocking.

## 5. Desktop Behavior

AdMob has no official desktop SDK. QtAdMob keeps the public API available on Windows, macOS desktop, and Linux so shared QML source compiles.

- Setters update local Qt properties.
- Width/height return `0` when no native banner exists.
- Load/show methods return without side effects.
- No fake loaded/rewarded signals are emitted.

## 6. CMake Submodule Design

- Consumers link `QtAdMob::qtadmob`.
- The backing target is `qtadmob`.
- `AUTOMOC` handles `Q_OBJECT` wrappers.
- `OBJCXX` and iOS sources are enabled only when `IOS` is true.
- Android package sources are configured only through `qtadmob_configure_android_target()`.

## 7. Validation State

- macOS desktop, Android `arm64-v8a`, and iOS CMake builds passed in previous validation.
- iOS validation used `/Users/cuongkjm/cuongkjm/Apps/GoogleMobileAdsSdkiOS-13.5.0` as `GOOGLE_MOBILE_ADS_IOS_ROOT`.

## Unresolved Questions
- None.

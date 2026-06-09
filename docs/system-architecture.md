# System Architecture

This document describes the layered system architecture, data flow, thread synchronization, and bridging mechanisms that make cross-platform AdMob integration functional inside Qt.

## 1. High-Level Layered Architecture

The library splits responsibilities into three distinct layers:
1. **QML Presentation Layer (Declarative)**: Exposes components and control properties directly to developers.
2. **C++ Wrapper Layer (Logic & Coordination)**: Exposes Qt properties, handles signals, and acts as the bridge orchestrator.
3. **Platform Native Layer (System OS)**: Runs the official Google Mobile Ads SDK inside Java/JNI (Android) or Objective-C++ (iOS).

### Visual Overview

```
┌─────────────────────────────────────────────────────────┐
│                QML Presentation Layer                   │
│  - QmlBanner { x: 0; y: 0; unitId: "..." }              │
│  - QmlInterstitialAd / QmlRewardedVideoAd               │
└────────────────────────────┬────────────────────────────┘
                             │ (Q_PROPERTY, Slots, Signals)
┌────────────────────────────▼────────────────────────────┐
│                 C++ Wrapper Layer                       │
│  - QmlBanner.cpp / QmlInterstitialAd.cpp                │
│  - QmlRewardedVideoAd.cpp                               │
└──────────────┬───────────────────────────┬──────────────┘
               │ (JNI Call via             │ (Obj-C++ direct
               │  QAndroidJniObject)       │  message sending)
               ▼                           ▼
┌──────────────────────────────┐ ┌────────────────────────┐
│      Android Native          │ │       iOS Native       │
│  - QtAdMobActivity.java      │ │  - QtAdmob*Delegate.h │
│  - AdView, InterstitialAd    │ │  - QtAdmob*Delegate.mm│
│  - RewardedVideoAd           │ │  - GADBannerView       │
│  - JNI Native Callbacks      │ │  - GADRequest          │
└──────────────┬───────────────┘ └───────────┬────────────┘
               │                             │
┌──────────────▼───────────────┐ ┌───────────▼────────────┐
│ Google Play Mobile Ads SDK   │ │ Google Mobile Ads SDK  │
└──────────────────────────────┘ └────────────────────────┘
```

---

## 2. Bridging Mechanisms

Because Qt operates in a standard C++ context, and mobile devices operate in either Java virtual machines (JVM) or Objective-C runtime contexts, specific bridges have been implemented.

### Android: JNI Bridge Flow
- **Outgoing Calls**: C++ calls JNI methods on the underlying Activity using Qt's `QAndroidJniObject` utility class.
  ```cpp
  // Example C++ Outgoing call
  QAndroidJniObject::callStaticMethod<void>(
      "com/gmail/manhcuong5993/QtAdMobActivity",
      "InitializeBanner"
  );
  ```
- **Incoming Callbacks (Signals)**: Java uses declared `native` methods to trigger callbacks. C++ exports these native symbols. When AdMob triggers an event in Java, the activity calls the native JNI method, which immediately maps to the active `QmlBanner::Instances()` or equivalent ad singleton to emit the C++ Qt Signal.
  ```java
  // Java Native method definition
  private static native void BannerLoaded();
  ```

### iOS: Objective-C++ Bridge Flow
- **Outgoing Calls**: Implementation files use Objective-C++ (`.mm` format), allowing normal C++ classes to instantiate and hold a `void*` pointer referring to an Objective-C instance (`QtAdmobBannerIosDelegate`). Message passing triggers the target selector directly.
  ```cpp
  // Outgoing Obj-C delegate call from C++
  QtAdmobBannerIosDelegate* delegate = (QtAdmobBannerIosDelegate*)self;
  [delegate loadBanner];
  ```
- **Incoming Callbacks (Signals)**: Delegates implement official SDK protocols (like `GADBannerViewDelegate`). When the iOS SDK fires an event, the delegate calls corresponding C++ methods directly on the `QtAdmobBannerIosDelegateImpl` bridge instance, which emits the signals on the original `QmlBanner` object.

---

## 3. Singleton and Instances Design

- High-level control objects like `QmlBanner`, `QmlInterstitialAd`, and `QmlRewardedVideoAd` implement static `Instances()` methods:
  ```cpp
  static QmlBanner* Instances();
  ```
- This design pattern ensures that when platform-specific background operations complete (whether a JNI callback from Java or a delegate callback from Objective-C++), the native library has a reliable global object instance reference to invoke. This allows events to bubble back up to the exact active QML component declared in the user's interface tree.
- The `Instances()` pointer is typically cached during the object constructor and set back to `nullptr` upon destruction.

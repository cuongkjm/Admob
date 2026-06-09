# Code Standards & Best Practices

This document outlines the coding standards, patterns, and integration guidelines established for the Qt AdMob integration project.

## 1. General Principles

Developers working on this repository should adhere to three core design guidelines:
- **YAGNI (You Aren't Gonna Need It)**: Implement only the functionality that is strictly requested or necessary. Do not write anticipatory or speculative abstractions.
- **KISS (Keep It Simple, Stupid)**: Minimize complexity. Write clean, direct code that is easy to read, debug, and maintain.
- **DRY (Don't Repeat Yourself)**: Extract common logic, reuse coordinate offset handlers, and synchronize lifecycle events consistently across Android and iOS platforms.

---

## 2. JNI Rules & Guidelines (Android Integration)

Android integration utilizes Qt Android Extras and Java Native Interface (JNI). To prevent memory leaks, application crashes, or UI threading exceptions, follow these rules:

1. **Keep UI Actions on UI Thread**:
   - AdMob SDK functions (such as modifying layouts, creating `AdView`, and updating visibility) *must* run on Android's main UI thread.
   - Use `runOnUiThread()` inside Java methods:
     ```java
     runOnUiThread(new Runnable() {
         public void run() {
             // Safe UI / AdMob operations
         }
     });
     ```

2. **C++ Native Method Signatures**:
   - Native declarations in `QtAdMobActivity.java` must match C++ export signatures.
   - Ensure JNI method names match perfectly:
     ```java
     private static native void BannerLoaded();
     ```
   - In C++ source code, register or define these native hooks properly under `extern "C"` blocks or static JNI bindings to bridge them into signals.

3. **Safe Reference Management**:
   - Clean up global references when shutting down components.
   - Guard against null objects when invoking JNI method calls via `QAndroidJniObject`.

---

## 3. Objective-C++ Integration (iOS Integration)

On iOS, C++ code (`Qml*.cpp`) cannot directly talk to Objective-C classes. We bridge them using **Objective-C++ (`.mm`) files** and intermediate structural bridge blocks:

1. **No-ARC compilation constraint**:
   - The project is compiled with `-fno-objc-arc` (Automatic Reference Counting disabled) in `Admob.pri`.
   - You **MUST** manage memory manually in Objective-C++ files (`.mm`).
   - Call `[super dealloc]` or `[obj release]` where appropriate inside delegates to avoid system memory leaks:
     ```objc
     -(void) dealloc {
         [super dealloc];
         _handler = nil;
         _request = nil;
         _bannerView = nil;
     }
     ```

2. **Pointer Bridging (`void* self`)**:
   - Standard C++ classes store a `void*` pointer representing the Objective-C delegate object (`self`).
   - Use typecasting inside `.mm` files to trigger native Objective-C selectors safely:
     ```cpp
     QtAdmobBannerIosDelegate* delegate = (QtAdmobBannerIosDelegate*)self;
     [delegate loadBanner];
     ```

3. **Accessing the Root View Controller**:
   - Qt controls the underlying window structure. Retrieve the active `rootViewController` by enumerating key windows in the application delegate:
     ```objc
     UIApplication *application = [UIApplication sharedApplication];
     NSArray *windows = [application windows];
     UIViewController * __block rootViewController = nil;
     [windows enumerateObjectsUsingBlock:^(UIWindow * _Nonnull window, NSUInteger idx, BOOL * _Nonnull stop) {
         rootViewController = [window rootViewController];
         *stop = (rootViewController != nil);
     }];
     ```

---

## 4. File Size & Modularization Guidelines

To keep the repository highly maintainable:
- **Maximum File Size**: Maintain files under **200-300 lines of code** whenever possible.
- **Single Responsibility Principle**: Ensure each ad type has its own isolated handler (e.g., `QmlBanner`, `QmlInterstitialAd`, `QmlRewardedVideoAd`) that handles setup, loading, presentation, and teardown independently.
- **No Shared Native State**: Avoid sharing global ad-state variables. Instead, use static `Instances()` methods or object pointers to route specific event responses to their respective target QML handles.

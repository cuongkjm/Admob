# Code Standards & Best Practices

## 1. Core Principles

- Apply YAGNI, KISS, and DRY.
- Keep platform code behind explicit compile guards.
- Prefer direct, readable wrappers over speculative abstractions.
- Preserve one public API across Android, iOS, Windows, macOS desktop, and Linux.

## 2. CMake Standards

- Use Qt 6 with CMake only.
- Expose `QtAdMob::qtadmob` as the public target.
- Do not use QMake `.pri` or `.pro` integration.
- Do not set global `CMAKE_CXX_STANDARD`; use `target_compile_features`.
- Do not use `CMAKE_SOURCE_DIR`, `PROJECT_SOURCE_DIR`, or local absolute paths for library sources.
- Keep `AUTOMOC` enabled for `Q_OBJECT` wrappers.
- Enable `OBJCXX` only for iOS.

## 3. Android JNI Standards

- Use Qt 6 `QJniObject` and `QJniEnvironment`.
- Keep Java helpers under `com.qtadmob`.
- Pass `long nativePointer` from Java to every native callback.
- Validate callbacks with `ActiveRegistry` before using native pointers.
- Dispatch Qt signals through `QMetaObject::invokeMethod(..., Qt::QueuedConnection)`.
- Java UI and AdMob SDK work must run through `Activity.runOnUiThread()`.
- Keep `proguard-rules.pro` aligned with JNI class and native method names.

## 4. iOS Objective-C++ Standards

- Compile `.mm` files only for iOS targets.
- Keep Google Mobile Ads iOS SDK paths configurable, never hardcoded to a local machine.
- Support SDK acquisition through Swift Package Manager, CocoaPods, or `GOOGLE_MOBILE_ADS_IOS_ROOT`.
- Clear delegate back-pointers before deleting C++ wrapper-owned delegate objects.
- Keep `-ObjC` link option for Google Mobile Ads categories.

## 5. Desktop No-Op Standards

- Windows, macOS desktop, and Linux must compile without Android/iOS SDK headers.
- Setters should update local Qt property state.
- Load/show methods should return safely without side effects.
- Do not emit fake loaded, rewarded, or success signals on desktop.
- Do not add mock ads or simulated SDK behavior.

## 6. File Size & Structure

- Keep files focused by ad type or platform role.
- Consider splitting code files above 200 lines when there is a natural boundary.
- Do not modularize markdown, config, or generated build artifacts just for line count.

## Unresolved Questions
- None.

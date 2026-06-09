# Design Principles

## 1. Declarative QML API

QtAdMob should feel like a normal QML API even though real ads are native mobile SDK views.

```qml
QmlBanner {
    unitId: "ca-app-pub-3940256099942544/6300978111"
    bannerSize: QmlBanner.BANNER
    visible: true
    x: 0
    y: parent.height - height
}
```

Properties store developer intent. Platform layers decide whether that intent creates real native ads or becomes desktop no-op state.

## 2. One Source Tree Across OS Families

Apps should be able to keep one shared Qt/QML source tree for Android, iOS, Windows, macOS desktop, and Linux.

- Android and iOS call real Google Mobile Ads SDKs.
- Desktop platforms compile the same API with no-op ad methods.
- Desktop no-op behavior must not simulate successful ads or rewards.

## 3. Native Callback Ownership

Native callbacks should route to the exact wrapper instance that created the platform ad object.

- Android Java stores `nativePointer` per ad object.
- C++ validates native pointers with `ActiveRegistry`.
- iOS delegates keep explicit back-pointers to their owning wrapper.
- Destructors clear native links before deleting wrapper-owned state.

## 4. Non-Blocking Operations

Ad loading and presentation must not block the Qt UI thread.

- Android UI work runs on Android UI thread.
- JNI callbacks queue Qt signal emission back to Qt.
- iOS delegates report through platform callback methods.
- Desktop no-op calls return immediately.

## Unresolved Questions
- None.

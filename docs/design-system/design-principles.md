# Design Principles

This document outlines the design principles underlying the Qt AdMob integration API.

## 1. Declarative QML Design

A core goal of this library is to seamlessly blend native mobile ad integrations into QML's declarative, property-binding-centric philosophy.

- **Reactive Property Bindings**: Banner properties such as position, visibility, or size are exposed as reactive Qt Properties (`Q_PROPERTY`). Developers don't need to manually invoke setters across thread boundaries; standard QML layout binding handles positioning instantly:
  ```qml
  QmlBanner {
      id: banner
      unitId: "ca-app-pub-3940256099942544/6300978111"
      bannerSize: QmlBanner.BANNER
      visible: true
      x: parent.width - width
      y: parent.height - height
  }
  ```

- **Asynchronous & Non-blocking Operations**: Ad loading can take several seconds and involves expensive networking. The library keeps all API loading calls strictly asynchronous. Initiating a load action is done via explicit action slots (`loadBanner()`, `loadInterstitialAd()`), while state updates are received via reactive QML signal handlers:
  ```qml
  onBannerLoaded: {
      console.log("Banner is ready to be displayed.")
  }
  onBannerFailedToLoad: {
      console.log("Failed to fetch banner ad, error code:", errorCode)
  }
  ```

---

## 2. Singleton-Like Native Routing

Because each application typically runs a single ad unit stream or unified screen presentation context, native mobile integrations utilize singleton registration concepts:

- **Centralized Event Dispatching**: Standard `Instances()` references let native platform bridges immediately resolve the C++ handler instance to emit signal actions when background processes finish.
- **Resource Conservation**: Consolidating platform components avoids memory leaks, duplicated web renderers, and overlapping ad layers.

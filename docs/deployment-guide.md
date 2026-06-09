# Deployment & Integration Guide

This guide details how to integrate and compile the Qt AdMob library inside your Android and iOS applications.

## 1. Project-wide Setup

Include the `Admob.pri` project inclusion file inside your main Qt project file (`.pro`):

```qmake
include($$PWD/path/to/Admob/Admob.pri)
```

Ensure your main configuration is ready for cross-platform linking:
- Add `quick` and `gui-private` to your `QT` variable.
- For Android builds, the library automatically adds the `androidextras` module.

---

## 2. Android Deployment Steps

### Step 2.1: AndroidManifest.xml Configuration
Add the AdMob Application ID metadata and network permissions inside your application tag in `AndroidManifest.xml`:

```xml
<manifest ...>
    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />

    <application android:name="org.qtproject.qt5.android.bindings.QtApplication" ...>
        <!-- Sample AdMob App ID. Replace with your actual App ID -->
        <meta-data
            android:name="com.google.android.gms.ads.APPLICATION_ID"
            android:value="ca-app-pub-3940256099942544~3347511713"/>
    </application>
</manifest>
```

### Step 2.2: Build Gradle Dependencies
In your Android build configurations (`build.gradle`), include the play-services-ads library reference:

```groovy
dependencies {
    implementation 'com.google.android.gms:play-services-ads:20.6.0' // Use an appropriate compatible version
}
```

### Step 2.3: Use Custom Activity Class
Ensure your Android configuration points to our custom activity class rather than default `QtActivity`:
- Set `android:name` in your `<activity>` tag within `AndroidManifest.xml` to `com.gmail.manhcuong5993.QtAdMobActivity`.

---

## 3. iOS Deployment Steps

On iOS, the QMake environment configures linking directly via standard system properties.

### Step 3.1: Framework & Binary Setup
The `Admob.pri` configures iOS deployment settings automatically:
- It configures standard search framework paths: `$$PWD/Platform/Ios/MobileAds`.
- It links against core Cocoa frameworks:
  - `GoogleMobileAds`
  - `GoogleAppMeasurement`
  - `GoogleUtilities`
  - `nanopb`
- Employs `-ObjC` linking configuration and disables Automatic Reference Counting specifically on library files (`-fno-objc-arc`).

Ensure you have placed the Google Mobile Ads SDK framework packages inside the designated search folder: `Platform/Ios/MobileAds/`.

### Step 3.2: Info.plist Configuration
AdMob on iOS requires the `GADApplicationIdentifier` property inside the main app bundle `Info.plist` file:

```xml
<key>GADApplicationIdentifier</key>
<string>ca-app-pub-3940256099942544~1458002511</string>
```

*Note: The bundled `Platform/Ios/Info.plist` file is pre-configured with Google's default simulator testing App ID.*

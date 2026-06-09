package com.qtadmob;

import android.app.Activity;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.RequestConfiguration;

import java.util.ArrayList;
import java.util.List;

public final class AdMobController {
    private static boolean initialized;
    private static final List<String> testDeviceIds = new ArrayList<>();

    private AdMobController() {
    }

    public static synchronized void initialize(Activity activity) {
        if (!initialized) {
            MobileAds.initialize(activity);
            initialized = true;
        }
    }

    public static synchronized void addTestDeviceId(String testDeviceId) {
        if (testDeviceId == null || testDeviceId.length() == 0 || testDeviceIds.contains(testDeviceId)) {
            return;
        }
        testDeviceIds.add(testDeviceId);
        MobileAds.setRequestConfiguration(new RequestConfiguration.Builder()
                .setTestDeviceIds(new ArrayList<>(testDeviceIds))
                .build());
    }

    public static AdRequest createRequest() {
        return new AdRequest.Builder().build();
    }
}

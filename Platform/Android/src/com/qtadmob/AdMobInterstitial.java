package com.qtadmob;

import android.app.Activity;

import com.google.android.gms.ads.AdError;
import com.google.android.gms.ads.FullScreenContentCallback;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.interstitial.InterstitialAd;
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback;

import java.util.concurrent.atomic.AtomicLong;

public class AdMobInterstitial {
    private final Activity activity;
    private final AtomicLong nativePointer;
    private InterstitialAd interstitialAd;
    private String adUnitId;

    private static native void InterstitialAdLoaded(long nativePointer);
    private static native void InterstitialAdFailedToLoad(long nativePointer, int errorCode);
    private static native void InterstitialAdOpened(long nativePointer);
    private static native void InterstitialAdLeftApplication(long nativePointer);
    private static native void InterstitialAdClosed(long nativePointer);

    public AdMobInterstitial(Activity activity, long nativePointer) {
        this.activity = activity;
        this.nativePointer = new AtomicLong(nativePointer);
        AdMobController.initialize(activity);
    }

    public void setInterstitialAdUnitId(String adUnitId) {
        this.adUnitId = adUnitId;
    }

    public void setTestDeviceId(String testDeviceId) {
        AdMobController.addTestDeviceId(testDeviceId);
    }

    public void loadInterstitialAd() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (nativePointer.get() == 0 || adUnitId == null) {
                    return;
                }
                InterstitialAd.load(activity, adUnitId, AdMobController.createRequest(), new InterstitialAdLoadCallback() {
                    @Override
                    public void onAdLoaded(InterstitialAd ad) {
                        interstitialAd = ad;
                        interstitialAd.setFullScreenContentCallback(createFullScreenCallback());
                        long pointer = nativePointer.get();
                        if (pointer != 0) InterstitialAdLoaded(pointer);
                    }

                    @Override
                    public void onAdFailedToLoad(LoadAdError error) {
                        interstitialAd = null;
                        long pointer = nativePointer.get();
                        if (pointer != 0) InterstitialAdFailedToLoad(pointer, error.getCode());
                    }
                });
            }
        });
    }

    public void showInterstitialAd() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                long pointer = nativePointer.get();
                if (pointer == 0) {
                    return;
                }
                if (interstitialAd == null || activity.isFinishing() || activity.isDestroyed()) {
                    InterstitialAdFailedToLoad(pointer, 0);
                    return;
                }
                interstitialAd.show(activity);
            }
        });
    }

    public void destroy() {
        if (nativePointer.getAndSet(0) == 0) {
            return;
        }
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (interstitialAd != null) {
                    interstitialAd.setFullScreenContentCallback(null);
                    interstitialAd = null;
                }
            }
        });
    }

    private FullScreenContentCallback createFullScreenCallback() {
        return new FullScreenContentCallback() {
            @Override
            public void onAdShowedFullScreenContent() {
                long pointer = nativePointer.get();
                if (pointer != 0) InterstitialAdOpened(pointer);
            }

            @Override
            public void onAdClicked() {
                long pointer = nativePointer.get();
                if (pointer != 0) InterstitialAdLeftApplication(pointer);
            }

            @Override
            public void onAdDismissedFullScreenContent() {
                interstitialAd = null;
                long pointer = nativePointer.get();
                if (pointer != 0) InterstitialAdClosed(pointer);
            }

            @Override
            public void onAdFailedToShowFullScreenContent(AdError error) {
                interstitialAd = null;
                long pointer = nativePointer.get();
                if (pointer != 0) InterstitialAdFailedToLoad(pointer, error.getCode());
            }
        };
    }
}

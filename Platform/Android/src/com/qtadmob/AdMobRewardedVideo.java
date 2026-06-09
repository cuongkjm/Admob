package com.qtadmob;

import android.app.Activity;

import com.google.android.gms.ads.AdError;
import com.google.android.gms.ads.FullScreenContentCallback;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.rewarded.RewardedAd;
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;
import java.util.concurrent.atomic.AtomicLong;

public class AdMobRewardedVideo {
    private final Activity activity;
    private final AtomicLong nativePointer;
    private RewardedAd rewardedAd;
    private String adUnitId;

    private static native void Rewarded(long nativePointer);
    private static native void RewardedVideoAdClosed(long nativePointer);
    private static native void RewardedVideoAdFailedToLoad(long nativePointer, int errorCode);
    private static native void RewardedVideoAdLeftApplication(long nativePointer);
    private static native void RewardedVideoAdLoaded(long nativePointer);
    private static native void RewardedVideoAdOpened(long nativePointer);
    private static native void RewardedVideoCompleted(long nativePointer);
    private static native void RewardedVideoStarted(long nativePointer);

    public AdMobRewardedVideo(Activity activity, long nativePointer) {
        this.activity = activity;
        this.nativePointer = new AtomicLong(nativePointer);
        AdMobController.initialize(activity);
    }

    public void setUnitId(String adUnitId) {
        this.adUnitId = adUnitId;
    }

    public void setTestDeviceId(String testDeviceId) {
        AdMobController.addTestDeviceId(testDeviceId);
    }

    public void loadRewardedVideoAd() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (nativePointer.get() == 0 || adUnitId == null) {
                    return;
                }
                RewardedAd.load(activity, adUnitId, AdMobController.createRequest(), new RewardedAdLoadCallback() {
                    @Override
                    public void onAdLoaded(RewardedAd ad) {
                        rewardedAd = ad;
                        rewardedAd.setFullScreenContentCallback(createFullScreenCallback());
                        long pointer = nativePointer.get();
                        if (pointer != 0) RewardedVideoAdLoaded(pointer);
                    }

                    @Override
                    public void onAdFailedToLoad(LoadAdError error) {
                        rewardedAd = null;
                        long pointer = nativePointer.get();
                        if (pointer != 0) RewardedVideoAdFailedToLoad(pointer, error.getCode());
                    }
                });
            }
        });
    }

    public void show() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (nativePointer.get() == 0 || rewardedAd == null || activity.isFinishing() || activity.isDestroyed()) {
                    return;
                }
                rewardedAd.show(activity, rewardItem -> {
                    long pointer = nativePointer.get();
                    if (pointer != 0) {
                        Rewarded(pointer);
                        RewardedVideoCompleted(pointer);
                    }
                });
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
                if (rewardedAd != null) {
                    rewardedAd.setFullScreenContentCallback(null);
                    rewardedAd = null;
                }
            }
        });
    }

    private FullScreenContentCallback createFullScreenCallback() {
        return new FullScreenContentCallback() {
            @Override
            public void onAdShowedFullScreenContent() {
                long pointer = nativePointer.get();
                if (pointer != 0) {
                    RewardedVideoAdOpened(pointer);
                    RewardedVideoStarted(pointer);
                }
            }

            @Override
            public void onAdClicked() {
                long pointer = nativePointer.get();
                if (pointer != 0) RewardedVideoAdLeftApplication(pointer);
            }

            @Override
            public void onAdDismissedFullScreenContent() {
                rewardedAd = null;
                long pointer = nativePointer.get();
                if (pointer != 0) RewardedVideoAdClosed(pointer);
            }

            @Override
            public void onAdFailedToShowFullScreenContent(AdError error) {
                rewardedAd = null;
                long pointer = nativePointer.get();
                if (pointer != 0) RewardedVideoAdFailedToLoad(pointer, error.getCode());
            }
        };
    }
}

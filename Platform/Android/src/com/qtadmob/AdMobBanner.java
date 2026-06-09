package com.qtadmob;

import android.app.Activity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.LoadAdError;

import java.util.concurrent.atomic.AtomicLong;

public class AdMobBanner {
    private final Activity activity;
    private final AtomicLong nativePointer;
    private AdView adView;
    private AdSize adSize = AdSize.BANNER;
    private String adUnitId;
    private int x;
    private int y;
    private int width;
    private int height;
    private boolean visible = true;

    private static native void BannerLoaded(long nativePointer);
    private static native void BannerFailedToLoad(long nativePointer, int errorCode);
    private static native void BannerOpened(long nativePointer);
    private static native void BannerLeftApplication(long nativePointer);
    private static native void BannerClosed(long nativePointer);

    public AdMobBanner(Activity activity, long nativePointer) {
        this.activity = activity;
        this.nativePointer = new AtomicLong(nativePointer);
        AdMobController.initialize(activity);
    }

    public void initializeBanner() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ensureAdView();
            }
        });
    }

    public void setBannerSize(final int size) {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AdSize nextSize = toAdSize(size);
                if (adView != null && adView.getAdSize() != null && !adView.getAdSize().equals(nextSize)) {
                    destroyAdView();
                }
                adSize = nextSize;
                width = adSize.getWidth();
                height = adSize.getHeight();
                ensureAdView();
            }
        });
    }

    public void setBannerUnitId(final String adUnitId) {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (AdMobBanner.this.adUnitId != null && !AdMobBanner.this.adUnitId.equals(adUnitId)) {
                    destroyAdView();
                }
                AdMobBanner.this.adUnitId = adUnitId;
                ensureAdView();
            }
        });
    }

    public void setTestDeviceId(String testDeviceId) {
        AdMobController.addTestDeviceId(testDeviceId);
    }

    public void loadBanner() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ensureAdView();
                if (adView != null) {
                    adView.loadAd(AdMobController.createRequest());
                }
            }
        });
    }

    public int getAdBannerWidth() {
        return width;
    }

    public int getAdBannerHeight() {
        return height;
    }

    public void setX(int x) {
        this.x = x;
        updatePosition();
    }

    public void setY(int y) {
        this.y = y;
        updatePosition();
    }

    public void setVisible(final boolean visible) {
        this.visible = visible;
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (adView != null) {
                    adView.setVisibility(visible ? View.VISIBLE : View.INVISIBLE);
                }
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
                destroyAdView();
            }
        });
    }

    private void ensureAdView() {
        if (nativePointer.get() == 0 || adView != null) {
            return;
        }

        adView = new AdView(activity);
        adView.setAdSize(adSize);
        if (adUnitId != null) {
            adView.setAdUnitId(adUnitId);
        }
        adView.setVisibility(visible ? View.VISIBLE : View.INVISIBLE);
        adView.setAdListener(new AdListener() {
            @Override
            public void onAdLoaded() {
                width = adView.getAdSize().getWidth();
                height = adView.getAdSize().getHeight();
                long pointer = nativePointer.get();
                if (pointer != 0) BannerLoaded(pointer);
            }

            @Override
            public void onAdFailedToLoad(LoadAdError error) {
                long pointer = nativePointer.get();
                if (pointer != 0) BannerFailedToLoad(pointer, error.getCode());
            }

            @Override
            public void onAdOpened() {
                long pointer = nativePointer.get();
                if (pointer != 0) BannerOpened(pointer);
            }

            @Override
            public void onAdClicked() {
                long pointer = nativePointer.get();
                if (pointer != 0) BannerLeftApplication(pointer);
            }

            @Override
            public void onAdClosed() {
                long pointer = nativePointer.get();
                if (pointer != 0) BannerClosed(pointer);
            }
        });

        ViewGroup content = activity.findViewById(android.R.id.content);
        content.addView(adView, new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT));
        applyPosition();
    }

    private void updatePosition() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                applyPosition();
            }
        });
    }

    private void applyPosition() {
        if (adView == null) {
            return;
        }
        float density = activity.getResources().getDisplayMetrics().density;
        adView.setX(x * density);
        adView.setY(y * density);
    }

    private void destroyAdView() {
        if (adView == null) {
            return;
        }
        ViewGroup parent = (ViewGroup) adView.getParent();
        if (parent != null) {
            parent.removeView(adView);
        }
        adView.setAdListener(null);
        adView.destroy();
        adView = null;
    }

    private AdSize toAdSize(int size) {
        switch (size) {
            case 1:
                return AdSize.FLUID;
            case 2:
                return AdSize.FULL_BANNER;
            case 3:
                return AdSize.LARGE_BANNER;
            case 4:
                return AdSize.LEADERBOARD;
            case 5:
                return AdSize.MEDIUM_RECTANGLE;
            case 6:
                return AdSize.BANNER;
            case 7:
                return AdSize.BANNER;
            case 8:
                return AdSize.WIDE_SKYSCRAPER;
            default:
                return AdSize.BANNER;
        }
    }
}

package com.gmail.manhcuong5993;

import android.os.Bundle;
import android.content.Context;
import android.view.View;
import android.view.Window;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import org.qtproject.qt5.android.bindings.QtActivity;
import org.qtproject.qt5.android.bindings.QtApplication;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.google.android.gms.ads.reward.RewardedVideoAdListener;
import com.google.android.gms.ads.reward.RewardItem;

public class QtAdMobActivity extends QtActivity implements RewardedVideoAdListener
{

    /* Banner Ads variables */
    private ViewGroup mViewGroup;
    private AdView mAdView = null;
    private int m_AdBannerWidth = 0;
    private int m_AdBannerHeight = 0;
    private int m_AdBannerPositionX = 0;
    private int m_AdBannerPositionY = 0;
    private String m_AdBannerTestDeviceId;

    /* Interstitial Ad variables */
    private InterstitialAd m_AdInterstitial = null;
    private String m_InterstitialAdTestDeviceId;

    /* Rewarded Video Ad variables */
    private RewardedVideoAd m_AdRewardedVideo = null;
    private String m_RewardedVideoAdUnitId;
    private String m_RewardedVideoTestDeviceId;

    /* Banner Ads Signals (call a function from c++) */
    private static native void BannerLoaded();
    private static native void BannerFailedToLoad(int errorCode);
    private static native void BannerOpened();
    private static native void BannerLeftApplication();
    private static native void BannerClosed();

    /* Interstitial Ads Signals (call a function from c++) */
    private static native void InterstitialAdLoaded();
    private static native void InterstitialAdFailedToLoad(int errorCode);
    private static native void InterstitialAdOpened();
    private static native void InterstitialAdLeftApplication();
    private static native void InterstitialAdClosed();

    /* Rewarded Video Ads Signals (call a function from c++) */
    private static native void Rewarded();
    private static native void RewardedVideoAdClosed();
    private static native void RewardedVideoAdFailedToLoad(int errorCode);
    private static native void RewardedVideoAdLeftApplication();
    private static native void RewardedVideoAdLoaded();
    private static native void RewardedVideoAdOpened();
    private static native void RewardedVideoCompleted();
    private static native void RewardedVideoStarted();

    /*Banner Ads functions*/
    public void InitializeBanner()
    {
        final QtAdMobActivity self = this;
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                // Confirm AdView has already been available
                if (mAdView != null) return;

                // Initialize AdView with visible display
                mAdView = new AdView(self);
                mAdView.setVisibility(View.VISIBLE);

                // Get Android Window Root View
                View view = getWindow().getDecorView().getRootView();
                if (view instanceof ViewGroup)
                {
                    mViewGroup = (ViewGroup) view;

                    // Add a FrameLayout overlay
                    //FrameLayout.LayoutParams layoutParams =
                    //    new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                    //                                 FrameLayout.LayoutParams.WRAP_CONTENT);
                    //mAdView.setLayoutParams(layoutParams);
                    mAdView.setX(m_AdBannerPositionX);
                    mAdView.setY(m_AdBannerPositionY);
                    mViewGroup.addView(mAdView);

                    mAdView.setAdListener(new AdListener()
                    {
                        // Add listener when an ad finishes loading
                        public void onAdLoaded()
                        {
                            BannerLoaded();
                        }

                        public void onAdFailedToLoad(int errorCode)
                        {
                            BannerFailedToLoad(errorCode);
                        }

                        public void onAdOpened()
                        {
                            BannerOpened();
                        }

                        public void onAdLeftApplication()
                        {
                            BannerLeftApplication();
                        }

                        public void onAdClosed()
                        {
                            BannerClosed();
                        }
                    });
                }
            }
        });
    }

    public void SetBannerSize(final int size)
    {
        final QtAdMobActivity self = this;
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                AdSize adSize = AdSize.BANNER;
                switch(size)
                {
                    case 0:
                        adSize = AdSize.BANNER;
                    break;
                    case 1:
                        adSize = AdSize.FLUID;
                    break;
                    case 2:
                        adSize = AdSize.FULL_BANNER;
                    break;
                    case 3:
                        adSize = AdSize.LARGE_BANNER;
                    break;
                    case 4:
                        adSize = AdSize.LEADERBOARD;
                    break;
                    case 5:
                        adSize = AdSize.MEDIUM_RECTANGLE;
                    break;
                    case 6:
                        adSize = AdSize.SEARCH;
                    break;
                    case 7:
                        adSize = AdSize.SMART_BANNER;
                    break;
                    case 8:
                        adSize = AdSize.WIDE_SKYSCRAPER;
                    break;
                };

                mAdView.setAdSize(adSize);
                m_AdBannerWidth = adSize.getWidth();
                m_AdBannerHeight = adSize.getHeight();
            }
        });
    }

    public void SetBannerUnitId(final String adId)
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                mAdView.setAdUnitId(adId);
            }
        });
    }

    public void SetBannerTestDeviceId(final String testDeviceId) {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                m_AdBannerTestDeviceId = testDeviceId;
            }
        });
    }

    public void LoadBanner()
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                AdRequest.Builder adRequest = new AdRequest.Builder();
                adRequest.addTestDevice(m_AdBannerTestDeviceId);
                mAdView.loadAd(adRequest.build());
            }
        });
    }

    public int GetAdBannerWidth()
    {
        return m_AdBannerWidth;
    }

    public int GetAdBannerHeight()
    {
        return m_AdBannerHeight;
    }

    public void SetX(final int x) {
        m_AdBannerPositionX = x;
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                                                                                     FrameLayout.LayoutParams.WRAP_CONTENT);
                mAdView.setLayoutParams(layoutParams);
                mAdView.setX(m_AdBannerPositionX);
                mAdView.setY(m_AdBannerPositionY);
            }
        });
    }

    public void SetY(final int y)
    {
        m_AdBannerPositionY = y;
        runOnUiThread(new Runnable()
        {
            public void run()
          {
               FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                                                                                     FrameLayout.LayoutParams.WRAP_CONTENT);
               mAdView.setLayoutParams(layoutParams);
               mAdView.setX(m_AdBannerPositionX);
               mAdView.setY(m_AdBannerPositionY);
          }
      });
    }

    public void SetVisible(final boolean visible)
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                if(visible == true)
                {
                    mAdView.setVisibility(View.VISIBLE);
                }
                else
                {
                    mAdView.setVisibility(View.INVISIBLE);
                }
            }
        });
    }

    /* Interstitial Ad functions*/
    public void InitializeInterstitialAd()
    {
        final QtAdMobActivity self = this;
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                m_AdInterstitial = new InterstitialAd(self);
                m_AdInterstitial.setAdListener(new AdListener()
                {
                    @Override
                    public void onAdClosed()
                    {
                        InterstitialAdClosed();
                    }

                    @Override
                    public void onAdFailedToLoad(int errorCode)
                    {
                        InterstitialAdFailedToLoad(errorCode);
                    }

                    @Override
                    public void onAdOpened()
                    {
                        InterstitialAdOpened();
                    }

                    @Override
                    public void onAdLeftApplication()
                    {
                        InterstitialAdLeftApplication();
                    }

                    @Override
                    public void onAdLoaded()
                    {
                        InterstitialAdLoaded();
                    }
                });
            }
        });
    }

    public void SetInterstitialAdUnitId(final String adId)
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                m_AdInterstitial.setAdUnitId(adId);
            }
        });
    }

    public void SetInterstitialAdTestDeviceId(final String testDeviceId) {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                m_InterstitialAdTestDeviceId = testDeviceId;
            }
        });
    }

    public void LoadInterstitialAd()
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                AdRequest.Builder adRequest = new AdRequest.Builder();
                adRequest.addTestDevice(m_InterstitialAdTestDeviceId);
                m_AdInterstitial.loadAd(adRequest.build());
            }
        });
    }

    public void ShowInterstitialAd()
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                m_AdInterstitial.show();
            }
        });
    }

    /* Rewarded Video Ads functions*/
    public void InitializeRewardedVideoAd()
    {
        final QtAdMobActivity self = this;
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                m_AdRewardedVideo = MobileAds.getRewardedVideoAdInstance(self);
                m_AdRewardedVideo.setRewardedVideoAdListener(self);
            }
        });
    }

    public void SetRewardedVideoAdUnitId(final String adId)
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                m_RewardedVideoAdUnitId = adId;
            }
        });
    }

    public void SetRewardedVideoTestDeviceId(final String testDeviceId){
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                m_RewardedVideoTestDeviceId = testDeviceId;
            }
        });
    }

    public void LoadRewardedVideoAd()
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                AdRequest.Builder adRequest = new AdRequest.Builder();
                adRequest.addTestDevice(m_RewardedVideoTestDeviceId);
                m_AdRewardedVideo.loadAd(m_RewardedVideoAdUnitId, adRequest.build());
            }
        });
    }

    public void ShowRewardedVideoAd()
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                m_AdRewardedVideo.show();
            }
        });
    }

    /* Override Methods Of QtActivity */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }
    @Override
    public void onResume()
    {
        super.onResume();

        // Resume the AdView.
        if ( mAdView != null) mAdView.resume();
    }
    @Override
    public void onPause()
    {
        super.onPause();

        // Pause the AdView.
        if ( mAdView != null) mAdView.pause();
    }
    @Override
    public void onDestroy()
    {
        super.onDestroy();

        // Destroy the AdView.
        if ( mAdView != null) mAdView.destroy();
    }

    /* Override Methods Of Interface RewardedVideoAdListener */
    @Override
    public void onRewarded(RewardItem reward)
    {
        Rewarded();
    }
    @Override
    public void onRewardedVideoAdClosed()
    {
        RewardedVideoAdClosed();
    }
    @Override
    public void onRewardedVideoAdFailedToLoad(int errorCode)
    {
        RewardedVideoAdFailedToLoad(errorCode);
    }
    @Override
    public void onRewardedVideoAdLeftApplication()
    {
    }
    @Override
    public void onRewardedVideoAdLoaded()
    {
        RewardedVideoAdLoaded();
    }
    @Override
    public void onRewardedVideoAdOpened()
    {
    }
    @Override
    public void onRewardedVideoCompleted()
    {
    }
    @Override
    public void onRewardedVideoStarted()
    {
    }
}

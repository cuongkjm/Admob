cmake_minimum_required(VERSION 3.16)

project(QtAdMob VERSION 1.0 LANGUAGES CXX)

if(IOS)
    enable_language(OBJCXX)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)

add_library(qtadmob STATIC)
add_library(QtAdMob::qtadmob ALIAS qtadmob)

set_target_properties(qtadmob PROPERTIES AUTOMOC ON)
target_compile_features(qtadmob PUBLIC cxx_std_17)

target_sources(qtadmob
    PRIVATE
        QmlBanner.cpp
        QmlInterstitialAd.cpp
        QmlRewardedVideoAd.cpp
        QmlBanner.h
        QmlInterstitialAd.h
        QmlRewardedVideoAd.h
        ActiveRegistry.h
)

target_include_directories(qtadmob PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(qtadmob PUBLIC Qt6::Core Qt6::Gui Qt6::Quick)

if(IOS)
    set(GOOGLE_MOBILE_ADS_IOS_ROOT "" CACHE PATH "Path to GoogleMobileAds iOS framework root")

    target_sources(qtadmob
        PRIVATE
            "${CMAKE_CURRENT_LIST_DIR}/Platform/Ios/QtAdmobBannerIosDelegate.mm"
            "${CMAKE_CURRENT_LIST_DIR}/Platform/Ios/QtAdmobInterstitialIosDelegate.mm"
            "${CMAKE_CURRENT_LIST_DIR}/Platform/Ios/QtAdmobRewardVideoDelegate.mm"
            "${CMAKE_CURRENT_LIST_DIR}/Platform/Ios/QtAdmobBannerIosDelegate.h"
            "${CMAKE_CURRENT_LIST_DIR}/Platform/Ios/QtAdmobBannerIosDelegateImpl.h"
            "${CMAKE_CURRENT_LIST_DIR}/Platform/Ios/QtAdmobInterstitialIosDelegate.h"
            "${CMAKE_CURRENT_LIST_DIR}/Platform/Ios/QtAdmobInterstitialIosDelegateImpl.h"
            "${CMAKE_CURRENT_LIST_DIR}/Platform/Ios/QtAdmobRewardVideoDelegate.h"
            "${CMAKE_CURRENT_LIST_DIR}/Platform/Ios/QtAdmobRewardVideoDelegateImpl.h"
    )
    target_include_directories(qtadmob PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Platform/Ios")
    if(GOOGLE_MOBILE_ADS_IOS_ROOT)
        target_include_directories(qtadmob PUBLIC "${GOOGLE_MOBILE_ADS_IOS_ROOT}")
        target_link_directories(qtadmob PUBLIC "${GOOGLE_MOBILE_ADS_IOS_ROOT}")
    endif()
    target_link_options(qtadmob PUBLIC -ObjC)
    target_link_libraries(qtadmob PUBLIC "-framework GoogleMobileAds")
endif()

set_property(GLOBAL PROPERTY QTADMOB_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Platform/Android")

function(qtadmob_configure_android_target target)
    if(NOT ANDROID)
        return()
    endif()

    get_property(qtadmob_android_package_source_dir GLOBAL PROPERTY QTADMOB_ANDROID_PACKAGE_SOURCE_DIR)
    get_target_property(existing_package_source_dir ${target} QT_ANDROID_PACKAGE_SOURCE_DIR)

    if(NOT existing_package_source_dir OR existing_package_source_dir STREQUAL "existing_package_source_dir-NOTFOUND")
        set_property(TARGET ${target} PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR "${qtadmob_android_package_source_dir}")
        return()
    endif()

    set(merged_package_source_dir "${CMAKE_CURRENT_BINARY_DIR}/${target}-qtadmob-android-package")
    file(REMOVE_RECURSE "${merged_package_source_dir}")
    file(COPY "${existing_package_source_dir}/" DESTINATION "${merged_package_source_dir}")
    file(COPY "${qtadmob_android_package_source_dir}/" DESTINATION "${merged_package_source_dir}")
    set_property(TARGET ${target} PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR "${merged_package_source_dir}")
endfunction()

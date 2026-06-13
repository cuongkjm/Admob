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

set(qtadmob_qml_import_dir "${CMAKE_BINARY_DIR}/AdMob")
file(MAKE_DIRECTORY "${qtadmob_qml_import_dir}")
file(WRITE "${qtadmob_qml_import_dir}/qmldir" "module AdMob\n")

if(IOS)
    set(GOOGLE_MOBILE_ADS_IOS_ROOT "" CACHE PATH "Path to GoogleMobileAds iOS framework root")
    if(NOT GOOGLE_MOBILE_ADS_IOS_ROOT)
        message(FATAL_ERROR "GOOGLE_MOBILE_ADS_IOS_ROOT is required for iOS builds. Pass -DGOOGLE_MOBILE_ADS_IOS_ROOT=/path/to/GoogleMobileAdsSdkiOS.")
    endif()

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
    target_compile_definitions(qtadmob PRIVATE NS_SWIFT_SENDING=)
    if(GOOGLE_MOBILE_ADS_IOS_ROOT)
        set(google_mobile_ads_ios_framework_root "${GOOGLE_MOBILE_ADS_IOS_ROOT}")
        if(IS_DIRECTORY "${google_mobile_ads_ios_framework_root}/GoogleMobileAds.xcframework")
            set(google_mobile_ads_ios_framework_root "${google_mobile_ads_ios_framework_root}/GoogleMobileAds.xcframework")
        endif()

        string(TOLOWER "${CMAKE_OSX_SYSROOT}" cmake_osx_sysroot_lower)
        set(google_mobile_ads_ios_framework_search_dir "")
        if(IS_DIRECTORY "${google_mobile_ads_ios_framework_root}/GoogleMobileAds.framework")
            set(google_mobile_ads_ios_framework_search_dir "${google_mobile_ads_ios_framework_root}")
        elseif(google_mobile_ads_ios_framework_root MATCHES "\\.framework$")
            get_filename_component(google_mobile_ads_ios_framework_search_dir "${google_mobile_ads_ios_framework_root}" DIRECTORY)
        elseif(cmake_osx_sysroot_lower MATCHES "iphonesimulator" AND IS_DIRECTORY "${google_mobile_ads_ios_framework_root}/ios-arm64_x86_64-simulator/GoogleMobileAds.framework")
            set(google_mobile_ads_ios_framework_search_dir "${google_mobile_ads_ios_framework_root}/ios-arm64_x86_64-simulator")
        elseif(IS_DIRECTORY "${google_mobile_ads_ios_framework_root}/ios-arm64/GoogleMobileAds.framework")
            set(google_mobile_ads_ios_framework_search_dir "${google_mobile_ads_ios_framework_root}/ios-arm64")
        endif()

        if(google_mobile_ads_ios_framework_search_dir)
            target_compile_options(qtadmob PUBLIC "SHELL:-F${google_mobile_ads_ios_framework_search_dir}")
            target_link_options(qtadmob PUBLIC "SHELL:-F${google_mobile_ads_ios_framework_search_dir}")
        else()
            message(FATAL_ERROR "GOOGLE_MOBILE_ADS_IOS_ROOT does not contain a recognized GoogleMobileAds.framework path")
        endif()
    endif()
    target_link_options(qtadmob PUBLIC -ObjC)
    target_link_libraries(qtadmob PUBLIC
        "-framework GoogleMobileAds"
        "-framework JavaScriptCore"
    )
endif()

set_property(GLOBAL PROPERTY QTADMOB_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Platform/Android")

function(qtadmob_configure_ios_target target)
    if(NOT IOS OR NOT CMAKE_GENERATOR STREQUAL "Xcode")
        return()
    endif()

    set_target_properties(${target} PROPERTIES
        XCODE_ATTRIBUTE_ALWAYS_EMBED_SWIFT_STANDARD_LIBRARIES YES
        XCODE_ATTRIBUTE_LIBRARY_SEARCH_PATHS "$(inherited) $(TOOLCHAIN_DIR)/usr/lib/swift/$(PLATFORM_NAME)"
    )
endfunction()

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

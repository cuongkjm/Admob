QT += quick gui-private
android: QT += androidextras
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/QmlBanner.cpp \
    $$PWD/QmlInterstitialAd.cpp \
    $$PWD/QmlRewardedVideoAd.cpp

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \
#    $$PWD/android/src/org/qtproject/QtAdMobActivity.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

HEADERS += \
    $$PWD/QmlBanner.h \
    $$PWD/QmlInterstitialAd.h \
    $$PWD/QmlRewardedVideoAd.h


INCLUDEPATH += $$PWD/Platform/Ios

ios:HEADERS += \
        $$PWD/Platform/Ios/QtAdmobBannerIosDelegate.h \
        $$PWD/Platform/Ios/QtAdmobBannerIosDelegateImpl.h \
        $$PWD/Platform/Ios/QtAdmobInterstitialIosDelegate.h \
        $$PWD/Platform/Ios/QtAdmobInterstitialIosDelegateImpl.h \
        $$PWD/Platform/Ios/QtAdmobRewardVideoDelegate.h \
        $$PWD/Platform/Ios/QtAdmobRewardVideoDelegateImpl.h

ios:SOURCES += \
        $$PWD/Platform/Ios/QtAdmobBannerIosDelegate.mm \
        $$PWD/Platform/Ios/QtAdmobInterstitialIosDelegate.mm \
        $$PWD/Platform/Ios/QtAdmobRewardVideoDelegate.mm

ios:QMAKE_CXXFLAGS += -fno-objc-arc
ios:LIBS += -F $$PWD/Platform/Ios/MobileAds -framework GoogleMobileAds \
            -framework GoogleAppMeasurement \
            -framework GoogleUtilities \
            -framework nanopb
ios:QMAKE_LFLAGS += -ObjC
ios:QMAKE_INFO_PLIST = $$PWD/Platform/Ios/Info.plist



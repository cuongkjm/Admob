-keep class com.qtadmob.** { *; }
-keepclassmembers class com.qtadmob.** {
    native <methods>;
}

# WorkManager creates this Room implementation by reflection during startup.
-keep class androidx.work.impl.WorkDatabase_Impl {
    void <init>();
}

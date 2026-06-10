QT += core gui widgets sql

TARGET = Qt_main
TEMPLATE = app

SOURCES += \
    main.cpp \
    Qt_main.cpp \
    BDD.cpp \
    style.cpp

HEADERS += \
    Qt_main.h \
    BDD.h \
    style.h

FORMS += \
    Qt_main.ui

RESOURCES += \
    Qt_main.qrc

android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

# Copier BDD.db dans le dossier de build sur Windows
win32 {
    BDD_SOURCE = $$PWD/BDD.db
    BDD_DEST = $$OUT_PWD/BDD.db
    QMAKE_POST_LINK += copy /Y \"$$replace(BDD_SOURCE, /, \\)\" \"$$replace(BDD_DEST, /, \\)\"
}

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/res/xml/qtprovider_paths.xml
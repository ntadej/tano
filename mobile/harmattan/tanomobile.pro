INCLUDEPATH += ../../src/ \
    ../../

DEFINES += HARMATTAN=1 \
    MOBILE=1 \
    WITH_EDITOR_VLCQT=0

QT+= declarative
symbian:TARGET.UID3 = 0xEE6F98BA

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

HEADERS += ../MobileCore.h \
    ../MobilePlaylistHandler.h \
    ../../src/container/Channel.h \
    ../../src/core/Common.h \
    ../../src/core/ConsoleOutput.h \
    ../../src/core/Enums.h \
    ../../src/core/ListModel.h \
    ../../src/core/LocaleManager.h \
    ../../src/core/Settings.h \
    ../../src/core/Shortcuts.h \
    ../../src/core/Udpxy.h \
    ../../src/playlist/M3UHandler.h \
    ../../src/playlist/PlaylistFilterModel.h \
    ../../src/playlist/PlaylistModel.h

SOURCES += ../main.cpp \
    ../MobileCore.cpp \
    ../MobilePlaylistHandler.cpp \
    ../../src/container/Channel.cpp \
    ../../src/core/Common.cpp \
    ../../src/core/ConsoleOutput.cpp \
    ../../src/core/Enums.cpp \
    ../../src/core/ListModel.cpp \
    ../../src/core/LocaleManager.cpp \
    ../../src/core/Settings.cpp \
    ../../src/core/Shortcuts.cpp \
    ../../src/core/Udpxy.cpp \
    ../../src/playlist/M3UHandler.cpp \
    ../../src/playlist/PlaylistFilterModel.cpp \
    ../../src/playlist/PlaylistModel.cpp

OTHER_FILES += \
    tanomobile.desktop \
    tanomobile.svg \
    tanomobile.png \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

RESOURCES += \
    ../qml/files.qrc

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

# enable booster
CONFIG += qdeclarative-boostable
QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic

# remember to set DYLD_FALLBACK_LIBRARY_PATH on MacOSX
# set LD_LIBRARY_PATH on Linux

TARGET = MasterSimulatorUI
# this is the central configuration file for all IBK dependent libraries
# we check if this file was created by our build tool helper python what ever
!include( ../../../externals/IBK/projects/Qt/CONFIG.pri ){
	message( "No custom build options specified" )
}

# this pri must be sourced from all our applications
include( ../../../externals/IBK/projects/Qt/IBK.pri )

TEMPLATE = app
QT += xml core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(debug, debug|release) {
	OBJECTS_DIR = debug
	DESTDIR = ../../../bin/debug
}
else {
	OBJECTS_DIR = release
	DESTDIR = ../../../bin/release
}

LIBS += -L../../../externals/lib \
	-L../../../lib \
	-lMasterSim \
	-lminizip \
	-lz \
	-lBlockMod \
	-lDataIO \
	-lIBKMK \
	-lIBK \
	-lTiCPP

INCLUDEPATH = \
	../../src \
	../../../externals/IBK/src \
	../../../externals/DataIO/src \
	../../../externals/BlockMod/src \
	../../../MasterSim/src


SOURCES += ../../src/main.cpp \
	../../src/MSIMUIConstants.cpp \
	../../src/MSIMDirectories.cpp \
	../../src/MSIMMainWindow.cpp \
	../../src/MSIMProjectHandler.cpp \
	../../src/MSIMSettings.cpp \
    ../../src/MSIMConversion.cpp \
    ../../src/MSIMLanguageHandler.cpp \
    ../../src/MSIMMessageHandler.cpp \
    ../../src/MSIMDebugApplication.cpp

HEADERS  += \
	../../src/MSIMUIConstants.h \
	../../src/MSIMDirectories.h \
	../../src/MSIMMainWindow.h \
	../../src/MSIMProjectHandler.h \
	../../src/MSIMSettings.h \
    ../../src/MSIMConversion.h \
    ../../src/MSIMLanguageHandler.h \
    ../../src/MSIMMessageHandler.h \
    ../../src/MSIMDebugApplication.h

FORMS    += \
	../../src/MSIMMainWindow.ui

TRANSLATIONS += ../../resources/translations/MasterSimulatorUI_de.ts
CODECFORSRC = UTF-8

RESOURCES += \
    ../../resources/MasterSimulator.qrc


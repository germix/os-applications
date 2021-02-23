#-------------------------------------------------
#
# Project created by QtCreator 2016-07-08T04:56:26
#
#-------------------------------------------------

QT       += core gui

DESTDIR = ../bin

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BuddySystem-1.1
TEMPLATE = app


SOURCES +=  \
	src/main.cpp \
	src/MainWindow.cpp \
	src/BuddyNode.cpp \
	src/BuddySystem.cpp \
	src/BuddySystemCanvas.cpp \
	src/Lexer.cpp \
	src/utils.cpp \
    src/AboutDialog.cpp \
	src/HelpDialog.cpp

HEADERS  += \
	src/MainWindow.h \
	src/BuddyNode.h \
	src/BuddySystem.h \
	src/BuddySystemCanvas.h \
	src/Lexer.h \
    src/AboutDialog.h \
	src/HelpDialog.h

FORMS   += src/MainWindow.ui \
    src/AboutDialog.ui \
	src/HelpDialog.ui

#--------------------------------------------------------------------------------------------------

RESOURCES += res/resources.qrc

win32:RC_FILE = res/resources_win32.rc

#--------------------------------------------------------------------------------------------------

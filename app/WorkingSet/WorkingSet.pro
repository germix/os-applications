#-------------------------------------------------
#
# Project created by QtCreator 2017-01-31T18:34:54
#
#-------------------------------------------------

QT       += core gui

DESTDIR = ../bin

TARGET = WorkingSet
TEMPLATE = app


SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/WorkingSet.cpp \
    src/AboutDialog.cpp

HEADERS  += src/MainWindow.h \
    src/WorkingSet.h \
    src/AboutDialog.h

FORMS    += src/MainWindow.ui \
    src/AboutDialog.ui

#--------------------------------------------------------------------------------------------------

RESOURCES += res/resources.qrc

win32:RC_FILE = res/resources_win32.rc

#--------------------------------------------------------------------------------------------------

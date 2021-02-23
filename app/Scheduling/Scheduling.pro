#-------------------------------------------------
#
# Project created by QtCreator 2016-07-04T06:04:01
#
#-------------------------------------------------

QT       += core gui xml

DESTDIR = ../bin

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scheduling
TEMPLATE = app

INCLUDEPATH += src


SOURCES += src/main.cpp\
		src/MainWindow.cpp \
	src/ColorLineEdit.cpp \
	src/MyItemDelegate.cpp \
	src/MyItemModel.cpp \
	src/utils.cpp \
	src/GantCanvas.cpp \
	src/GantLine.cpp \
	src/GantSegment.cpp \
	src/AboutDialog.cpp \
	src/SchedulingFIFO.cpp \
	src/SchedulingSJF.cpp \
	src/Process.cpp \
	src/TimeMarkManager.cpp \
	src/ProcessState.cpp \
	src/AbstractContext.cpp \
	src/SchedulingSRT.cpp \
	src/SchedulingRR.cpp \
    src/HelpDialog.cpp

HEADERS  += src/MainWindow.h \
	src/ColorLineEdit.h \
	src/MyItemDelegate.h \
	src/MyItemModel.h \
	src/utils.h \
	src/GantCanvas.h \
	src/GantLine.h \
	src/GantSegment.h \
	src/AboutDialog.h \
	src/Scheduling.h \
	src/SchedulingFIFO.h \
	src/SchedulingSJF.h \
	src/Process.h \
	src/TimeMark.h \
	src/TimeMarkManager.h \
	src/AbstractContext.h \
	src/SchedulingSRT.h \
	src/SchedulingRR.h \
    src/HelpDialog.h \
    src/AbstractContext2.h

FORMS    += src/MainWindow.ui \
	src/AboutDialog.ui \
    src/HelpDialog.ui

#--------------------------------------------------------------------------------------------------

RESOURCES += res/resources.qrc

win32:RC_FILE = res/resources_win32.rc

#--------------------------------------------------------------------------------------------------

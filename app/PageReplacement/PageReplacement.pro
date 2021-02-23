#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T08:09:16
#
#-------------------------------------------------

QT       += core gui

DESTDIR = ../bin

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PageReplacement
TEMPLATE = app


SOURCES += \
	src/main.cpp\
	src/MainWindow.cpp \
	src/PageReplacementExecutor.cpp \
	src/PageReplacementAlgolFIFO.cpp \
	src/PageReplacementAlgolLRU.cpp \
	src/PageReplacementAlgolOPT.cpp \
	src/PageReplacementAlgolCLOCK.cpp \
	src/TableView.cpp \
	src/TableElement.cpp \
	src/PageProvider.cpp \
	src/FrameProvider.cpp \
	src/PageReferences.cpp \
	src/AboutDialog.cpp \
	src/PageListLRU.cpp

HEADERS  += \
	src/MainWindow.h \
	src/PageReplacement.h \
	src/PageReplacementExecutor.h \
	src/PageReplacementAlgolFIFO.h \
	src/PageReplacementAlgolLRU.h \
	src/PageReplacementAlgolOPT.h \
	src/PageReplacementAlgolCLOCK.h \
	src/PagePointer.h \
	src/Page.h \
	src/Frame.h \
	src/ReferenceBit.h \
	src/TableView.h \
	src/TableElement.h \
	src/PageProvider.h \
	src/FrameProvider.h \
	src/PageReferences.h \
	src/PageReplacementExecutorListener.h \
	src/AboutDialog.h \
	src/PageListLRU.h

FORMS    += \
	src/MainWindow.ui \
	src/AboutDialog.ui

#--------------------------------------------------------------------------------------------------

RESOURCES += res/resources.qrc

win32:RC_FILE = res/resources_win32.rc

#--------------------------------------------------------------------------------------------------



#-------------------------------------------------
#
# Project created by QtCreator 2017-01-29T21:35:32
#
#-------------------------------------------------

QT       += core gui
DESTDIR = ../bin

TARGET = DynamicPartitioning
TEMPLATE = app


SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/Canvas.cpp \
    src/Lexer.cpp \
    src/utils.cpp \
    src/MemoryManager.cpp \
    src/ChunkModel.cpp \
    src/AboutDialog.cpp \
    src/HelpDialog.cpp

HEADERS  += src/MainWindow.h \
    src/Canvas.h \
    src/Lexer.h \
    src/MemoryManager.h \
    src/ChunkModel.h \
    src/AboutDialog.h \
    src/HelpDialog.h

FORMS    += src/MainWindow.ui \
    src/AboutDialog.ui \
    src/HelpDialog.ui

#--------------------------------------------------------------------------------------------------

RESOURCES += res/resources.qrc

win32:RC_FILE = res/resources_win32.rc

#--------------------------------------------------------------------------------------------------

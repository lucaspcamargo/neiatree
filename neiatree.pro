QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = neiatree
TEMPLATE = app


SOURCES += src/main.cpp \
    src/nResourceProcessorTool.cpp \
    src/nRPTDialog.cpp \
    src/nRPTRulesFile.cpp \
    src/nRPTTaskLIst.cpp \
    src/nRPTTaskWorker.cpp \
    src/nRPTWorker.cpp

FORMS += \
    src/nRPTDialog.ui

HEADERS += \
    src/nResourceProcessorTool.h \
    src/nRPTDialog.h \
    src/nRPTRulesFile.h \
    src/nRPTTask.h \
    src/nRPTTaskLIst.h \
    src/nRPTTaskWorker.h \
    src/nRPTWorker.h

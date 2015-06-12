QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = neiatree
TEMPLATE = app


SOURCES += src/main.cpp \
    src/ntree.cpp \
    src/nRPTDialog.cpp \
    src/nRPTRulesFile.cpp \
    src/nRPTTaskList.cpp \
    src/nRPTTaskWorker.cpp \
    src/nRPTWorker.cpp \
    src/nWrappedVariantMap.cpp

FORMS += \
    src/nRPTDialog.ui

HEADERS += \
    src/ntree.h \
    src/nRPTDialog.h \
    src/nRPTRulesFile.h \
    src/nRPTTask.h \
    src/nRPTTaskList.h \
    src/nRPTTaskWorker.h \
    src/nRPTWorker.h \
    util/nIODefines.h \
    src/nWrappedVariantMap.h

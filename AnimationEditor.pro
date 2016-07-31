QT += core widgets gui

CONFIG += c++11

TARGET = AnimationEditor
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
SOURCES += \
    src/main.cpp \
    src/AnimationEditor.cpp

HEADERS += \
    src/AnimationEditor.h \
    src/Animation.h

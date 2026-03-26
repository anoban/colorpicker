TEMPLATE = app
SOURCES += colorpicker.cpp
HEADERS += ./include/*.hpp
INCLUDEPATH += ./include
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # disables all the APIs deprecated before Qt 6.0.0
TARGET = Colorpicker # this becomes the programme title
QT += core gui widgets # https://stackoverflow.com/questions/48746408/qt5-undefined-reference-to-qapplicationqapplicationint-char-int
QMAKE_CXXFLAGS += -std=c++20 -march=tigerlake -mavx512f -ffast-math -mprefer-vector-width=512 -Wno-class-memaccess
RC_ICONS = ./icons/icon.png

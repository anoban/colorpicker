TEMPLATE = app
SOURCES += colorpicker.cpp
HEADERS += ./include/*.hpp
INCLUDEPATH += ./include
TARGET = colorpicker.out
QT += core gui widgets # https://stackoverflow.com/questions/48746408/qt5-undefined-reference-to-qapplicationqapplicationint-char-int
QMAKE_CXXFLAGS += -std=c++20 -march=tigerlake -mavx512f -ffast-math -mprefer-vector-width=512 -Wno-class-memaccess

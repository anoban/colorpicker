TEMPLATE = app
SOURCES += colorpicker.cpp
HEADERS += ./include/*.hpp
INCLUDEPATH += ./include
TARGET = colorpicker.out
QT += core gui widgets # https://stackoverflow.com/questions/48746408/qt5-undefined-reference-to-qapplicationqapplicationint-char-int
CONFIG += ltcg c++20

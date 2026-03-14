#pragma once
#ifndef __MAINWINDOW_HPP
    #define __MAINWINDOW_HPP 1
#endif

#include <QtWidgets/QWidget>
#include <trackbargroup.hpp>

class window final : public QWidget {
        Q_OBJECT

    private:
        trackbar_grid* _trackbar_group;

    public:
        inline window(QWidget* const _parent_window = nullptr) noexcept : QWidget(_parent_window) { }
};

#pragma once

// title bars are handled by the OS and the desktop window managers (GNOME or KDE), hence Qt does not have much control over it
// to be able to customize it, we need first hide the OS dependent title bar and implement a pseudo title bar

#ifndef __TITLEBAR_HPP
    #define __TITLEBAR_HPP 1
#endif

#include <config.hpp>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class title_bar final : public QGridLayout { // a dummy title bar that replaces the DWM provided one
        Q_OBJECT
        //

    private:
        QPushButton _stayontop;
        QPushButton _minimize;
        QPushButton _close;
        QPushButton _about;

    public:
        inline explicit title_bar(QWidget* const _parent_window) noexcept :
            QGridLayout { _parent_window }, _stayontop { _parent_window }, _minimize { _parent_window }, _close { _parent_window }, _about { _parent_window } {
            //
            // setFixedWidth(configs::titlebar::WIDTH);
            // setFixedHeight(configs::titlebar::HEIGHT);
            setGeometry(QRect { 0, 0, configs::titlebar::WIDTH, configs::titlebar::HEIGHT });
        }

    private:
};

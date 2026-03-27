#pragma once

// title bars are handled by the OS and the desktop window managers (GNOME or KDE), hence Qt does not have much control over it
// to be able to customize it, we need first hide the OS dependent title bar and implement a pseudo title bar

#ifndef __TITLEBAR_HPP
    #define __TITLEBAR_HPP 1
#endif

// clang-format off
#include <config.hpp>
#include <utilities.hpp>
// clang-format on

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class title_bar final : public QWidget { // a dummy title bar that replaces the DWM provided one
        Q_OBJECT
        //

    private:
        std::array<QPushButton, configs::titlebar::NBUTTONS> _pushbuttons;
        QHBoxLayout                                          _layout;

    public:
        inline explicit title_bar(QWidget* const _parent_window) noexcept :
            QWidget { _parent_window },
            _pushbuttons { QPushButton { _parent_window },
                           QPushButton { _parent_window },
                           QPushButton { _parent_window },
                           QPushButton { _parent_window },
                           QPushButton { _parent_window } },
            _layout { _parent_window } {
            //
            // setFixedWidth(configs::titlebar::WIDTH);
            // setFixedHeight(configs::titlebar::HEIGHT);
            setGeometry(QRect { 0, 0, configs::titlebar::WIDTH, configs::titlebar::HEIGHT });
            const auto _pushbutton_stylesheet = utilities::read_qss(R"(./styles/QPushButton.qss)");

            // set the button icons
            _pushbuttons[configs::titlebar::button_offsets::MINIMIZE].setIcon(QIcon(R"(./icons/minimize.png)"));
            _pushbuttons[configs::titlebar::button_offsets::CLOSE].setIcon(QIcon(R"(./icons/close.png)"));
            _pushbuttons[configs::titlebar::button_offsets::STAYONTOP].setIcon(QIcon(R"(./icons/stayontop.png)"));

            for (unsigned i = 0; i < configs::titlebar::NBUTTONS; ++i) {
                _pushbuttons[i].setFixedSize(configs::titlebar::ICON_SHAPE);
                _layout.addWidget(&_pushbuttons[i]);
            }
        }

    private:
};

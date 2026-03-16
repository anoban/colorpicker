#pragma once
#ifndef __MAINWINDOW_HPP
    #define __MAINWINDOW_HPP 1
#endif

#include <config.hpp>
#include <QtWidgets/QFrame>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

class main_window final : public QFrame {
        Q_OBJECT

    private:
        QSlider _rslider;
        QSlider _gslider;
        QSlider _bslider;

    public:
        inline main_window(QWidget* const _parent_window = nullptr) noexcept :
            QFrame(_parent_window),
            // when child widgets inherit from the parent widget, calling QWidget::show() on the parent will automatically draw the children too
            // no need to call .show() on every widget inside the main window
            _rslider(Qt::Orientation::Horizontal, this),
            _gslider(Qt::Orientation::Horizontal, this),
            _bslider(Qt::Orientation::Horizontal, this) {
            //
            setMinimumHeight(configs::main_window::HEIGHT);
            setMinimumWidth(configs::main_window::WIDTH);

            // make the corners round
            setStyleSheet(".QFrame{border: 10px; border-radius: 30px;}");

            // horizontal sliders

            _rslider.setFocusPolicy(Qt::FocusPolicy::StrongFocus);
            _rslider.setTickPosition(QSlider::TicksBothSides); // show ticks above and below the slider
            _rslider.setTickInterval(configs::trackbars::TICK_INTERVAL);
            _rslider.setSingleStep(1);
            _rslider.setMaximumWidth(configs::trackbars::WIDTH);
            _rslider.setMinimumHeight(configs::trackbars::HEIGHT);
            _rslider.setGeometry(configs::trackbars::LEFTPAD, configs::trackbars::VERTICAL_MARGIN, configs::trackbars::WIDTH, configs::trackbars::HEIGHT);

            _gslider.setFocusPolicy(Qt::FocusPolicy::StrongFocus);
            _gslider.setTickPosition(QSlider::TicksBothSides); // show ticks above and below the slider
            _gslider.setTickInterval(configs::trackbars::TICK_INTERVAL);
            _gslider.setSingleStep(1);
            _gslider.setMaximumWidth(configs::trackbars::WIDTH);
            _gslider.setMinimumHeight(configs::trackbars::HEIGHT);
            _gslider.setGeometry(
                configs::trackbars::LEFTPAD,
                configs::trackbars::VERTICAL_MARGIN + configs::trackbars::VSPACE,
                configs::trackbars::WIDTH,
                configs::trackbars::HEIGHT
            );

            _bslider.setFocusPolicy(Qt::FocusPolicy::StrongFocus);
            _bslider.setTickPosition(QSlider::TicksBothSides); // show ticks above and below the slider
            _bslider.setTickInterval(configs::trackbars::TICK_INTERVAL);
            _bslider.setSingleStep(1);
            _bslider.setMaximumWidth(configs::trackbars::WIDTH);
            _bslider.setMinimumHeight(configs::trackbars::HEIGHT);
            _bslider.setGeometry(
                configs::trackbars::LEFTPAD,
                configs::trackbars::VERTICAL_MARGIN + 2 * configs::trackbars::VSPACE,
                configs::trackbars::WIDTH,
                configs::trackbars::HEIGHT
            );
        }
};

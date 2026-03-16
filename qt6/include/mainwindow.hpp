#pragma once
#ifndef __MAINWINDOW_HPP
    #define __MAINWINDOW_HPP 1
#endif

#include <config.hpp>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSlider>

class main_window final : public QFrame {
        Q_OBJECT

    private:
        QSlider _rslider;
        QSlider _gslider;
        QSlider _bslider;

        QLineEdit _rlabel;
        QLineEdit _glabel;
        QLineEdit _blabel;

        QLineEdit _hexstring;

    public:
        explicit inline main_window(QWidget* const _parent_window = nullptr) noexcept :
            QFrame(_parent_window),
            // when child widgets inherit from the parent widget, calling QWidget::show() on the parent will automatically draw the children too
            // no need to call .show() on every widget inside the main window
            _rslider(Qt::Orientation::Horizontal, this),
            _gslider(Qt::Orientation::Horizontal, this),
            _bslider(Qt::Orientation::Horizontal, this),
            _rlabel(this),
            _glabel(this),
            _blabel(this),
            _hexstring(this) {
            //
            setFixedWidth(configs::main_window::WIDTH);
            setFixedHeight(configs::main_window::HEIGHT);

            // make the corners round
            setStyleSheet(".QFrame{border: 10px; border-radius: 30px;}");

            // horizontal sliders - RGB

            _rslider.setFocusPolicy(Qt::FocusPolicy::StrongFocus);
            // _rslider.setTickPosition(QSlider::TicksBothSides); // show ticks above and below the slider
            // _rslider.setTickInterval(configs::trackbars::TICK_INTERVAL);
            _rslider.setSingleStep(1);
            _rslider.setMaximumWidth(configs::trackbars::WIDTH);
            _rslider.setMinimumHeight(configs::trackbars::HEIGHT);
            _rslider.setGeometry(configs::trackbars::PAD, configs::trackbars::VERTICAL_MARGIN, configs::trackbars::WIDTH, configs::trackbars::HEIGHT);

            _gslider.setFocusPolicy(Qt::FocusPolicy::StrongFocus);
            _gslider.setSingleStep(1);
            _gslider.setMaximumWidth(configs::trackbars::WIDTH);
            _gslider.setMinimumHeight(configs::trackbars::HEIGHT);
            _gslider.setGeometry(
                configs::trackbars::PAD, configs::trackbars::VERTICAL_MARGIN + configs::trackbars::VSPACE, configs::trackbars::WIDTH, configs::trackbars::HEIGHT
            );

            _bslider.setFocusPolicy(Qt::FocusPolicy::StrongFocus);
            _bslider.setSingleStep(1);
            _bslider.setMaximumWidth(configs::trackbars::WIDTH);
            _bslider.setMinimumHeight(configs::trackbars::HEIGHT);
            _bslider.setGeometry(
                configs::trackbars::PAD,
                configs::trackbars::VERTICAL_MARGIN + 2 * configs::trackbars::VSPACE,
                configs::trackbars::WIDTH,
                configs::trackbars::HEIGHT
            );

            // trackbar labels
            _rlabel.setGeometry(
                configs::trackbars::PAD + configs::trackbars::WIDTH + configs::trackbars::labels::PAD,
                configs::trackbars::VERTICAL_MARGIN,
                configs::trackbars::labels::WIDTH,
                configs::trackbars::labels::HEIGHT
            );

            _glabel.setGeometry(
                configs::trackbars::PAD + configs::trackbars::WIDTH + configs::trackbars::labels::PAD,
                configs::trackbars::VERTICAL_MARGIN + configs::trackbars::VSPACE,
                configs::trackbars::labels::WIDTH,
                configs::trackbars::labels::HEIGHT
            );

            _blabel.setGeometry(
                configs::trackbars::PAD + configs::trackbars::WIDTH + configs::trackbars::labels::PAD,
                configs::trackbars::VERTICAL_MARGIN + 2 * configs::trackbars::VSPACE,
                configs::trackbars::labels::WIDTH,
                configs::trackbars::labels::HEIGHT
            );

            _hexstring.setGeometry(
                configs::trackbars::PAD + configs::trackbars::WIDTH + configs::trackbars::labels::PAD + configs::trackbars::labels::WIDTH +
                    configs::trackbars::labels::PAD,
                configs::trackbars::VERTICAL_MARGIN + 2 * configs::trackbars::VSPACE,
                configs::hexstring::WIDTH,
                configs::hexstring::HEIGHT
            );
        }
};

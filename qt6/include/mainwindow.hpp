#pragma once
#ifndef __MAINWINDOW_HPP
    #define __MAINWINDOW_HPP 1
#endif

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

// clang-format off
#include <config.hpp>
#include <rgbhexstring.hpp>
// clang-format on

class main_window final : public QMainWindow {
        Q_OBJECT

    private:
        std::array<QSlider, configs::trackbars::N>  _rgbsliders;   // sliders for RGB colours
        std::array<QSpinBox, configs::trackbars::N> _rgbspinboxes; // labels for the RGB sliders
        rgbhexstring                                _hexstring;

    public:
        explicit inline main_window(QWidget* const _parent_window = nullptr) noexcept :
            QMainWindow(_parent_window),
            // when child widgets inherit from the parent widget, calling QWidget::show() on the parent will automatically draw the children too
            // no need to call .show() on every widget inside the main window

            _rgbsliders { QSlider(Qt::Orientation::Horizontal, this), QSlider(Qt::Orientation::Horizontal, this), QSlider(Qt::Orientation::Horizontal, this) },
            _rgbspinboxes { QSpinBox(this), QSpinBox(this), QSpinBox(this) },
            _hexstring { this } {
            //
            setFixedWidth(configs::main_window::WIDTH);
            setFixedHeight(configs::main_window::HEIGHT);

            // make the corners round
            setStyleSheet(" QMainWindow {border: 10px; border-radius: 30px;}");

            // horizontal sliders - RGB
            for (unsigned i = 0; i < _rgbsliders.size(); ++i) {
                // _rslider.setTickPosition(QSlider::TicksBothSides); // show ticks above and below the slider
                // _rslider.setTickInterval(configs::trackbars::TICK_INTERVAL);
                _rgbsliders[i].setFocusPolicy(Qt::FocusPolicy::StrongFocus);
                _rgbsliders[i].setSingleStep(1);
                _rgbsliders[i].setMaximumWidth(configs::trackbars::WIDTH);
                _rgbsliders[i].setMinimumHeight(configs::trackbars::HEIGHT);
                _rgbsliders[i].setGeometry(
                    configs::trackbars::PAD,
                    configs::trackbars::VERTICAL_MARGIN + i * configs::trackbars::VSPACE,
                    configs::trackbars::WIDTH,
                    configs::trackbars::HEIGHT
                );

                _rgbsliders[i].setRange(
                    // we need to have matching ranges in sliders and their corresponding labels
                    std::numeric_limits<unsigned char>::min(),
                    std::numeric_limits<unsigned char>::max()
                );
            }

            // trackbar labels
            for (unsigned i = 0; i < _rgbspinboxes.size(); ++i) {
                _rgbspinboxes[i].setGeometry(
                    configs::trackbars::PAD + configs::trackbars::WIDTH + configs::trackbars::labels::PAD,
                    configs::trackbars::VERTICAL_MARGIN + i * configs::trackbars::VSPACE,
                    configs::trackbars::labels::WIDTH,
                    configs::trackbars::labels::HEIGHT
                );
                _rgbspinboxes[i].setButtonSymbols(QAbstractSpinBox::NoButtons);
                _rgbspinboxes[i].setRange(std::numeric_limits<unsigned char>::min(), std::numeric_limits<unsigned char>::max());
                _rgbspinboxes[i].setAlignment(Qt::AlignmentFlag::AlignCenter);
            }

            __connect_signals_to_slots();
        }

    private:
        inline void __connect_signals_to_slots() const noexcept {
            // establishing two way communication between sliders and their corresponding labels
            // connect the QSlider::valueChanged signal of each slider to the SpinBox::setValue slot of their corresponding label
            for (unsigned i = 0; i < _rgbsliders.size(); ++i) connect(&_rgbsliders[i], &QSlider::valueChanged, &_rgbspinboxes[i], &QSpinBox::setValue);
            // connect the QSpinBox::valueChanged signal of each label to the QSlider::setValue slot of their corresponding slider
            for (unsigned i = 0; i < _rgbspinboxes.size(); ++i) connect(&_rgbspinboxes[i], &QSpinBox::valueChanged, &_rgbsliders[i], &QSlider::setValue);

            //
            connect(&_rgbsliders[0], &QSlider::valueChanged, &_hexstring, &rgbhexstring::rslider_moved);
            connect(&_rgbsliders[1], &QSlider::valueChanged, &_hexstring, &rgbhexstring::gslider_moved);
            connect(&_rgbsliders[2], &QSlider::valueChanged, &_hexstring, &rgbhexstring::bslider_moved);
        }
};

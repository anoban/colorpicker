#pragma once
#ifndef __MAINWINDOW_HPP
    #define __MAINWINDOW_HPP 1
#endif

#include <config.hpp>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>

class main_window final : public QMainWindow {
        Q_OBJECT

    private:
        std::array<QSlider, configs::trackbars::N>  _rgbsliders;
        std::array<QSpinBox, configs::trackbars::N> _rgbspinboxes;

        QLineEdit _hexstring;

    public:
        explicit inline main_window(QWidget* const _parent_window = nullptr) noexcept :
            QMainWindow(_parent_window),
            // when child widgets inherit from the parent widget, calling QWidget::show() on the parent will automatically draw the children too
            // no need to call .show() on every widget inside the main window

            _rgbsliders {
                QSlider { Qt::Orientation::Horizontal, this },
                 QSlider { Qt::Orientation::Horizontal, this },
                 QSlider { Qt::Orientation::Horizontal, this }
        },

            _rgbspinboxes { QSpinBox { this }, QSpinBox { this }, QSpinBox { this } },
            _hexstring(this) {
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
                _rgbspinboxes[i].setRange(0, 255);
            }

            _hexstring.setGeometry(
                configs::trackbars::PAD + configs::trackbars::WIDTH + configs::trackbars::labels::PAD + configs::trackbars::labels::WIDTH +
                    configs::trackbars::labels::PAD,
                configs::trackbars::VERTICAL_MARGIN + 2 * configs::trackbars::VSPACE,
                configs::hexstring::WIDTH,
                configs::hexstring::HEIGHT
            );

            __connect_signals_to_slots();
        }

    private:
        inline void __connect_signals_to_slots() noexcept {
            for (unsigned i = 0; i < _rgbspinboxes.size(); ++i) connect(&_rgbsliders[i], &QSlider::valueChanged, &_rgbspinboxes[i], &QSpinBox::setValue);
            for (unsigned i = 0; i < _rgbspinboxes.size(); ++i) connect(&_rgbspinboxes[i], &QSpinBox::valueChanged, &_rgbsliders[i], &QSlider::setValue);
        }
};

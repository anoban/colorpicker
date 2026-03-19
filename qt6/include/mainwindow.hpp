#pragma once
#ifndef __MAINWINDOW_HPP
    #define __MAINWINDOW_HPP 1
#endif

#include <QtWidgets/QFrame>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

// clang-format off
#include <config.hpp>
#include <rgbhexstring.hpp>
#include <utilities.hpp>
// clang-format on

class main_window final : public QFrame {
        // choosing QFrame as parent class, since QMainWindow does not support the CSS box model, which is required to customize round corners using style sheets
        Q_OBJECT

    private:
        std::array<QSlider, configs::trackbars::N>  _rgbsliders;                                    // sliders for RGB colours
        std::array<QSpinBox, configs::trackbars::N> _rgbspinboxes;                                  // labels for the RGB sliders
        rgbhexstring                                _hexstring;                                     // the RGB colour combination in hex format e.g. #9F25E9
        int                                         _rslider_value, _gslider_value, _bslider_value; // RGB slider values
        QPalette                                    _palette;                                       // colour palette to paint the main window background with

    public:
        explicit inline main_window(QWidget* const _parent_window = nullptr) noexcept :
            QFrame(_parent_window, Qt::WindowType::Window | Qt::WindowType::WindowMinimizeButtonHint),
            // when child widgets inherit from the parent widget, calling QWidget::show() on the parent will automatically draw the children too
            // no need to call .show() on every widget inside the main window

            _rgbsliders { QSlider(Qt::Orientation::Horizontal, this), QSlider(Qt::Orientation::Horizontal, this), QSlider(Qt::Orientation::Horizontal, this) },
            _rgbspinboxes { QSpinBox(this), QSpinBox(this), QSpinBox(this) },
            _hexstring { this },
            _rslider_value {},
            _gslider_value {},
            _bslider_value {},
            _palette {} {
            // setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
            // creating round corners using stylesheets won't actually make the corners appear round,
            // the round corners will appear inside an outer rectangular corner, and to hide this outer rectangular corner, we need to apply a mask
            setAutoFillBackground(true); // https://doc.qt.io/archives/qt-6.2/qwidget.html#autoFillBackground-prop
            // if enabled, setAutoFillBackground will cause Qt to fill the background of the widget before invoking the paint event
            // the color used is defined by the QPalette::Window color role from the widget's palette.
            setFixedWidth(configs::main_window::WIDTH); // the main window will have a fixed size, with no options to enlarge
            setFixedHeight(configs::main_window::HEIGHT);

            auto _qslider_stylesheet = utilities::read_qss(R"(./styles/QSlider.qss)");

            for (unsigned i = 0; i < configs::trackbars::N; ++i) {
                //---------------------
                // sliders
                //---------------------

                _rgbsliders[i].setFocusPolicy(Qt::FocusPolicy::StrongFocus);
                _rgbsliders[i].setSingleStep(1);
                _rgbsliders[i].setMinimumWidth(configs::trackbars::WIDTH);
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

                if (_qslider_stylesheet) _rgbsliders[i].setStyleSheet(_qslider_stylesheet.value()); // styling for the slider button groove

                //---------------------
                // spin boxes
                //---------------------

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

    public:
        Q_SLOT inline void __attribute__((__always_inline__)) rslider_moved(int _new_value) noexcept {
            // will be signalled to when the red slider is moved
            _rslider_value = _new_value;
            __update_bg();
        }

        Q_SLOT inline void __attribute__((__always_inline__)) gslider_moved(int _new_value) noexcept {
            // will be signalled to when the green slider is moved
            _gslider_value = _new_value;
            __update_bg();
        }

        Q_SLOT inline void __attribute__((__always_inline__)) bslider_moved(int _new_value) noexcept {
            // will be signalled to when the blue slider is moved
            _bslider_value = _new_value;
            __update_bg();
        }

    private:
        inline void __attribute__((__always_inline__)) __connect_signals_to_slots() const noexcept {
            // establishing two way communication between sliders and their corresponding labels
            // connect the QSlider::valueChanged signal of each slider to the SpinBox::setValue slot of their corresponding label
            for (unsigned i = 0; i < _rgbsliders.size(); ++i) connect(&_rgbsliders[i], &QSlider::valueChanged, &_rgbspinboxes[i], &QSpinBox::setValue);
            // connect the QSpinBox::valueChanged signal of each label to the QSlider::setValue slot of their corresponding slider
            for (unsigned i = 0; i < _rgbspinboxes.size(); ++i) connect(&_rgbspinboxes[i], &QSpinBox::valueChanged, &_rgbsliders[i], &QSlider::setValue);

            // establishing one way communication between all the three sliders and the hex string
            connect(&_rgbsliders[0], &QSlider::valueChanged, &_hexstring, &rgbhexstring::rslider_moved);
            connect(&_rgbsliders[1], &QSlider::valueChanged, &_hexstring, &rgbhexstring::gslider_moved);
            connect(&_rgbsliders[2], &QSlider::valueChanged, &_hexstring, &rgbhexstring::bslider_moved);

            // establishing one way communication between all the three sliders and the main window
            connect(&_rgbsliders[0], &QSlider::valueChanged, this, &main_window::rslider_moved);
            connect(&_rgbsliders[1], &QSlider::valueChanged, this, &main_window::gslider_moved);
            connect(&_rgbsliders[2], &QSlider::valueChanged, this, &main_window::bslider_moved);
        }

        inline void __attribute__((__always_inline__)) __update_bg() noexcept {
            // update the colour palette with the current state of the sliders
            _palette.setColor(QPalette::Window, QColor { _rslider_value, _gslider_value, _bslider_value });
            setPalette(_palette); // set the updated palette, triggering a window redraw
        }
};

#pragma once
#ifndef __MAINWINDOW_HPP
    #define __MAINWINDOW_HPP 1
#endif

#include <QtCore/QPointF>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QFrame>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

// clang-format off
#include <rgbhexstring.hpp>
#include <titlebar.hpp>
// clang-format on

class main_window final : public QFrame {
        // choosing QFrame as parent class, since QMainWindow does not support the CSS box model, which is required to customize round corners using style sheets
        Q_OBJECT

    private:
        title_bar                                 _custom_titlebar;
        std::array<QSlider, configs::sliders::N>  _rgbsliders;   // sliders for RGB colours
        std::array<QSpinBox, configs::sliders::N> _rgbspinboxes; // labels for the RGB sliders
        // the reason for using QSpinBox instead of QLabel or QLineEdit is, QLabel does not have a rectangular outline and QLineEdit is fiddly to use with numerical outputs
        // so we use QSpinBox and hide their increment and decrement buttons
        std::array<int, configs::sliders::N>      _slider_values; // RGB QSlider values
        rgb_hexstring                             _hexstring;     // the RGB colour combination in hex format e.g. #9F25E9
        QPointF                                   _mouse_pos;

    public:
        explicit inline main_window() noexcept :
            QFrame {
                nullptr, Qt::WindowType::Window // | Qt::WindowType::FramelessWindowHint /* Qt::WindowType::FramelessWindowHint hides the default title bar */
            },
            _custom_titlebar { this },
            // when child widgets inherit from the parent widget, calling QWidget::show() on the parent will automatically draw the children too
            // no need to call .show() on every widget inside the main window
            _rgbsliders { QSlider(Qt::Orientation::Horizontal, this), QSlider(Qt::Orientation::Horizontal, this), QSlider(Qt::Orientation::Horizontal, this) },
            _rgbspinboxes { QSpinBox(this), QSpinBox(this), QSpinBox(this) },
            _slider_values {},
            _hexstring { this },
            _mouse_pos {} {
            setAutoFillBackground(true); // https://doc.qt.io/archives/qt-6.2/qwidget.html#autoFillBackground-prop
            // if enabled, setAutoFillBackground will cause Qt to fill the background of the widget before invoking the paint event
            // the colour used is defined by the QPalette::Window colour role from the widget's palette.
            setFixedWidth(configs::main_window::WIDTH); // the main window will have a fixed size, with no options to enlarge
            setFixedHeight(configs::main_window::HEIGHT);
            setAttribute(Qt::WA_TranslucentBackground); // hides the original rectangular window

            // stylesheet for the QSliders
            const auto _qslider_stylesheet  = utilities::read_qss(R"(./styles/QSlider.qss)");
            // the order of CSS box model styling is top-left, top-right, bottom-right and bottom-left
            // https://thesmithfam.org/blog/2010/03/10/fancy-qslider-stylesheet/

            // don't need this anymore
            const auto _qspinbox_stylesheet = utilities::read_qss(R"(./styles/QSpinBox.qss)"); // style sheet for the main window (QFrame)

            for (unsigned i = 0; i < configs::sliders::N; ++i) {
                //---------------------
                // sliders
                //---------------------

                _rgbsliders[i].setFocusPolicy(Qt::FocusPolicy::StrongFocus);
                _rgbsliders[i].setSingleStep(1);
                _rgbsliders[i].setMinimumWidth(configs::sliders::WIDTH);
                _rgbsliders[i].setMinimumHeight(configs::sliders::HEIGHT);
                _rgbsliders[i].setGeometry(
                    configs::sliders::HPAD, configs::sliders::VSPACE + i * configs::sliders::VSPACE_SLIDERS, configs::sliders::WIDTH, configs::sliders::HEIGHT
                );

                _rgbsliders[i].setRange(
                    // we need to have matching ranges in sliders and their corresponding labels
                    std::numeric_limits<unsigned char>::min(),
                    std::numeric_limits<unsigned char>::max()
                );

                _rgbsliders[i].setProperty("class", configs::sliders::QSS_CLASS_NAMES[i]);          // css property to be leveraged in QSlider.qss
                if (_qslider_stylesheet) _rgbsliders[i].setStyleSheet(_qslider_stylesheet.value()); // styling for the slider button groove

                //---------------------
                // spin boxes
                //---------------------

                _rgbspinboxes[i].setGeometry(
                    configs::sliders::HPAD + configs::sliders::WIDTH + configs::sliders::labels::HPAD,
                    configs::sliders::VSPACE + i * configs::sliders::VSPACE_SLIDERS,
                    configs::sliders::labels::WIDTH,
                    configs::sliders::labels::HEIGHT
                );
                _rgbspinboxes[i].setButtonSymbols(QAbstractSpinBox::NoButtons); // hide the spin box buttons
                _rgbspinboxes[i].setRange(std::numeric_limits<unsigned char>::min(), std::numeric_limits<unsigned char>::max());
                _rgbspinboxes[i].setAlignment(Qt::AlignmentFlag::AlignCenter);

                if (_qspinbox_stylesheet) _rgbspinboxes[i].setStyleSheet(_qspinbox_stylesheet.value());
            }

            __connect_signals_to_slots();
        }

        Q_SLOT void __attribute__((__noinline__)) on_rslider_move(int _new_value) noexcept {
            // will be signalled to when the red slider is moved
            _slider_values[configs::rgb_offsets::RED] = _new_value;
        }

        Q_SLOT void __attribute__((__noinline__)) on_gslider_move(int _new_value) noexcept {
            // will be signalled to when the green slider is moved
            _slider_values[configs::rgb_offsets::GREEN] = _new_value;
        }

        Q_SLOT void __attribute__((__noinline__)) on_bslider_move(int _new_value) noexcept {
            // will be signalled to when the blue slider is moved
            _slider_values[configs::rgb_offsets::BLUE] = _new_value;
        }

        virtual inline void paintEvent(QPaintEvent* const _paint_event) noexcept override {
            // setting border-radius in stylesheets for QFrame results in botched corners - round corners overlaid on rectangular corners
            // this will create a main window with clean, smooth and round corners
            // https://runebook.dev/en/docs/qt/qwidget/paintEvent

            // https://doc.qt.io/archives/qt-6.1/qwidget.html#paintEvent
            // a paint event is a request to repaint all or part of a widget. It can happen for one of the following reasons:
            // repaint() or update() was invoked, the widget was obscured and has now been uncovered, or many other reasons

            // Qt also tries to speed up painting by merging multiple paint events into one. When update() is called several times or the window system sends several
            // paint events, Qt merges these events into one event with a larger region (see QRegion::united())
            // the repaint() function does not permit this optimization, so we suggest using update() whenever possible
            QPainter _painter { this };
            _painter.setRenderHint(QPainter::RenderHint::Antialiasing); // for smooth corners
            _painter.setBrush(
                QColor { _slider_values[configs::rgb_offsets::RED], _slider_values[configs::rgb_offsets::GREEN], _slider_values[configs::rgb_offsets::BLUE] }
            );
            _painter.setPen(Qt::GlobalColor::transparent); // thin borders

            // when the paint event occurs, the update() region has normally been erased, so you are painting on the widget's background
            // this background can be set using setBackgroundRole() and setPalette()
            __update_background();

            // generally, you should refrain from calling update() or repaint() inside a paintEvent()
            // for example, calling update() or repaint() on children inside a paintEvent() results in undefined behavior; the child may or may not get a paint event.
            QRect _current_rect { rect() };
            _painter.drawRoundedRect(_current_rect, 10, 10);
            QWidget::paintEvent(_paint_event);
        }

        // making a window frameless using Qt::WindowType::FramelessWindowHint also makes us lose all the functionalities afforded by the OS DWM
        // we'll have to hand roll these for our class - exit, move etc.
        // https://stackoverflow.com/questions/37718329/pyqt5-draggable-frameless-window
        virtual inline void mousePressEvent(QMouseEvent* const _event) noexcept override {
            // the default implementation implements the closing of popup widgets when you click outside the window. for other widget types it does nothing
            // we want the window movable(draggable) with mouse
            _mouse_pos = _event->globalPosition(); // store the coordinates of the mouse at click (press)
        }

        virtual inline void mouseMoveEvent(QMouseEvent* const _event) noexcept override {
            // capture the current position of the cursor, and calculate how far the cursor has moved (along x and y axes)
            const auto _distance { _event->globalPosition() - _mouse_pos };
            move(x() + _distance.x(), y() + _distance.y()); // move the window to the new position of the cursor
            _mouse_pos = _event->globalPosition();          // update to the current cursor position
        }

    private:
        inline void __attribute__((__always_inline__)) __connect_signals_to_slots() const noexcept {
            // establishing two way communication between sliders and their corresponding labels
            // connect the QSlider::valueChanged signal of each slider to the SpinBox::setValue slot of their corresponding label
            for (unsigned i = 0; i < _rgbsliders.size(); ++i) connect(&_rgbsliders[i], &QSlider::valueChanged, &_rgbspinboxes[i], &QSpinBox::setValue);
            // connect the QSpinBox::valueChanged signal of each label to the QSlider::setValue slot of their corresponding slider
            for (unsigned i = 0; i < _rgbspinboxes.size(); ++i) connect(&_rgbspinboxes[i], &QSpinBox::valueChanged, &_rgbsliders[i], &QSlider::setValue);

            // establishing one way communication between all the three sliders and the hex string
            connect(&_rgbsliders[configs::rgb_offsets::RED], &QSlider::valueChanged, &_hexstring, &rgb_hexstring::on_rslider_move);
            connect(&_rgbsliders[configs::rgb_offsets::GREEN], &QSlider::valueChanged, &_hexstring, &rgb_hexstring::on_gslider_move);
            connect(&_rgbsliders[configs::rgb_offsets::BLUE], &QSlider::valueChanged, &_hexstring, &rgb_hexstring::on_bslider_move);

            // establishing one way communication between all the three sliders and the main window
            connect(&_rgbsliders[configs::rgb_offsets::RED], &QSlider::valueChanged, this, &main_window::on_rslider_move);
            connect(&_rgbsliders[configs::rgb_offsets::GREEN], &QSlider::valueChanged, this, &main_window::on_gslider_move);
            connect(&_rgbsliders[configs::rgb_offsets::BLUE], &QSlider::valueChanged, this, &main_window::on_bslider_move);
        }

        inline void __attribute__((__always_inline__)) __update_background() noexcept {
            static QPalette _bgpalette {};  // colour palette to paint the main window background with
            static QPalette _txtpalette {}; // contrasting colour palette for texts
            // update the colour palette with the current state of the sliders
            _bgpalette.setColor(
                QPalette::ColorRole::Window,
                QColor { _slider_values[configs::rgb_offsets::RED], _slider_values[configs::rgb_offsets::GREEN], _slider_values[configs::rgb_offsets::BLUE] }
            );
            setPalette(_bgpalette); // set the updated palette, triggering a window redraw

            // handle contrasting the text colour with the background colour
            if (utilities::rgb_to_greyscale(
                    _slider_values[configs::rgb_offsets::RED], _slider_values[configs::rgb_offsets::GREEN], _slider_values[configs::rgb_offsets::BLUE]
                ) > 0.500) {
                _txtpalette.setColor(QPalette::ColorRole::Text, Qt::GlobalColor::black);
                for (unsigned i = 0; i < configs::sliders::N; ++i) _rgbspinboxes[i].setPalette(_txtpalette);
            } else {
                _txtpalette.setColor(QPalette::ColorRole::Text, Qt::GlobalColor::white);
                for (unsigned i = 0; i < configs::sliders::N; ++i) _rgbspinboxes[i].setPalette(_txtpalette);
            }
        }
};

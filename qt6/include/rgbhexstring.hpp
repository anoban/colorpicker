#pragma once
#ifndef __HEXSTRING_HPP
    #define __HEXSTRING_HPP 1
#endif

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

// clang-format off
#include <utilities.hpp>
// clang-format on

#include <cstring>
#include <iostream>

class rgb_hexstring final : public QLineEdit {
        Q_OBJECT

    private:
        std::array<int, configs::sliders::N> _slider_values;
        QString                              _hexstring; // QString is equivalent to std::wstring on Windows where wchar_t is 16 bits wide
        QTextStream                          _hex_strstream;
        Qt::GlobalColor                      _textcolour;

    public:
        inline explicit rgb_hexstring(QWidget* const _parent_window) noexcept :
            QLineEdit { _parent_window }, _slider_values {}, _hexstring {}, _hex_strstream { &_hexstring }, _textcolour { Qt::GlobalColor::white } {
            _hexstring.resize(configs::hexstring::SIZE);
            _hex_strstream.setPadChar('0');                            // pad the hex representation with zeroes to make it two digits when the value is < 16
            _hex_strstream.setFieldAlignment(QTextStream::AlignRight); // when we only have one digit, pad with a zero on the left
            _hex_strstream << Qt::hex                                  // we want the hex representation
                           << Qt::uppercasedigits                      // we want the hex representations in upper case
                           << ::qSetFieldWidth(2);                     // we want fixed width of 2 characters

            // _hex_strstream << "#000000"; // so the hex string widget doesn't show up ampty at application start - doesn't seem to work???
            setGeometry( // since we have control over this class's implementation, let's do this inside the ctor, instead of having main window do this
                configs::sliders::HPAD + configs::sliders::WIDTH + configs::sliders::spinboxes::HPAD + configs::sliders::spinboxes::WIDTH +
                    configs::sliders::spinboxes::HPAD,
                configs::sliders::VSPACE + 2 * configs::sliders::VSPACE_SLIDERS,
                configs::hexstring::WIDTH,
                configs::hexstring::HEIGHT
            );

            setAlignment(Qt::AlignmentFlag::AlignHCenter | Qt::AlignmentFlag::AlignVCenter); // center the text inside the widget
            setReadOnly(true); // disables user input (just act like a QLabel), while allowing copying
            // const auto _qlineedit_stylesheet = utilities::read_qss(R"(./styles/QLineEdit.qss)");
            // if (_qlineedit_stylesheet) setStyleSheet(_qlineedit_stylesheet.value()); // apply the base style with default white text colour
            setStyleSheet(configs::hexstring::WHITETEXT);
        }

        Q_SLOT void __attribute__((__noinline__)) on_rslider_move(int _new_value) noexcept {
            _slider_values[configs::rgb_offsets::RED] = _new_value;
            __update_hexstring();
        }

        Q_SLOT void __attribute__((__noinline__)) on_gslider_move(int _new_value) noexcept {
            _slider_values[configs::rgb_offsets::GREEN] = _new_value;
            __update_hexstring();
        }

        Q_SLOT void __attribute__((__noinline__)) on_bslider_move(int _new_value) noexcept {
            _slider_values[configs::rgb_offsets::BLUE] = _new_value;
            __update_hexstring();
        }

    private:
        inline void __attribute__((__always_inline__)) __update_hexstring() noexcept {
            // static_assert(sizeof(QChar) == sizeof(wchar_t));                   // QChar is equivalent to unsigned short
            // the above requires -fshort-wchar compiler flag because by default, wchar_t is 4 bytes on linux??? not 2 bytes
            // ::memset(_hexstring.data(), 0, sizeof(QChar) * _hexstring.size()); // clean up the buffer before every write
            _hexstring.clear();            // this is what we need to reset the QString before every new write, memset didn't help
            if (!_hex_strstream.seek(0)) { // get to the beginning of the stream buffer
                ::fprintf(stderr, "Error inside %s, call to QTextStream::seek() failed!\n", __PRETTY_FUNCTION__);
                return;
            }

            // https://stackoverflow.com/questions/4784155/how-to-format-a-qstring
            // even if we use two byte wchar_t s, we don't know that there's alternative implementations for all the stdio.h functions to handle 2 byte wchar_t s????
            // https://doc.qt.io/qt-6/qstring.html#asprintf QString::asprintf isn't recommended in new Qt code :(

            // ::printf("QTextStream::pos() is %lld\n", _hex_strstream.pos());
            _hex_strstream << '#' << _slider_values[configs::rgb_offsets::RED] << _slider_values[configs::rgb_offsets::GREEN]
                           << _slider_values[configs::rgb_offsets::BLUE];

            // ::printf("%s\n", _hexstring.toStdString().c_str()); // doesn't work
            // ::puts(_hexstring.toStdString().c_str()); // doesn't work either
            // std::cout << _hexstring.toStdString() << '\n'; // but this works, why?????

            // const auto _stdstr = _hexstring.toStdString();
            // ::puts(_stdstr.c_str()); // still doesn't work!!!
            // ::printf("QString::size() is %lld\n", _hexstring.size()); // the QString keeps growing continuously
            // ::printf("QString has %lld zeroes and the position of '#' is %lld\n", _hexstring.count(QChar { 0 }), _hexstring.indexOf(QChar { '#' }));
            // for (unsigned i = 0; i < _stdstr.size(); ++i)
            //     ::printf("%03u) %d - %c\n", i, _stdstr[i], _stdstr[i]); // looks like the buffer has zeroes at the front
            // the problem is with each << to the QString, through the QTextStream, the string keeps growing, with more and more null bytes at the front

            const auto _needed_textcolour = utilities::text_colour(
                _slider_values[configs::rgb_offsets::RED], _slider_values[configs::rgb_offsets::GREEN], _slider_values[configs::rgb_offsets::BLUE]
            );
            // if we are using a single stylesheet with two different classes, we need to reset the same stylesheet after updating the class, for the text colour change to take effect
            // which is messy, so opting to use separate hardcoded stylesheets instead
            if (_needed_textcolour != _textcolour) {
                switch (_needed_textcolour) {
                    case Qt::GlobalColor::black :
                        _textcolour = Qt::GlobalColor::black; // update the current text colour
                        setStyleSheet(configs::hexstring::BLACKTEXT);
                        break;
                    case Qt::GlobalColor::white :
                        _textcolour = Qt::GlobalColor::white;
                        setStyleSheet(configs::hexstring::WHITETEXT);
                        break;
                    default : ::fputs("only variants Qt::GlobalColor::black and Qt::GlobalColor::white are accepted!\n", stderr); break;
                }
            }

            setText(_hexstring);
        }
};

#pragma once
#ifndef __HEXSTRING_HPP
    #define __HEXSTRING_HPP 1
#endif

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

// clang-format off
#include <config.hpp>
// clang-format on

#include <cstring>

class rgbhexstring final : public QLineEdit {
        Q_OBJECT

    private:
        int         _rslider_value;
        int         _gslider_value;
        int         _bslider_value;
        QString     _hexstring; // QString is equivalent to std::wstring on Windows where wchar_t is 16 bits wide
        QTextStream _hexstrstream;

    public:
        inline rgbhexstring(QWidget* const _parent_window = nullptr) noexcept :
            QLineEdit(_parent_window), _rslider_value {}, _gslider_value {}, _bslider_value {}, _hexstring {}, _hexstrstream { &_hexstring } {
            _hexstring.resize(configs::hexstring::SIZE);
            _hexstrstream.setPadChar('0');                            // pad the hex representation with zeroes to make it two digits when the value is < 16
            _hexstrstream.setFieldAlignment(QTextStream::AlignRight); // when we only have one digit, pad with a zero on the left
            _hexstrstream << Qt::hex                                  // we want the hex representation
                          << Qt::uppercasedigits                      // we want the hex representations in upper case
                          << ::qSetFieldWidth(2);                     // we want fixed width of 2 characters

            setGeometry( // since we have control over this class's implementation, let's do this inside the ctor, instead of having main window do this
                configs::trackbars::PAD + configs::trackbars::WIDTH + configs::trackbars::labels::PAD + configs::trackbars::labels::WIDTH +
                    configs::trackbars::labels::PAD,
                configs::trackbars::VERTICAL_MARGIN + 2 * configs::trackbars::VSPACE,
                configs::hexstring::WIDTH,
                configs::hexstring::HEIGHT
            );
            setAlignment(Qt::AlignmentFlag::AlignHCenter); // not working????
        }

        Q_SLOT inline void rslider_moved(int _new_value) noexcept {
            _rslider_value = _new_value;
            __update_hexstring();
        }

        Q_SLOT inline void gslider_moved(int _new_value) noexcept {
            _gslider_value = _new_value;
            __update_hexstring();
        }

        Q_SLOT inline void bslider_moved(int _new_value) noexcept {
            _bslider_value = _new_value;
            __update_hexstring();
        }

    private:
        inline void __attribute__((__always_inline__)) __update_hexstring() noexcept {
            // static_assert(sizeof(QChar) == sizeof(wchar_t));                   // QChar is equivalent to unsigned short
            // the above requires -fshort-wchar compiler flag because by default, wchar_t is 4 bytes on linux??? not 2 bytes
            ::memset(_hexstring.data(), 0, sizeof(QChar) * _hexstring.size()); // clean up the buffer before every write
            if (!_hexstrstream.seek(0)) {
                fprintf(stderr, "Error inside %s, call to QTextStream::seek() failed!\n", __PRETTY_FUNCTION__);
                return;
            }

            // https://stackoverflow.com/questions/4784155/how-to-format-a-qstring
            // even if we use two byte wchar_t s, we don't know that there's alternative implementations for all the stdio.h functions to handle 2 byte wchar_t s????
            // https://doc.qt.io/qt-6/qstring.html#asprintf QString::asprintf isn't recommended in new Qt code :(

            _hexstrstream << '#' << _rslider_value << _gslider_value << _bslider_value;

            // setText(QString("#%1%2%3").arg(_rslider_value, 2, 16, u'0').arg(_gslider_value, 2, 16, u'0').arg(_bslider_value, 2, 16, u'0'));
            // ::puts(_hexstring.toStdString().c_str());
            setText(_hexstring);
        }
};

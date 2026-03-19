#pragma once
#ifndef __UTILITIES_HPP
    #define __UTILITIES_HPP 1
#endif

#include <cassert>
#include <cstdio>
#include <optional>

#include <config.hpp>
#include <QtCore/QFile>
#include <QtCore/QString>

namespace utilities {

    [[nodiscard]] static inline std::optional<QString> read_qss(const char* const fpath) noexcept(noexcept(QFile {}) && noexcept(QFile {}.readAll())) {
        // using Qt IO functions so we don't need to marshall plain ascii buffers into QChar buffers by hand
        QFile _stylesheet { fpath };
        if (!_stylesheet.open(QIODeviceBase::OpenModeFlag::ReadOnly | QIODeviceBase::OpenModeFlag::Text)) {
            ::fprintf(stderr, "Error inside %s, call to QFile::open() failed!\n", __PRETTY_FUNCTION__);
            return std::nullopt;
        }
        QByteArray _contents = _stylesheet.readAll();
        return QString(_contents);
    }

    [[maybe_unused]] static inline void print_build_info() noexcept {
        ::printf(
            "Colorpicker %s\n"
            "Qt %s\n"
            "g++ %s\n"
            "C++ standard %s",
            COLORPICKER_FULL_VERSION,
            QT_FULL_VERSION,
            CXX_FULL_VERSION,
            CPP_VERSION
        );
    }

} // namespace utilities

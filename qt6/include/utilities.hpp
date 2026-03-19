#pragma once
#ifndef __UTILITIES_HPP
    #define __UTILITIES_HPP 1
#endif

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <new>
#include <optional>

#include <unistd.h>

#include <config.hpp>
#include <QtCore/QString>

// clang-format off
#include <sys/fcntl.h>
#include <sys/stat.h>
// clang-format on

namespace utilities {

    [[nodiscard]] static inline std::optional<QString> read_css(const char* const fpath) noexcept {
        QString     buffer {};
        struct stat filestat {};
        long        nbytes {};

        const int fdesc { ::open(fpath, O_RDONLY) };
        if (fdesc == -1) { // if open() failed, the return value will be -1
            ::fprintf(stderr, "Call to open() failed inside %s at line %d!; errno %d\n", __FUNCTION__, __LINE__, errno);
            return std::nullopt;
        }

        if (::fstat(fdesc, &filestat)) { // if succeeds, 0 is returned, -1 if fails
            ::fprintf(stderr, "Call to fstat() failed inside %s at line %d!; errno %d\n", __FUNCTION__, __LINE__, errno);
            goto CLOSE_AND_RETURN;
        }

        buffer.resize(filestat.st_size);

        if ((nbytes = ::read(fdesc, buffer.data(), filestat.st_size)) != -1) {
            nreadbytes = nbytes;
            assert(nbytes == filestat.st_size); // double checking
        } else {
            ::fprintf(stderr, "Call to read() failed inside %s at line %d!; errno %d\n", __FUNCTION__, __LINE__, errno);
            delete[] buffer;
            buffer = nullptr;
        }
        // then, fall through the CLOSE_AND_RETURN label

CLOSE_AND_RETURN:
        // close() returns 0 on success and -1 on failure
        if (::close(fdesc)) ::fprintf(stderr, "Call to close() failed inside %s at line %d!; errno %d\n", __FUNCTION__, __LINE__, errno);
        return buffer;
    }
} // namespace utilities

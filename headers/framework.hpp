#pragma once
#ifndef __FRAMEWORK_H__
    #define __FRAMEWORK_H__

    #define NOMINMAX            // undef MIN and MAX macros
    #define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
    #define WIN32_EXTRA_MEAN

                                // // Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

    #include <algorithm>
    #include <cstdint>
    #include <cstdlib>

    #include <malloc.h>
    #include <memory.h>
    #include <SDKDDKVer.h>
    #include <tchar.h>
    #include <windows.h>

#endif // !__FRAMEWORK_H__

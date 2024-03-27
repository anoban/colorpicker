#pragma once
#ifndef __COLORPICKER_H__
    #define __COLORPICKER_H__
    #define _AMD64_
    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_MEAN

    #if (!(defined __clang__)) || (__STDC_VERSION__ != 202311L) // as of now, only clang supports C23 nullptr and attributes
        #define nullptr ((void*) (0))                           // not feeling good about this though :(
    #endif
// #define _WIN32_IE_ 0x0600

// clang-format off
    #include <windef.h>
    #include <resource.h>
    #include <dwmapi.h> // Desktop Window Manager header, implements dark mode features.
    #include <SDKDDKVer.h>
    #include <strsafe.h>
    #include <string.h>
    #include <tchar.h>
    #include <wingdi.h>
    #include <WinUser.h>
    #include <windowsx.h>
    #include <CommCtrl.h>
    #include <math.h>
// clang-format on

    #pragma comment(lib, "Dwmapi.lib")
    #pragma comment(lib, "Gdi32.lib")
    #pragma comment(lib, "User32.lib")
    #pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK WindowHandler(HWND hParentWindow, UINT message, WPARAM wParam, LPARAM lParam);

#endif // !__COLORPICKER_H__

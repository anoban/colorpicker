#pragma once
#ifndef __COLORPICKER_H__
    #define __COLORPICKER_H__

    #include <framework.h>
    #include <resource.h>

// prototypes of procs in handlers.cpp

LRESULT CALLBACK ScrollHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // !__COLORPICKER_H__

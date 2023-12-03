#pragma once
#ifndef __COLORPICKER_H__
    #define __COLORPICKER_H__

    #include <resource.hpp>
    #include <framework.hpp>

// prototypes of procs in handlers.cpp

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ScrollHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // !__COLORPICKER_H__

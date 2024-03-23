#include <colorpicker.h>

#define VSPACE_TRACKBARS             40LLU // space subsequent track bars at this vertical distances
#define NTRACKBARS                   3     // number of track bars used in the application

#define TRACKBAR_WIDTH               200LLU
#define TRACKBAR_HEIGHT              25LLU
#define TRACKBAR_LEFTPAD             20LLU
#define TRACKBARGRID_VERTICAL_MARGIN 85LLU

#define TRACKBAR_LABEL_LEFTPAD       20LLU
#define TRACKBAR_LABEL_WIDTH         50LLU
#define TRACKBAR_LABEL_HEIGHT        30LLU

#define HEXSTRING_TEXTBOX_WIDTH      120LLU
#define HEXSTRING_TEXTBOX_HEIGHT     30LLU

// globals defined in main.c
extern HINSTANCE hApplicationInst;
extern INT32     idFocus;
extern WNDPROC   oldScroll[3];

// static inline constexpr COLORREF AlphaBlend(_In_ const COLORREF rgbColor, _In_ const float alpha)  { }

LRESULT CALLBACK ScrollHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    INT64 iId = GetWindowLongPtrW(hWnd, GWLP_ID);
    switch (message) {
        case WM_KEYDOWN :
            if (wParam == VK_TAB) SetFocus(GetDlgItem(GetParent(hWnd), (iId + (GetKeyState(VK_SHIFT) < 0 ? 2 : 1)) % 3));
            break;
        case WM_SETFOCUS : idFocus = iId; break;
        default          : break;
    }
    return CallWindowProcW(oldScroll[iId], hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static COLORREF       crPrim[3];
    static COLORREF       crefTitleBar; // color of the title bar

    static HBRUSH         hBrush[3], hStaticBrush;

    static HWND           hTrackBars[NTRACKBARS];     // track bars
    static HWND           hTrackBarLabel[NTRACKBARS]; // track bar labels
    static HWND           hTextBox;                   // the text box that shows the hex representation of the RGB colour of choice

    // iTrackBarCaret is an array of
    static INT32          iTrackBarCaret[NTRACKBARS], cyChar;
    static RECT           rcColor;

    // needs to be in the static memory since these are used even when this callback isn't invoked
    static WCHAR          wszHexColourString[8]; // the hex string shown inside the text box

    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-logfonta
    // defining a logical font object to override the fugly default font
    static const LOGFONTW lfFontLato = { .lfHeight         = 0, // font mapper uses a default height value when it searches for a match.
                                         .lfWidth          = 0,
                                         .lfEscapement     = 0,
                                         .lfOrientation    = 0,
                                         .lfWeight         = FW_BOLD, // bold font weight
                                         .lfItalic         = FALSE,
                                         .lfUnderline      = FALSE,
                                         .lfStrikeOut      = FALSE,
                                         .lfCharSet        = ANSI_CHARSET, // we don't need any non-ASCII alphabets, so
                                         .lfOutPrecision   = OUT_TT_PRECIS,
                                         .lfClipPrecision  = CLIP_DEFAULT_PRECIS,
                                         .lfQuality        = ANTIALIASED_QUALITY,
                                         .lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS,
                                         .lfFaceName       = L"Lato" }; // assumes Lato is installed on the system
    // handle to the the materialized logical font object
    const HFONT           hfLato     = CreateFontIndirectW(&lfFontLato);

    INT32                 i = 0, iClientWindowWidth = 0, iClientWindowHeight = 0;

    // needs to be in the static memory since these are used even wehn the callback isn't invoked
    static WCHAR          wszTrackBarLabelText[5]; // the decimal colour value diplayed next to the track bars
    static const BOOL     bUseDarkMode = TRUE;     // make the title bar dark

    INITCOMMONCONTROLSEX  CommCtrlEx   = { .dwSize = sizeof(INITCOMMONCONTROLSEX), .dwICC = ICC_BAR_CLASSES };
    InitCommonControlsEx(&CommCtrlEx);

    switch (message) {
        case WM_CREATE :
            {
                SetMenu(hWnd, nullptr); // hide the menu bar

                // https://learn.microsoft.com/en-us/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute
                // enable immersive dark mode, where the colour of the title bar becomes customizeable
                DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &bUseDarkMode, sizeof(BOOL));
                // set the colour of title bar to the same colour as the client window (stored in crefTitleBar)
                DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &crefTitleBar, sizeof(COLORREF));

                hTextBox = CreateWindowExW( // the text window that displays the hex RGB string
                    WS_EX_CLIENTEDGE,
                    L"edit",
                    nullptr,
                    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPED,
                    TRACKBAR_LEFTPAD * 2 + TRACKBAR_WIDTH + TRACKBAR_LABEL_LEFTPAD + TRACKBAR_LABEL_WIDTH,
                    VSPACE_TRACKBARS * 2 + TRACKBARGRID_VERTICAL_MARGIN, // make the text box vertically align with the last track bar
                    HEXSTRING_TEXTBOX_WIDTH,
                    HEXSTRING_TEXTBOX_HEIGHT,
                    hWnd,
                    (HMENU) (10),
                    hApplicationInst,
                    nullptr
                );

                // creation of the three track bars, their labels and their corresponding label texts
                for (i = 0; i < NTRACKBARS; ++i) {
                    hTrackBars[i] = CreateWindowExW(
                        0L,
                        TRACKBAR_CLASSW, // horizontal track bar (WinGDI calls horizontal bars with a tracking cursor track bars)
                        nullptr,
                        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_FIXEDLENGTH | WS_OVERLAPPED,
                        TRACKBAR_LEFTPAD,
                        i * VSPACE_TRACKBARS + TRACKBARGRID_VERTICAL_MARGIN,
                        // + 25 padding because we don't want the first track bar being really close to the title bar
                        TRACKBAR_WIDTH,
                        TRACKBAR_HEIGHT,
                        hWnd,
                        (HMENU) (i),
                        hApplicationInst,
                        nullptr
                    );

                    SendMessageW(hTrackBars[i], TBM_SETRANGE, TRUE, MAKELONG(0, 255));
                    SendMessageW(hTrackBars[i], TBM_SETPAGESIZE, 0, 20);
                    SendMessageW(hTrackBars[i], TBM_SETTICFREQ, 20, 0);
                    SendMessageW(hTrackBars[i], TBM_SETPOS, TRUE, 0);

                    hTrackBarLabel[i] = CreateWindowExW(
                        0L,
                        L"static",
                        L"0",
                        WS_CHILD | WS_VISIBLE | SS_CENTER,
                        TRACKBAR_LEFTPAD + TRACKBAR_WIDTH + TRACKBAR_LABEL_LEFTPAD,
                        i * VSPACE_TRACKBARS + TRACKBARGRID_VERTICAL_MARGIN,
                        TRACKBAR_LABEL_WIDTH,
                        TRACKBAR_LABEL_HEIGHT,
                        hWnd,
                        (HMENU) (i + 3),
                        hApplicationInst,
                        nullptr
                    );

                    // set the customized font for use in the track bar label texts
                    SendMessageW(hTrackBarLabel[i], WM_SETFONT, (WPARAM) hfLato, TRUE);

                    oldScroll[i] = (WNDPROC) (SetWindowLongPtrW(
                        hTrackBars[i],
                        GWLP_WNDPROC,
                        (uintptr_t /* Petzold's example uses a LONG (4 bytes) here, that throws an SEH access violation on x64 machines */)
                            ScrollHandler
                    ));
                    hBrush[i]    = CreateSolidBrush(crPrim[i]);
                }

                hStaticBrush = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
                cyChar       = HIWORD(GetDialogBaseUnits());

                // override the default font with the customized one for the text box
                SendMessageW(hTextBox, WM_SETFONT, (WPARAM) hfLato, TRUE);

                break;
            }

        case WM_SIZE : // when the window is resized
            {
                // low-order word of lParam specifies the new width of the client area.
                // high-order word of lParam specifies the new height of the client area.

                iClientWindowWidth  = LOWORD(lParam);
                iClientWindowHeight = HIWORD(lParam);

                SetRect(&rcColor, 0, 0, iClientWindowWidth, iClientWindowHeight);

                // for (i = 0; i < 3; ++i) {
                //     MoveWindow(hTrackBars[i], (2 * i + 1) * cxClient / 14, 2 * cyChar, cxClient / 14, cyClient - 4 * cyChar, TRUE);
                //     MoveWindow(hTrackBarLabel[i], (4 * i + 1) * cxClient / 28, cyChar / 2, cxClient / 7, cyChar, TRUE);
                //     MoveWindow(hTrackBarLabelText[i], (4 * i + 1) * cxClient / 28, cyChar / 2, cxClient / 7, cyChar, TRUE);
                // }

                SetFocus(hWnd);
                break;
            }

        case WM_SETFOCUS :
            {
                SetFocus(hTrackBars[idFocus]);
                break;
            }

        case WM_HSCROLL :                                        // when the horizontal track bars are adjusted,
            {
                i = GetWindowLongPtrW((HWND) (lParam), GWLP_ID); // capture which track bar was adjusted

                switch
                    LOWORD(wParam) {
                        case TB_PAGEDOWN      : iTrackBarCaret[i] += 20; // fallthrough
                        case TB_LINEDOWN      : iTrackBarCaret[i] = min(255, iTrackBarCaret[i] + 1); break;
                        case TB_PAGEUP        : iTrackBarCaret[i] -= 20; // fallthrough
                        case TB_LINEUP        : iTrackBarCaret[i] = max(0, iTrackBarCaret[i] - 1); break;
                        case TB_TOP           : iTrackBarCaret[i] = 0; break;
                        case TB_BOTTOM        : iTrackBarCaret[i] = 255; break;
                        case TB_THUMBPOSITION : // fallthrough
                        case TB_THUMBTRACK    : iTrackBarCaret[i] = HIWORD(wParam); break;
                        default               : break;
                    }

                // if the horizontal adjustment came from the keyboard, move the slider to appropriate position
                SetScrollPos(hTrackBars[i], SB_CTL, iTrackBarCaret[i], TRUE);

                // if a slider is moved, use the new position of the slider to update the label texts
                StringCbPrintfExW(
                    wszTrackBarLabelText,
                    sizeof(wszTrackBarLabelText), // in bytes
                    (STRSAFE_LPWSTR*) (wszTrackBarLabelText + __crt_countof(wszTrackBarLabelText)),
                    nullptr,
                    STRSAFE_FILL_ON_FAILURE | STRSAFE_FILL_BEHIND_NULL,
                    L"%3d",
                    iTrackBarCaret[i]
                );
                SetWindowTextW(hTrackBarLabel[i], wszTrackBarLabelText);

                DeleteObject((HBRUSH) (SetClassLongPtrW(
                    hWnd, GCLP_HBRBACKGROUND, (uintptr_t) (CreateSolidBrush(RGB(iTrackBarCaret[0], iTrackBarCaret[1], iTrackBarCaret[2])))
                )));

                crefTitleBar = RGB(iTrackBarCaret[0], iTrackBarCaret[1], iTrackBarCaret[2]);
                // DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &crTitleBar, sizeof(COLORREF));
                // the line above colours the window borders, not the complete title bar.

                // this line colours the title bar
                DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &crefTitleBar, sizeof(COLORREF));

                // update the hex colour code
                StringCbPrintfExW(
                    wszHexColourString,
                    sizeof(wszHexColourString) /* in bytes */,
                    (STRSAFE_LPWSTR*) (wszHexColourString + 8) /* end of buffer */,
                    nullptr,
                    STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE,
                    L"#%02X%02X%02X",
                    iTrackBarCaret[0],
                    iTrackBarCaret[1],
                    iTrackBarCaret[2]
                );
                SetWindowTextW(hTextBox, wszHexColourString);

                InvalidateRect(hWnd, &rcColor, TRUE);
                break;
            }

        case WM_DESTROY :
            {
                DeleteObject((HBRUSH) (SetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND, (uintptr_t) (GetStockObject(WHITE_BRUSH)))));
                DeleteObject(hStaticBrush);

                for (i = 0; i < 3; ++i) DeleteObject(hBrush[i]);

                PostQuitMessage(0);
                break;
            }

        default : return DefWindowProcW(hWnd, message, wParam, lParam);
    }

    return 0;
}

#include <colorpicker.h>

#define VSPACE_TRACKBARS             40LL // space subsequent track bars at this vertical distances
#define NTRACKBARS                   3LL  // number of track bars used in the application

#define TRACKBAR_WIDTH               200LL
#define TRACKBAR_HEIGHT              25LL
#define TRACKBAR_LEFTPAD             20LL // padding between the left end of the track bars and the left edge of the client window
#define TRACKBARGRID_VERTICAL_MARGIN 85LL // margin betwen the first track bar and the bottom edge of the title bar

#define TRACKBAR_LABEL_LEFTPAD       20LL // padding between the right end of the track bar and the left edge of it's cognate label box
#define TRACKBAR_LABEL_WIDTH         50LL
#define TRACKBAR_LABEL_HEIGHT        30LL

#define HEXSTRING_TEXTBOX_WIDTH      120LL
#define HEXSTRING_TEXTBOX_HEIGHT     30LL

#define PAGE_UPDOWN_STEP             17LL // distance to move the slider on the track bar, when page-up or page-down keys were pressed

// globals defined in main.c
extern HINSTANCE hApplicationInst;
extern INT32     iFocusedItemId;
extern WNDPROC   oldScroll[3];

LRESULT CALLBACK WindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static COLORREF       crPrim[3];
    static COLORREF       crefWindowBackground; // color of the title bar

    static HBRUSH         hBrush[3], hStaticBrush;

    static HWND           hTrackBars[NTRACKBARS];
    static HWND           hTrackBarLabel[NTRACKBARS]; // track bar labels
    static HWND           hTextBox;                   // the text box that shows the hex representation of the RGB colour of choice

    // iTrackBarCaret is an array of integers
    static INT32          iTrackBarSliderPos[NTRACKBARS];

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

    INT64                 i = 0, iMovedTrackbarId = 0;

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
                DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &crefWindowBackground, sizeof(COLORREF));

                hTextBox = CreateWindowExW( // the text window that displays the hex RGB string
                    WS_EX_CLIENTEDGE,
                    L"edit",
                    nullptr,
                    WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | SS_CENTER,
                    // SS_CENTER overlays the text at the center of the textbox
                    TRACKBAR_LEFTPAD * 2 + TRACKBAR_WIDTH + TRACKBAR_LABEL_LEFTPAD + TRACKBAR_LABEL_WIDTH,
                    VSPACE_TRACKBARS * 2 + TRACKBARGRID_VERTICAL_MARGIN, // make the text box vertically align with the last track bar
                    HEXSTRING_TEXTBOX_WIDTH,
                    HEXSTRING_TEXTBOX_HEIGHT,
                    hWnd,
                    (HMENU) (10), // id = 10
                    hApplicationInst,
                    nullptr
                );

                // override the default font with the customized one for the text box
                SendMessageW(hTextBox, WM_SETFONT, (WPARAM) hfLato, TRUE);

                // creation of the three track bars, their labels and their corresponding label texts
                for (i = 0; i < NTRACKBARS; ++i) {
                    hTrackBars[i] = CreateWindowExW(
                        0L,
                        TRACKBAR_CLASSW, // horizontal track bar (WinGDI calls horizontal bars with a tracking cursor track bars)
                        nullptr,
                        WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | TBS_AUTOTICKS | TBS_HORZ,
                        TRACKBAR_LEFTPAD,
                        i * VSPACE_TRACKBARS + TRACKBARGRID_VERTICAL_MARGIN,
                        // + 25 padding because we don't want the first track bar being really close to the title bar
                        TRACKBAR_WIDTH,
                        TRACKBAR_HEIGHT,
                        hWnd,
                        (HMENU) (i), // ids = 0 - 2
                        hApplicationInst,
                        nullptr
                    );

                    // messages prefixed with TBM_* are related to track bar controls
                    SendMessageW(hTrackBars[i], TBM_SETRANGE, TRUE /* redraw window */, MAKELONG(0, 255));
                    // the slider range from 0 to 255
                    SendMessageW(hTrackBars[i], TBM_SETPAGESIZE, FALSE, PAGE_UPDOWN_STEP);
                    // the number of logical positions the trackbar's slider moves in response to keyboard input, such as the or keys, or
                    // mouse input, such as clicks in the trackbar's channel
                    SendMessageW(hTrackBars[i], TBM_SETTICFREQ, TRUE, PAGE_UPDOWN_STEP);
                    // set the spaces between adjacent ticks
                    SendMessageW(hTrackBars[i], TBM_SETPOS, TRUE, 0);
                    // set the slider position at window launch

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
                        (HMENU) (i + 3), // ids = 3 - 5
                        hApplicationInst,
                        nullptr
                    );

                    // set the customized font for use in the track bar label texts
                    SendMessageW(hTrackBarLabel[i], WM_SETFONT, (WPARAM) hfLato, TRUE);

                    hBrush[i] = CreateSolidBrush(crPrim[i]);
                }

                hStaticBrush = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));

                break;
            }

        case WM_SETFOCUS :
            {
                SetFocus(hTrackBars[iFocusedItemId]);
                break;
            }

        case WM_HSCROLL : // when the horizontal track bars are adjusted,
            {
                iMovedTrackbarId = GetWindowLongPtrW((HWND) (lParam), GWLP_ID /* Retrieves the identifier of the window. */);
                // capture which track bar was adjusted in the variable i

                switch
                    LOWORD(wParam) {
                            // TB_LINEUP, TB_LINEDOWN cases hanlde mouse scrolls
                        case TB_LINEDOWN : iTrackBarSliderPos[iMovedTrackbarId] = min(255, iTrackBarSliderPos[iMovedTrackbarId]); break;
                        case TB_LINEUP :
                            iTrackBarSliderPos[iMovedTrackbarId] = max(0, iTrackBarSliderPos[iMovedTrackbarId]);
                            break;
                            // TB_TOP, TB_BOTTOM cases handle strokes of Home and End buttons
                        case TB_TOP           : iTrackBarSliderPos[iMovedTrackbarId] = 0; break;
                        case TB_BOTTOM        : iTrackBarSliderPos[iMovedTrackbarId] = 255; break;

                        case TB_THUMBPOSITION : // fallthrough to TB_THUMBTRACK
                        case TB_THUMBTRACK    : iTrackBarSliderPos[iMovedTrackbarId] = HIWORD(wParam); break;
                        default               : break;        // TB_PAGEUP, TB_PAGEDOWN
                    }

                // if the horizontal adjustment came from the keyboard, move the slider to appropriate position
                SendMessageW(hTrackBars[iMovedTrackbarId], TBM_SETPOS, TRUE, iTrackBarSliderPos[iMovedTrackbarId]);

                // if a slider is moved, use the new position of the slider to update the label texts
                StringCbPrintfExW(
                    wszTrackBarLabelText,
                    sizeof(wszTrackBarLabelText), // in bytes
                    (STRSAFE_LPWSTR*) (wszTrackBarLabelText + __crt_countof(wszTrackBarLabelText)),
                    nullptr,
                    STRSAFE_FILL_ON_FAILURE | STRSAFE_FILL_BEHIND_NULL,
                    L"%3d",
                    iTrackBarSliderPos[iMovedTrackbarId]
                );
                SetWindowTextW(hTrackBarLabel[iMovedTrackbarId], wszTrackBarLabelText);

                DeleteObject((HBRUSH) (SetClassLongPtrW(
                    hWnd,
                    GCLP_HBRBACKGROUND,
                    (uintptr_t) (CreateSolidBrush(RGB(iTrackBarSliderPos[0], iTrackBarSliderPos[1], iTrackBarSliderPos[2])))
                )));

                crefWindowBackground = RGB(iTrackBarSliderPos[0], iTrackBarSliderPos[1], iTrackBarSliderPos[2]);

                // this line colours the title bar
                DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &crefWindowBackground, sizeof(COLORREF));

                // update the hex colour code
                StringCbPrintfExW(
                    wszHexColourString,
                    sizeof(wszHexColourString) /* in bytes */,
                    (STRSAFE_LPWSTR*) (wszHexColourString + 8) /* end of buffer */,
                    nullptr,
                    STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE,
                    L"#%02X%02X%02X",
                    iTrackBarSliderPos[0],
                    iTrackBarSliderPos[1],
                    iTrackBarSliderPos[2]
                );
                SetWindowTextW(hTextBox, wszHexColourString);

                InvalidateRect( // trigger a whole window redraw
                    hWnd,
                    nullptr,    // nullptr because we want the entire client area to be redrawn
                    TRUE        // erase the background too
                );

                break;
            }

        case WM_DESTROY :
            {
                DeleteObject((HBRUSH) (SetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND, (uintptr_t) (GetStockObject(WHITE_BRUSH)))));
                DeleteObject(hStaticBrush);

                for (i = 0; i < 3; ++i) DeleteObject(hBrush[i]);
                DeleteObject(hfLato);
                PostQuitMessage(0);
                break;
            }

        default : return DefWindowProcW(hWnd, message, wParam, lParam);
    }

    return 0;
}

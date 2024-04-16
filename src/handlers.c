#include <colorpicker.h>

#define VSPACE_TRACKBARS             40LL // space subsequent track bars at this vertical distances
#define NTRACKBARS                   3LL  // number of track bars used in the application

#define TRACKBAR_WIDTH               260LL
#define TRACKBAR_HEIGHT              25LL
#define TRACKBAR_LEFTPAD             20LL  // padding between the left end of the track bars and the left edge of the client window
#define TRACKBARGRID_VERTICAL_MARGIN 150LL // margin betwen the first track bar and the bottom edge of the title bar

#define TRACKBAR_LABEL_LEFTPAD       20LL  // padding between the right end of the track bar and the left edge of it's cognate label box
#define TRACKBAR_LABEL_WIDTH         50LL
#define TRACKBAR_LABEL_HEIGHT        30LL

#define HEXSTRING_TEXTBOX_WIDTH      120LL
#define HEXSTRING_TEXTBOX_HEIGHT     30LL

#define PAGE_UPDOWN_STEP             17LL // distance to move the slider on the track bar, when page-up or page-down keys were pressed

// globals defined in main.c
extern HINSTANCE hApplicationInst;
extern HINSTANCE hPickerToolInstance;
extern INT32     iFocusedItemId;
extern HFONT     hfLato;

typedef enum {
    ID_TRACKBAR_RED,
    ID_TRACKBAR_GREEN,
    ID_TRACKBAR_BLUE,
    ID_TRACKBAR_RED_EDITBOX,
    ID_TRACKBAR_GREEN_EDITBOX,
    ID_TRACKBAR_BLUE_EDITBOX,
    ID_HEXSTRING_EDITBOX,
    ID_STAYONTOP_BUTTON,
    ID_LAUNCHPICKERTOOL_BUTTON
} ELEMENTID;

// DO NOT DO ANYTHING HEAVY INSIDE THE WINDOW PROCEDURE!!
LRESULT CALLBACK WindowHandler(_In_ HWND hParentWindow, _In_ const UINT message, _In_ const WPARAM wParam, _In_ const LPARAM lParam) {
    static COLORREF   crefWindowBackground; // color of the title bar

    static HBRUSH     hOldBrush;

    static HWND       hTrackBars[NTRACKBARS];
    static HWND       hTrackBarLabel[NTRACKBARS];
    static HWND       hTextBox; // the text box that shows the hex representation of the RGB colour of choice
    static HWND       hStayOnTopButton;
    static HWND       hLaunchPickerToolButton;

    // iTrackBarCaret is an array of integers
    static INT32      iTrackBarSliderPos[NTRACKBARS];

    // needs to be in the static memory since these are used even when this callback isn't invoked
    static WCHAR      wszHexColourString[12]; // the hex string shown inside the text box

    static INT64      i = 0, iMovedTrackbarId = 0;

    // needs to be in the static memory since these are used even wehn the callback isn't invoked
    static WCHAR      wszTrackBarLabelText[5]; // the decimal colour value diplayed next to the track bars
    static WCHAR      wszUserInput[10];        // buffer to receive user inputs from the edit boxes
    static const BOOL bUseDarkMode = TRUE;     // make the title bar dark
    static BOOL       bSliderMoved;
    static WORD       wChangedEditControlId = 0, wCognateTrackbarId = 0;
    static HWND       hChangedEditBox;

    switch (message) {
        case WM_CREATE :
            {
                SetMenu(hParentWindow, nullptr); // hide the menu bar

                // https://learn.microsoft.com/en-us/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute
                // enable immersive dark mode, where the colour of the title bar becomes customizeable
                DwmSetWindowAttribute(hParentWindow, DWMWA_USE_IMMERSIVE_DARK_MODE, &bUseDarkMode, sizeof(BOOL));
                // set the colour of title bar to the same colour as the client window (stored in crefTitleBar)
                DwmSetWindowAttribute(hParentWindow, DWMWA_BORDER_COLOR, &crefWindowBackground, sizeof(COLORREF));

                hTextBox                = CreateWindowExW( // the text window that displays the hex RGB string
                    0L,
                    L"EDIT",
                    nullptr,
                    WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_BORDER | SS_CENTER,
                    // SS_CENTER overlays the text at the center of the textbox
                    TRACKBAR_LEFTPAD * 2 + TRACKBAR_WIDTH + TRACKBAR_LABEL_LEFTPAD + TRACKBAR_LABEL_WIDTH,
                    VSPACE_TRACKBARS * 2 + TRACKBARGRID_VERTICAL_MARGIN, // make the text box vertically align with the last track bar
                    HEXSTRING_TEXTBOX_WIDTH,
                    HEXSTRING_TEXTBOX_HEIGHT,
                    hParentWindow,
                    (HMENU) ID_HEXSTRING_EDITBOX,
                    hApplicationInst,
                    nullptr
                );

                hStayOnTopButton        = CreateWindowExW( // the button to anchor the window on top of all other windows on screen
                    0L,
                    L"BUTTON",
                    nullptr,
                    WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_BORDER | BS_PUSHBUTTON | BS_BITMAP | BS_ICON,
                    TRACKBAR_LEFTPAD * 2 + TRACKBAR_WIDTH + TRACKBAR_LABEL_LEFTPAD + TRACKBAR_LABEL_WIDTH,
                    VSPACE_TRACKBARS + TRACKBARGRID_VERTICAL_MARGIN, // make the text box vertically align with the last track bar
                    30,
                    HEXSTRING_TEXTBOX_HEIGHT,
                    hParentWindow,
                    (HMENU) ID_STAYONTOP_BUTTON,
                    hApplicationInst,
                    nullptr
                );

                hLaunchPickerToolButton = CreateWindowExW( // the button that launches the screen colour picker tool
                    0L,
                    L"BUTTON",
                    nullptr,
                    WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_BORDER | BS_PUSHBUTTON | BS_BITMAP | BS_ICON,
                    TRACKBAR_LEFTPAD * 4 + TRACKBAR_WIDTH + TRACKBAR_LABEL_LEFTPAD + TRACKBAR_LABEL_WIDTH,
                    VSPACE_TRACKBARS + TRACKBARGRID_VERTICAL_MARGIN, // make the text box vertically align with the last track bar
                    30,
                    HEXSTRING_TEXTBOX_HEIGHT,
                    hParentWindow,
                    (HMENU) ID_LAUNCHPICKERTOOL_BUTTON,
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
                        hParentWindow,
                        (HMENU) (ID_TRACKBAR_RED + i), // 0 - 2 THE PARENTHESIS AROUND ID_TRACKBAR_RED + i IS CRITICAL
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
                        L"EDIT",
                        nullptr,
                        WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
                        TRACKBAR_LEFTPAD + TRACKBAR_WIDTH + TRACKBAR_LABEL_LEFTPAD,
                        i * VSPACE_TRACKBARS + TRACKBARGRID_VERTICAL_MARGIN,
                        TRACKBAR_LABEL_WIDTH,
                        TRACKBAR_LABEL_HEIGHT,
                        hParentWindow,
                        (HMENU) (ID_TRACKBAR_RED_EDITBOX + i), // 3 - 5
                        hApplicationInst,
                        nullptr
                    );

                    // set the customized font for use in the track bar label texts
                    SendMessageW(hTrackBarLabel[i], WM_SETFONT, (WPARAM) hfLato, TRUE);
                }

                return DefWindowProcW(hParentWindow, message, wParam, lParam);
            } // END CASE WM_CREATE

        case WM_HSCROLL :                // when the horizontal track bars are adjusted,
            {
                bSliderMoved     = TRUE; // register that the even was triggered by a slider move
                // lParam gives the handle to the scroll bar that sent the WM_HSCROLL message
                iMovedTrackbarId = GetWindowLongPtrW((HWND) (lParam), GWLP_ID);
                // capture which track bar was adjusted in the variable iMovedTrackbarId
#ifdef _DEBUG
                if (iMovedTrackbarId > 2) MessageBoxW(nullptr, L"iMovedTrackbarId > 2", nullptr, MB_OK);
#endif // DEBUG

                switch
                    LOWORD(wParam) {
                            // https://learn.microsoft.com/en-us/cpp/mfc/slider-notification-messages?view=msvc-170
                            // slider control sends the TB_BOTTOM, TB_LINEDOWN, TB_LINEUP, and TB_TOP notification codes only when
                            // the user interacts with a slider control by using the keyboard
                        case TB_LINEDOWN :
                            iTrackBarSliderPos[iMovedTrackbarId] = min(255, iTrackBarSliderPos[iMovedTrackbarId] + 1);
                            break;
                            // TB_LINEDOWN - RIGHT ARROW OR DOWN ARROW

                        case TB_LINEUP :
                            iTrackBarSliderPos[iMovedTrackbarId] = max(0, iTrackBarSliderPos[iMovedTrackbarId] - 1);
                            break;
                            // TB_LINEUP - LEFT ARROW OR UP ARROW

                        case TB_TOP : iTrackBarSliderPos[iMovedTrackbarId] = 0; break; // Home button

                        case TB_BOTTOM :                                               // End button
                            iTrackBarSliderPos[iMovedTrackbarId] = 255;
                            break;

                            // TB_THUMBPOSITION and TB_THUMBTRACK notification messages are only sent when the user is using the mouse
                        case TB_THUMBPOSITION : // fallthrough to TB_THUMBTRACK

                        case TB_THUMBTRACK :
                            // HIWORD specifies the current position of the slider only if the LOWORD is TB_THUMBPOSITION or TB_THUMBTRACK.
                            iTrackBarSliderPos[iMovedTrackbarId] = HIWORD(wParam);
                            break;

                            // TB_ENDTRACK, TB_PAGEDOWN and TB_PAGEUP notification codes are sent in both cases
                        case TB_PAGEUP :
                            iTrackBarSliderPos[iMovedTrackbarId] = max(0, iTrackBarSliderPos[iMovedTrackbarId] - PAGE_UPDOWN_STEP);
                            break;
                            // TB_PAGEUP - user clicked the channel to the left of the slider or hit the Page Up key

                        case TB_PAGEDOWN :
                            iTrackBarSliderPos[iMovedTrackbarId] = min(255, iTrackBarSliderPos[iMovedTrackbarId] + PAGE_UPDOWN_STEP);
                            break;
                            // TB_PAGEDOWN - user clicked the channel to the RIGHT of the slider
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

                // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setclasslongptrw
                // SetClassLongPtrW first finds the member of the WNDCLASSW structure specified by the nIndex argument
                // nIndex specifies the offset of the select member in the WNDCLASSW struct (the offsets must be negative integers)
                // for WNDCLASSEXW, these values must be positive offsets
                // and upon success, it returns the previously stored value in that struct member.

                crefWindowBackground = RGB(iTrackBarSliderPos[0], iTrackBarSliderPos[1], iTrackBarSliderPos[2]);

                hOldBrush            = SetClassLongPtrW( // create a new brush, stores it in the WNDCLASSEXW of parent window
                                              // and captures the handle of the old brush in the variable hStaticBrush
                    hParentWindow,
                    GCLP_HBRBACKGROUND,
                    // handle to the newly created brush
                    (uintptr_t) CreateSolidBrush(crefWindowBackground)
                );
                InvalidateRect( // trigger a whole window redraw
                    hParentWindow,
                    nullptr,    // nullptr because we want the entire client area to be redrawn
                    TRUE        // erase the background too
                );
                DeleteObject(hOldBrush);   // give up the old brush

                // this line colours the title bar
                DwmSetWindowAttribute(hParentWindow, DWMWA_CAPTION_COLOR, &crefWindowBackground, sizeof(COLORREF));

                // update the hex colour code
                StringCbPrintfExW(
                    wszHexColourString,
                    sizeof(wszHexColourString) /* in bytes */,
                    (STRSAFE_LPWSTR) (wszHexColourString + __crt_countof(wszHexColourString)) /* end of buffer */,
                    nullptr,
                    STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE,
                    L"#%02X%02X%02X", // #RRGGBB
                    iTrackBarSliderPos[0],
                    iTrackBarSliderPos[1],
                    iTrackBarSliderPos[2]
                );
                SetWindowTextW(hTextBox, wszHexColourString);
                bSliderMoved = FALSE; // before case break, set this flag to false
                break;
            } // END CASE WM_HSCROLL

        case WM_SETFOCUS :
            {
                iFocusedItemId = GetWindowLongPtrW((HWND) (lParam), GWLP_ID);
#ifdef _DEBUG
                if (iFocusedItemId > 2) MessageBoxW(nullptr, L"iFocusedItemId > 2", nullptr, MB_OK);
#endif // _DEBUG
                SetFocus(hTrackBars[iFocusedItemId]);
                break;
            }

        case WM_COMMAND :
            {
                if (!bSliderMoved) {     // only when the user has typed in something in the edit boxes,
                    switch (HIWORD(wParam)) {
                        case EN_UPDATE : // when the text in edit boxes have been altered
                            wChangedEditControlId = LOWORD(wParam);
                            wCognateTrackbarId    = wChangedEditControlId - 3;
                            hChangedEditBox       = lParam;
                            // capture the updated state of the edit box
                            GetWindowTextW(hChangedEditBox, wszUserInput, __crt_countof(wszUserInput));
                            // sanitization needed to differentiate hexstring edits from label edits
                            iTrackBarSliderPos[wCognateTrackbarId] = wcstoul(wszUserInput, wszUserInput + __crt_countof(wszUserInput), 10);
                            iTrackBarSliderPos[wCognateTrackbarId] =
                                iTrackBarSliderPos[wCognateTrackbarId] > 255 ? 255 : iTrackBarSliderPos[wCognateTrackbarId];
                            // MessageBoxW(nullptr, wszUserInput, L"", MB_OK);
                            SendMessageW(
                                hTrackBars[wCognateTrackbarId] /* TOdo: verify range 0 - 2*/,
                                TBM_SETPOS,
                                TRUE,
                                iTrackBarSliderPos[wCognateTrackbarId]
                            );

                            crefWindowBackground = RGB(iTrackBarSliderPos[0], iTrackBarSliderPos[1], iTrackBarSliderPos[2]);

                            hOldBrush            = SetClassLongPtrW( // create a new brush, stores it in the WNDCLASSEXW of parent window
                                                          // and captures the handle of the old brush in the variable hStaticBrush
                                hParentWindow,
                                GCLP_HBRBACKGROUND,
                                // handle to the newly created brush
                                (uintptr_t) CreateSolidBrush(crefWindowBackground)
                            );
                            InvalidateRect( // trigger a whole window redraw
                                hParentWindow,
                                nullptr,    // nullptr because we want the entire client area to be redrawn
                                TRUE        // erase the background too
                            );
                            DeleteObject(hOldBrush);   // give up the old brush

                            // this line colours the title bar
                            DwmSetWindowAttribute(hParentWindow, DWMWA_CAPTION_COLOR, &crefWindowBackground, sizeof(COLORREF));

                            // update the hex colour code
                            StringCbPrintfExW(
                                wszHexColourString,
                                sizeof(wszHexColourString) /* in bytes */,
                                (STRSAFE_LPWSTR*) (wszHexColourString + __crt_countof(wszHexColourString)) /* end of buffer */,
                                nullptr,
                                STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE,
                                L"#%02X%02X%02X", // #RRGGBB
                                iTrackBarSliderPos[0],
                                iTrackBarSliderPos[1],
                                iTrackBarSliderPos[2]
                            );
                            SetWindowTextW(hTextBox, wszHexColourString);
                            bSliderMoved = FALSE;
                            break;

                        default : break;
                    }
                }
                break;
            } // END CASE WM_COMMAND

        case WM_DESTROY :
            {
                DeleteObject(hOldBrush);
                DeleteObject(hfLato);
                PostQuitMessage(0);
                break;
            }

        default : return DefWindowProcW(hParentWindow, message, wParam, lParam);
    }

    return 0;
}

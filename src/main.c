#include <colorpicker.h>

#define MAX_LOADSTRING 100LLU

HINSTANCE hInst                         = { 0 }; // current instance
WCHAR     szTitle[MAX_LOADSTRING]       = { 0 }; // The title bar text
WCHAR     szWindowClass[MAX_LOADSTRING] = { 0 }; // the main window class name

INT32     idFocus                       = 0;
WNDPROC   oldScroll[3]                  = { 0 }; // function pointers

static ATOM __stdcall RegisterWindowClassExtW(HINSTANCE hInstance) {
    WNDCLASSEXW wcex   = { 0 };
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WindowHandler;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COLORPICKER));
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(COLOR_WINDOW);
    wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_COLORPICKER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

static BOOL __stdcall InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst     = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowExW(
        WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE | WS_EX_STATICEDGE | WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME,
        szWindowClass,
        szTitle,
        WS_SYSMENU | WS_THICKFRAME | WS_TILED | WS_VISIBLE | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        470,
        240,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COLORPICKER, szWindowClass, MAX_LOADSTRING);
    RegisterWindowClassExtW(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) return FALSE;

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_COLORPICKER));

    MSG    msg;

    // Main message loop:
    while (GetMessageW(&msg, NULL, 0, 0)) {
        if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return (int) msg.wParam;
}

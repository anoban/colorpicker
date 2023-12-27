#include <colorpicker.hpp>

constexpr std::size_t MAX_LOADSTRING { 100 };

HINSTANCE             hInst {};                         // current instance
WCHAR                 szTitle[MAX_LOADSTRING] {};       // The title bar text
WCHAR                 szWindowClass[MAX_LOADSTRING] {}; // the main window class name

INT32                 idFocus {};
WNDPROC               oldScroll[3] {};

[[msvc::forceinline]] static ATOM __stdcall RegisterWindowClassExtW(HINSTANCE hInstance) noexcept {
    WNDCLASSEXW wcex {};

    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WindowHandler;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COLORPICKER));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = ::CreateSolidBrush(COLOR_WINDOW);
    wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_COLORPICKER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return ::RegisterClassExW(&wcex);
}

[[msvc::forceinline]] static BOOL __stdcall InitInstance(HINSTANCE hInstance, int nCmdShow) noexcept {
    hInst     = hInstance; // Store instance handle in our global variable

    HWND hWnd = ::CreateWindowExW(
        WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE | WS_EX_STATICEDGE | WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME,
        szWindowClass,
        szTitle,
        WS_SYSMENU | WS_THICKFRAME | WS_TILED | WS_VISIBLE | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        470,
        240,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd) return FALSE;

    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);

    return TRUE;
}

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance, _In_opt_ [[maybe_unused]] HINSTANCE hPrevInstance, _In_ [[maybe_unused]] LPWSTR lpCmdLine, _In_ int nCmdShow
) {
    // Initialize global strings
    ::LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    ::LoadStringW(hInstance, IDC_COLORPICKER, szWindowClass, MAX_LOADSTRING);
    RegisterWindowClassExtW(hInstance);

    // Perform application initialization:
    if (!::InitInstance(hInstance, nCmdShow)) return FALSE;

    HACCEL hAccelTable = ::LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_COLORPICKER));

    MSG    msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!::TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg)) {
            ::TranslateMessage(&msg);
            ::DispatchMessageW(&msg);
        }
    }

    return (int) msg.wParam;
}
#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Register the window class
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSW wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    // Create the window
    HWND hwnd = CreateWindowExW(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        L"Image Viewer",                // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL) {
        GdiplusShutdown(gdiplusToken);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}

Gdiplus::Bitmap* gdiBitmap = NULL;

bool FileExists(const wchar_t* fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (gdiBitmap) {
                Graphics graphics(hdc);
                graphics.DrawImage(gdiBitmap, 0, 0);
            }

            EndPaint(hwnd, &ps);
        }
        return 0;

        case WM_CREATE:
        {
            const wchar_t* imagePath = L"image.bmp";
            
            if (!FileExists(imagePath)) {
                MessageBoxW(hwnd, L"Image file does not exist!", L"Error", MB_OK | MB_ICONERROR);
                return -1;
            }

            gdiBitmap = new Gdiplus::Bitmap(imagePath);
            if (gdiBitmap->GetLastStatus() != Ok) {
                delete gdiBitmap;
                gdiBitmap = NULL;
                MessageBoxW(hwnd, L"Could not load image!", L"Error", MB_OK | MB_ICONERROR);
            }
        }
        return 0;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

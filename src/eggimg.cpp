// eggimg.cpp : Defines the entry point for the application.
//

#include "glad/glad.h"
#include <Windows.h>
#include "eggimg.h"



#define WIN32_LEAN_AND_MEAN

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HDC windowHandleToDeviceContext;

void Draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SwapBuffers(windowHandleToDeviceContext);
}

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"eggimg Window";

    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = CLASS_NAME;
    wc.style = CS_OWNDC;

    if (!RegisterClass(&wc))
        return 1;

    // Create the window.
    
    HWND hwnd = CreateWindowExW(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"eggimg",    // Window text
        WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    
    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        Draw();
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    RECT rect;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
            PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
            32,                   // Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                   // Number of bits for the depthbuffer
            8,                    // Number of bits for the stencilbuffer
            0,                    // Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        windowHandleToDeviceContext = GetDC(hwnd);

        int letWindowsChooseThisPixelFormat;
        letWindowsChooseThisPixelFormat = ChoosePixelFormat(windowHandleToDeviceContext, &pfd);
        SetPixelFormat(windowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd);
        HGLRC ourOpenGLRenderingContext = wglCreateContext(windowHandleToDeviceContext);

        BOOL ret = wglMakeCurrent(windowHandleToDeviceContext, ourOpenGLRenderingContext);
        if (!gladLoadGL()) {
            MessageBoxA(0, "Failed to initialize GLAD!", "Error", MB_ICONERROR);
            return -1;
        }

        char* string = (char*)glGetString(GL_VERSION);
        MessageBoxA(hwnd, string, "OPENGL VERSION", 0);
        GetClientRect(hwnd, &rect);
        //wglDeleteContext(ourOpenGLRenderingContext);
    }
        break;
    case WM_PAINT:
        BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default: 
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}



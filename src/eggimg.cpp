// eggimg.cpp : Defines the entry point for the application.
//

#include "glad/glad.h"
#include <Windows.h>
#include "eggimg.h"
#include <iostream>
#include <cassert>
#include "file.h"



#define WIN32_LEAN_AND_MEAN

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HDC windowHandleToDeviceContext;

//vertex shader
static const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position = vec4(aPosition, 0.0, 1.0);
}
)";
// fragment shader
static const char* fragmentShaderSource = R"(
#version 330 core

in vec2 vTexCoord;

uniform sampler2D uTexture;

out vec4 fragColor;

void main()
{
    //fragColor = texture(uTexture, vTexCoord);
	fragColor = vec4(vTexCoord.x,vTexCoord.y,1.0,1.0);
}
)";

float verts[] =
{
	-1.f,1.f,
	-1.f,-1.f,
	1.f,-1.f,

	-1.f,1.f,
	1.f,-1.f,
	1.f,1.f
};

float uvs[] =
{
	0.f,1.f,
	0.f,0.f,
	1.f,0.f,

	0.f,1.f,
	1.f,0.f,
	1.f,1.f
};

GLuint vbo;
GLuint vao;
GLuint shaderProgram;

//TODO use element array buffers
void GenerateBoxMesh(void)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void GenerateShader(void)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	assert(status == GL_TRUE && "vertex shader failed to compile");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	assert(status == GL_TRUE && "fragment shader failed to compile");

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

}

void Draw(void)
{
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	SwapBuffers(windowHandleToDeviceContext);
}

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

#ifdef EGGIMG_DEBUG
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
#endif

	// get command line arg
	if (pCmdLine[0])
	{
		std::wcout << "cmdLine arg: " << pCmdLine << std::endl;
		ei_OpenFile(pCmdLine);
	}

	
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"eggimg Window";

	WNDCLASS wc = { 0 };

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
#ifdef EGGIMG_DEBUG
	fclose(stdout);
	fclose(stdin);
	FreeConsole();
#endif

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
		GenerateBoxMesh();
		GenerateShader();
		char* string = (char*)glGetString(GL_VERSION);
		std::cout << "OPENGL VERSION: " << string << std::endl;
		//MessageBoxA(hwnd, string, "OPENGL VERSION", 0);
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




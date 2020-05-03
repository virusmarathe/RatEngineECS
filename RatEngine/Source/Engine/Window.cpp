#include "Window.h"

Window::Window() : m_hwnd(NULL), m_running(false)
{
}

Window::~Window()
{
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			// event when window is created
			Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
			window->setHWND(hwnd);
			window->onCreate();
			break;
		}
		case WM_SETFOCUS:
		{
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onFocus();
			break;
		}
		case WM_KILLFOCUS:
		{
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onKillFocus();
			break;
		}
		case WM_DESTROY:
		{
			// event when window is destroyed
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onDestroy();
			PostQuitMessage(0);
			break;
		}
		default:
		{
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}

	return NULL;
}

bool Window::init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	m_hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
							wc.lpszClassName,
							L"Rat Game",
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							1024,
							768,
							NULL,
							NULL,
							NULL,
							this);

	if (!m_hwnd)
	{
		return false;
	}

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	// window is running
	m_running = true;

	return true;
}

bool Window::broadcast()
{
	MSG msg;

	this->onUpdate();

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);

	return false;
}

bool Window::release()
{
	if (m_hwnd)
	{
		if (!DestroyWindow(m_hwnd))
		{
			return false;
		}
	}

	return true;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	return rc;
}

void Window::onCreate()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
}

void Window::onDestroy()
{
	m_running = false;
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
}

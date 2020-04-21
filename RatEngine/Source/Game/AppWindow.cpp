#include "AppWindow.h"

AppWindow::AppWindow() : Window(), m_SwapChain(NULL)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	GraphicsEngine::get()->init();
	m_SwapChain = GraphicsEngine::get()->createSwapChain();
	RECT rc = getClientWindowRect();
	m_SwapChain->init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
}

void AppWindow::onUpdate()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(m_SwapChain, 1, 1, 0, 1);

	m_SwapChain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_SwapChain->release();
	GraphicsEngine::get()->release();
}

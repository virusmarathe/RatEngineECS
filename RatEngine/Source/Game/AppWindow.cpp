#include "AppWindow.h"
#include <Windows.h>
#include "Vector3.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include <iostream>

struct vector4
{
	float r, g, b, a;
};

struct vertex
{
	Vector3 position;
	vector4 color;
	vector4 color1;
};

_declspec(align(16))
struct constant
{
	constant() :m_Time(0) {}

	Matrix4x4 m_World;
	Matrix4x4 m_View;
	Matrix4x4 m_Projection;
	unsigned int m_Time;
};

AppWindow::AppWindow() : Window(), m_SwapChain(NULL), m_VertexBuffer(NULL), m_VertexShader(NULL), m_PixelShader(NULL),
						 m_ConstantBuffer(NULL), m_PrevFrameTime(0), m_DeltaTime(0), m_IndexBuffer(NULL)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::update()
{
	DeviceContext* context = GraphicsEngine::get()->getImmediateDeviceContext();
	constant data;
	RECT rect = getClientWindowRect();
	data.m_Time = GetTickCount();

	data.m_World.setIdentity();

	Matrix4x4 worldCam;
	worldCam.setIdentity();
	Matrix4x4 temp;
	temp.setIdentity();
	temp.setRotationX(m_XRot);
	worldCam *= temp;
	temp.setIdentity();
	temp.setRotationY(m_YRot);
	worldCam *= temp;
	float speed = 2.0f;
	Vector3 pos = m_CameraTransform.position() + worldCam.forward() * (m_ForwardDirection * speed * m_DeltaTime);
	pos = pos + worldCam.right() * (m_RightDirection * speed * m_DeltaTime);
	temp.setIdentity();
	temp.setTranslation(pos);
	worldCam *= temp;

	m_CameraTransform = worldCam;

	worldCam.inverse();

	data.m_View = worldCam;

	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);
	data.m_Projection.setPerspectiveFovLH(1.5708f, ((float)width) / ((float)height), 0.1f, 100.0f);

	m_ConstantBuffer->update(context, &data);
}

void AppWindow::onCreate()
{
	GraphicsEngine::get()->init();
	m_SwapChain = GraphicsEngine::get()->createSwapChain();
	RECT rc = getClientWindowRect();
	m_SwapChain->init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	InputSystem::get()->showCursor(false);

	m_CameraTransform.setIdentity();
	m_CameraTransform.setTranslation(Vector3(0, 0, -2));

	vertex list[] =
	{
		{Vector3(-0.5f, -0.5f, -0.5f),	1.0f,  0.0f, 0.0f, 1.0f,	0.4f,  1.0f, 0.0f, 1.0f},
		{Vector3(-0.5f, 0.5f, -0.5f),	0.0f, 1.0f, 0.0f, 1.0f,		0.0f,  0.3f, 1.0f, 1.0f},
		{Vector3(0.5f, 0.5f, -0.5f),	0.0f, 1.0f, 0.0f, 1.0f,		0.2f,  0.0f, 0.4f, 1.0f},
		{Vector3(0.5f, -0.5f, -0.5f),	1.0f, 0.0f, 0.0f, 1.0f,		1.0f,  1.0f, 1.0f, 1.0f},
		{Vector3(0.5f, -0.5f, 0.5f),	1.0f,  0.0f, 0.0f, 1.0f,	0.4f,  1.0f, 0.0f, 1.0f},
		{Vector3(0.5f, 0.5f, 0.5f),		0.0f, 1.0f, 0.0f, 1.0f,		0.0f,  0.3f, 1.0f, 1.0f},
		{Vector3(-0.5f, 0.5f, 0.5f),	0.0f, 1.0f, 0.0f, 1.0f,		0.2f,  0.0f, 0.4f, 1.0f},
		{Vector3(-0.5f, -0.5f, 0.5f),	1.0f, 0.0f, 0.0f, 1.0f,		1.0f,  1.0f, 1.0f, 1.0f}
	};
	m_VertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	UINT listSize = ARRAYSIZE(list);

	UINT indexList[] = 
	{
		0, 1 ,2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		1, 6, 5,
		5, 2, 1,
		7, 0, 3,
		3, 4, 7,
		3, 2, 5,
		5, 4, 3,
		7, 6, 1,
		1, 0, 7
	};
	m_IndexBuffer = GraphicsEngine::get()->createIndexBuffer();
	m_IndexBuffer->load(indexList, ARRAYSIZE(indexList));

	void* shaderByteCode = nullptr;
	SIZE_T shaderSize = 0;

	GraphicsEngine::get()->compileVertexShader(L"Source/Shaders/VertexShader.hlsl", "vsmain", &shaderByteCode, &shaderSize);
	m_VertexShader = GraphicsEngine::get()->createVertexShader(shaderByteCode, shaderSize);
	m_VertexBuffer->load(list, sizeof(vertex), listSize, shaderByteCode, shaderSize);

	GraphicsEngine::get()->compilePixelShader(L"Source/Shaders/PixelShader.hlsl", "psmain", &shaderByteCode, &shaderSize);
	m_PixelShader = GraphicsEngine::get()->createPixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::get()->releaseCompiledShader();

	constant data;
	data.m_Time = 0;

	m_ConstantBuffer = GraphicsEngine::get()->createConstantBuffer();
	m_ConstantBuffer->load(&data, sizeof(constant));

	m_PrevFrameTime = GetTickCount();
}

void AppWindow::onUpdate()
{
	InputSystem::get()->update();

	DWORD curTime = GetTickCount();
	m_DeltaTime = (curTime - m_PrevFrameTime) / 1000.0f;
	m_PrevFrameTime = curTime;

	DeviceContext* context = GraphicsEngine::get()->getImmediateDeviceContext();

	context->clearRenderTargetColor(m_SwapChain, 0.1f, 0.1f, 0.6f, 1);

	RECT rc = this->getClientWindowRect();
	context->setViewportSize((FLOAT)(rc.right - rc.left), (FLOAT)(rc.bottom - rc.top));

	update();

	context->setConstantBuffer(m_PixelShader, m_ConstantBuffer);
	context->setConstantBuffer(m_VertexShader, m_ConstantBuffer);

	context->setVertexShader(m_VertexShader);
	context->setPixelShader(m_PixelShader);
	context->setVertexBuffer(m_VertexBuffer);
	context->setIndexBuffer(m_IndexBuffer);
	context->drawIndexedTriangleList(m_IndexBuffer->getNumIndices(), 0, 0);

	m_SwapChain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_VertexBuffer->release();
	m_IndexBuffer->release();
	m_ConstantBuffer->release();
	m_VertexShader->release();
	m_PixelShader->release();
	m_SwapChain->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

#pragma region InputListener

void AppWindow::onKeyDown(int key)
{
	float speed = 1.0f;
	if (key == 'W')
	{
		m_ForwardDirection = 1.0f;
	}
	else if (key == 'S')
	{
		m_ForwardDirection = -1.0f;
	}
	if (key == 'A')
	{
		m_RightDirection = -1.0f;
	}
	else if (key == 'D')
	{
		m_RightDirection = 1.0f;
	}
}

void AppWindow::onKeyUp(int key)
{
	if (m_ForwardDirection != 0 && (key == 'W' || key == 'S'))
	{
		m_ForwardDirection = 0.0f;
	}

	if (m_RightDirection != 0 && (key == 'A' || key == 'D'))
	{
		m_RightDirection = 0.0f;
	}
}

void AppWindow::onMouseMove(const Point& mousePos)
{
	RECT rect = getClientWindowRect();
	int xCenter = (rect.right + rect.left) / 2.0f;
	int yCenter = (rect.bottom - rect.top) / 2.0f;

	m_XRot += (mousePos.Y - yCenter) * m_DeltaTime * 0.2f;
	m_YRot += (mousePos.X - xCenter) * m_DeltaTime * 0.2f;

	InputSystem::get()->setCursorPosition(Point(xCenter, yCenter));
}

void AppWindow::onLeftMouseDown(const Point& mousePos)
{
}

void AppWindow::onLeftMouseUp(const Point& mousePos)
{
}

void AppWindow::onRightMouseDown(const Point& mousePos)
{
}

void AppWindow::onRightMouseUp(const Point& mousePos)
{
}

void AppWindow::onMiddleMouseDown(const Point& mousePos)
{
}

void AppWindow::onMiddleMouseUp(const Point& mousePos)
{
}

#pragma endregion

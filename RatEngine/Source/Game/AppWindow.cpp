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
						 m_ConstantBuffer(NULL), m_PrevFrameTime(0), m_DeltaTime(0), m_LerpTimer(0), m_LerpDuration(10),
						 m_IndexBuffer(NULL), m_ScaleLerpTimer(0)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::updateQuadPosition()
{
	DeviceContext* context = GraphicsEngine::get()->getImmediateDeviceContext();
	constant data;
	RECT rect = getClientWindowRect();
	data.m_Time = GetTickCount();

	m_LerpTimer += m_DeltaTime;
	m_ScaleLerpTimer += m_DeltaTime;
	float lerpVal = m_LerpTimer / m_LerpDuration;
	float lerpScaleVal = m_ScaleLerpTimer / 0.5f;

	if (lerpVal > 1)
	{
		lerpVal = 1;
		m_LerpTimer = 0;
	}

	Matrix4x4 temp;
	data.m_World.setScale(Vector3(m_CubeScale, m_CubeScale, m_CubeScale));

	temp.setIdentity();
	temp.setRotationZ(0);
	data.m_World *= temp;
	temp.setIdentity();
	temp.setRotationY(m_YRot);  
	data.m_World *= temp;
	temp.setIdentity();
	temp.setRotationX(m_XRot);
	data.m_World *= temp;

	temp.setTranslation(Vector3(m_XPos, m_YPos, 0));
	data.m_World *= temp;

	data.m_View.setIdentity();
	data.m_Projection.setOrthoLH((rect.right - rect.left)/300.0f, (rect.bottom - rect.top)/300.0f, -4.0f, 4.0f);


	m_ConstantBuffer->update(context, &data);
}

void AppWindow::onCreate()
{
	GraphicsEngine::get()->init();
	m_SwapChain = GraphicsEngine::get()->createSwapChain();
	RECT rc = getClientWindowRect();
	m_SwapChain->init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

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

	updateQuadPosition();

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
		m_YPos += speed * m_DeltaTime;
	}
	else if (key == 'S')
	{
		m_YPos -= speed * m_DeltaTime;
	}
	if (key == 'A')
	{
		m_XPos -= speed * m_DeltaTime;
	}
	else if (key == 'D')
	{
		m_XPos += speed * m_DeltaTime;
	}
}

void AppWindow::onKeyUp(int key)
{
}

void AppWindow::onMouseMove(const Point& deltaMousePos)
{
	m_XRot -= deltaMousePos.Y * m_DeltaTime;
	m_YRot -= deltaMousePos.X * m_DeltaTime;
}

void AppWindow::onLeftMouseDown(const Point& mousePos)
{
	m_CubeScale += 0.1f;
}

void AppWindow::onLeftMouseUp(const Point& mousePos)
{
}

void AppWindow::onRightMouseDown(const Point& mousePos)
{
	m_CubeScale -= 0.1f;
}

void AppWindow::onRightMouseUp(const Point& mousePos)
{
}

void AppWindow::onMiddleMouseDown(const Point& mousePos)
{
	m_CubeScale = 1.0f;
}

void AppWindow::onMiddleMouseUp(const Point& mousePos)
{
}

#pragma endregion

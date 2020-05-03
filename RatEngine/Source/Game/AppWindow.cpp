#include "AppWindow.h"
#include <Windows.h>
#include "Vector3.h"
#include "Vector2.h"
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
	Vector2 texcoord;
	vector4 color;
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
	DeviceContext* context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();
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
	Window::onCreate();

	RECT rc = getClientWindowRect();
	m_SwapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	InputSystem::get()->showCursor(false);

	m_WoodTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/wood.jpg");

	m_CameraTransform.setIdentity();
	m_CameraTransform.setTranslation(Vector3(0, 0, -2));

	Vector3 position_list[] =
	{
		{ Vector3(-0.5f,-0.5f,-0.5f)},
		{ Vector3(-0.5f,0.5f,-0.5f) },
		{ Vector3(0.5f,0.5f,-0.5f) },
		{ Vector3(0.5f,-0.5f,-0.5f)},

		//BACK FACE
		{ Vector3(0.5f,-0.5f,0.5f) },
		{ Vector3(0.5f,0.5f,0.5f) },
		{ Vector3(-0.5f,0.5f,0.5f)},
		{ Vector3(-0.5f,-0.5f,0.5f) }
	};

	Vector2 texcoord_list[] =
	{
		{ Vector2(0.0f,0.0f) },
		{ Vector2(0.0f,1.0f) },
		{ Vector2(1.0f,0.0f) },
		{ Vector2(1.0f,1.0f) }
	};

	vertex list[] =
	{
		{position_list[0],	texcoord_list[1],	0.2f,  0.2f, 0.2f, 1.0f},
		{position_list[1],	texcoord_list[0],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[2],	texcoord_list[2],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[3],	texcoord_list[3],	0.2f,  0.2f, 0.2f, 1.0f},

		{position_list[4],	texcoord_list[1],	0.2f,  0.2f, 0.2f, 1.0f},
		{position_list[5],	texcoord_list[0],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[6],	texcoord_list[2],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[7],	texcoord_list[3],	0.2f,  0.2f, 0.2f, 1.0f},

		{position_list[1],	texcoord_list[1],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[6],	texcoord_list[0],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[5],	texcoord_list[2],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[2],	texcoord_list[3],	1.0f,  1.0f, 1.0f, 1.0f},

		{position_list[7],	texcoord_list[1],	0.2f,  0.2f, 0.2f, 1.0f},
		{position_list[0],	texcoord_list[0],	0.2f,  0.2f, 0.2f, 1.0f},
		{position_list[3],	texcoord_list[2],	0.2f,  0.2f, 0.2f, 1.0f},
		{position_list[4],	texcoord_list[3],	0.2f,  0.2f, 0.2f, 1.0f},

		{position_list[3],	texcoord_list[1],	0.2f,  0.2f, 0.2f, 1.0f},
		{position_list[2],	texcoord_list[0],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[5],	texcoord_list[2],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[4],	texcoord_list[3],	0.2f,  0.2f, 0.2f, 1.0f},

		{position_list[7],	texcoord_list[1],	0.2f,  0.2f, 0.2f, 1.0f},
		{position_list[6],	texcoord_list[0],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[1],	texcoord_list[2],	1.0f,  1.0f, 1.0f, 1.0f},
		{position_list[0],	texcoord_list[3],	0.2f,  0.2f, 0.2f, 1.0f}
	};
	UINT listSize = ARRAYSIZE(list);

	UINT indexList[] = 
	{
		0, 1 ,2,
		2, 3, 0,
		
		4, 5, 6,
		6, 7, 4,
		
		8, 9, 10,
		10, 11, 8,
		
		12, 13, 14,
		14, 15, 12,
		
		16, 17, 18,
		18, 19, 16,
		
		20, 21, 22,
		22, 23, 20
	};
	m_IndexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(indexList, ARRAYSIZE(indexList));

	void* shaderByteCode = nullptr;
	SIZE_T shaderSize = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"Source/Shaders/VertexShader.hlsl", "vsmain", &shaderByteCode, &shaderSize);
	m_VertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(list, sizeof(vertex), listSize, shaderByteCode, shaderSize);
	m_VertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shaderByteCode, shaderSize);

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"Source/Shaders/PixelShader.hlsl", "psmain", &shaderByteCode, &shaderSize);
	m_PixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant data;
	data.m_Time = 0;

	m_ConstantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&data, sizeof(constant));

	m_PrevFrameTime = GetTickCount();
}

void AppWindow::onUpdate()
{
	InputSystem::get()->update();

	DWORD curTime = GetTickCount();
	m_DeltaTime = (curTime - m_PrevFrameTime) / 1000.0f;
	m_PrevFrameTime = curTime;

	DeviceContext* context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();

	context->clearRenderTargetColor(m_SwapChain, 0.1f, 0.1f, 0.6f, 1);

	RECT rc = this->getClientWindowRect();
	context->setViewportSize((FLOAT)(rc.right - rc.left), (FLOAT)(rc.bottom - rc.top));

	update();

	context->setConstantBuffer(m_PixelShader, m_ConstantBuffer);
	context->setConstantBuffer(m_VertexShader, m_ConstantBuffer);

	context->setVertexShader(m_VertexShader);
	context->setPixelShader(m_PixelShader);

	context->setTexture(m_PixelShader, m_WoodTexture);

	context->setVertexBuffer(m_VertexBuffer);
	context->setIndexBuffer(m_IndexBuffer);
	context->drawIndexedTriangleList(m_IndexBuffer->getNumIndices(), 0, 0);

	m_SwapChain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	delete m_VertexBuffer;
	delete m_IndexBuffer;
	delete m_ConstantBuffer;
	delete m_VertexShader;
	delete m_PixelShader;
	delete m_SwapChain;
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
	int xCenter = (int)((rect.right + rect.left) / 2.0f);
	int yCenter = (int)((rect.bottom - rect.top) / 2.0f);

	float sensitivity = 0.001f;
	m_XRot += (mousePos.Y - yCenter) * sensitivity;
	m_YRot += (mousePos.X - xCenter) * sensitivity;

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

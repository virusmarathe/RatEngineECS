#include "AppWindow.h"
#include <Windows.h>
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include <iostream>
#include "Mesh.h"

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
	Matrix4x4 m_World;
	Matrix4x4 m_View;
	Matrix4x4 m_Projection;
	Vector4 m_LightDirection;
	Vector3 m_CameraPosition;
};

AppWindow::AppWindow() : Window(), m_SwapChain(NULL), m_PixelShader(NULL),
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

	Matrix4x4 lightRotationMatrix;
	lightRotationMatrix.setIdentity();
	lightRotationMatrix.setRotationY(m_LightRotY);
	m_LightRotY += 0.707f * m_DeltaTime;
	data.m_LightDirection = lightRotationMatrix.forward();

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
	data.m_CameraPosition = pos;

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

	m_WoodTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/brick.png");
	m_TeapotMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets/Meshes/statue.obj");

	m_CameraTransform.setIdentity();
	m_CameraTransform.setTranslation(Vector3(0, 0, -1));

	void* shaderByteCode = nullptr;
	SIZE_T shaderSize = 0;

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"Source/Shaders/PixelShader.hlsl", "psmain", &shaderByteCode, &shaderSize);
	m_PixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant data;
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
	context->setConstantBuffer(m_TeapotMesh->getVertexShader(), m_ConstantBuffer);

	context->setVertexShader(m_TeapotMesh->getVertexShader());
	context->setPixelShader(m_PixelShader);

	context->setTexture(m_PixelShader, m_WoodTexture);

	context->setVertexBuffer(m_TeapotMesh->getVertexBuffer());
	context->setIndexBuffer(m_TeapotMesh->getIndexBuffer());
	context->drawIndexedTriangleList(m_TeapotMesh->getIndexBuffer()->getNumIndices(), 0, 0);

	m_SwapChain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	delete m_IndexBuffer;
	delete m_ConstantBuffer;
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

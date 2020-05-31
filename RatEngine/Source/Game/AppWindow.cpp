#include "AppWindow.h"
#include <Windows.h>
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include <iostream>
#include "Debug.h"

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

AppWindow::AppWindow() : Window(), m_SwapChain(NULL)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::update()
{
	Matrix4x4 lightRotationMatrix;
	lightRotationMatrix.setIdentity();
	lightRotationMatrix.setRotationY(m_LightRotY);
	m_LightRotY += 0.707f * m_DeltaTime;
	meshRendererSystem.m_LightDirection = lightRotationMatrix.forward();

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
	meshRendererSystem.m_CameraTransform = m_CameraTransform;

	ecs.getComponent<TransformComponent>(skybox)->transform.setTranslation(m_CameraTransform.position());
}

void AppWindow::onCreate()
{
	Window::onCreate();

	RECT rc = getClientWindowRect();
	m_SwapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	InputSystem::get()->showCursor(false);

	m_CameraTransform.setIdentity();
	m_CameraTransform.setTranslation(Vector3(0, 0, -1));

	MeshRendererComponent comp;
	TransformComponent trans;
	SimpleMotionComponent motionComp;

	comp.mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets/Meshes/statue.obj");
	comp.m_Texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/wood.jpg");
	trans.transform.setIdentity();
	trans.transform.setTranslation(Vector3(1.0f, -0.1f, -1.5f));
	statue = ecs.makeEntity(trans, comp);

	comp.mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets/Meshes/teapot.obj");
	comp.m_Texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/brick.png");
	trans.transform.setIdentity();
	trans.transform.setTranslation(Vector3(-1.5f, 0, 0));
	motionComp.velocity = Vector3(1.0f, 0.0f, 0.0f);
	teapot = ecs.makeEntity(trans, comp, motionComp);

	comp.mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets/Meshes/sphere.obj");
	comp.m_Texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/sky.jpg");
	comp.backFaceCulled = false;
	trans.transform.setIdentity();
	trans.transform.setScale(Vector3(100.0f, 100.0f, 100.0f));
	skybox = ecs.makeEntity(trans, comp);

	mainSystems.addSystem(meshRendererSystem);
	mainSystems.addSystem(simpleMotionSystem);

	void* shaderByteCode = nullptr;
	SIZE_T shaderSize = 0;

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"Source/Shaders/PixelShader.hlsl", "psmain", &shaderByteCode, &shaderSize);
	ecs.getComponent<MeshRendererComponent>(statue)->pixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, shaderSize);
	ecs.getComponent<MeshRendererComponent>(teapot)->pixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"Source/Shaders/UnlitTextureShader.hlsl", "psmain", &shaderByteCode, &shaderSize);
	ecs.getComponent<MeshRendererComponent>(skybox)->pixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant data;
	ecs.getComponent<MeshRendererComponent>(statue)->constantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&data, sizeof(constant));
	ecs.getComponent<MeshRendererComponent>(teapot)->constantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&data, sizeof(constant));
	ecs.getComponent<MeshRendererComponent>(skybox)->constantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&data, sizeof(constant));

	meshRendererSystem.clientWindowRect = rc;
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	DeviceContext* context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();

	context->clearRenderTargetColor(m_SwapChain, 0.1f, 0.1f, 0.6f, 1);

	RECT rc = this->getClientWindowRect();
	context->setViewportSize((FLOAT)(rc.right - rc.left), (FLOAT)(rc.bottom - rc.top));

	float teapotXPos = ecs.getComponent<TransformComponent>(teapot)->transform.position().x;
	float teapotXVel = ecs.getComponent<SimpleMotionComponent>(teapot)->velocity.x;

	if (teapotXPos > 2.0f && teapotXVel > 0)
	{
		ecs.getComponent<SimpleMotionComponent>(teapot)->velocity = Vector3(-1.0f, 0.0f, 0.0f);
	}
	else if (teapotXPos <= -2.0f && teapotXVel < 0)
	{
		ecs.getComponent<SimpleMotionComponent>(teapot)->velocity = Vector3(1.0f, 0.0f, 0.0f);
	}

	update();
	ecs.updateSystems(mainSystems, m_DeltaTime);

	m_SwapChain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
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

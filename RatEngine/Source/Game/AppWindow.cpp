#include "AppWindow.h"
#include <Windows.h>
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include <iostream>
#include "Debug.h"

AppWindow::AppWindow() : Window(), m_SwapChain(NULL)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::update()
{
	meshRendererSystem.m_LightDirection = ecs.getComponent<TransformComponent>(directionalLight)->transform.forward();

	meshRendererSystem.m_CameraTransform = ecs.getComponent<TransformComponent>(camera)->transform;

	ecs.getComponent<TransformComponent>(skybox)->transform.setTranslation(ecs.getComponent<TransformComponent>(camera)->transform.position());

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
}

void AppWindow::onCreate()
{
	Window::onCreate();

	RECT rc = getClientWindowRect();
	m_SwapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	InputSystem::get()->showCursor(false);

	MeshRendererComponent comp;
	TransformComponent trans;
	SimpleMotionComponent motionComp;
	RotateTimerComponent rotComp;
	FlyCamComponent flyCamComp;

	rotComp.speed = 0.707f;
	rotComp.rotateEulerAngles = Vector3(0, 1, 0);
	trans.transform.setIdentity();
	directionalLight = ecs.makeEntity(trans, rotComp);

	comp.mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets/Meshes/statue.obj");
	comp.m_Texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/wood.jpg");
	trans.transform.setIdentity();
	trans.transform.setTranslation(Vector3(1.0f, -0.1f, -1.5f));
	rotComp.speed = -2.1f;
	statue = ecs.makeEntity(trans, comp, rotComp);

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

	trans.transform.setIdentity();
	trans.transform.setTranslation(Vector3(0, 0, -1));
	flyCamComp.speed = 2.0f;
	camera = ecs.makeEntity(trans, flyCamComp);

	inputSystems.addSystem(flyCamSystem);
	mainSystems.addSystem(simpleMotionSystem);
	mainSystems.addSystem(eulerRotatorSystem);
	renderingSystems.addSystem(meshRendererSystem);

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
	flyCamSystem.clientWindowRect = rc;
}

void AppWindow::onUpdate()
{
	// input
	InputSystem::get()->update();
	ecs.updateSystems(inputSystems, m_DeltaTime);

	// update
	ecs.updateSystems(mainSystems, m_DeltaTime);

	// render
	DeviceContext* context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();
	context->clearRenderTargetColor(m_SwapChain, 0.1f, 0.1f, 0.6f, 1);
	RECT rc = this->getClientWindowRect();
	context->setViewportSize((FLOAT)(rc.right - rc.left), (FLOAT)(rc.bottom - rc.top));
	update();
	ecs.updateSystems(renderingSystems, m_DeltaTime);
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
	InputSystem::get()->addListener(&flyCamSystem);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(&flyCamSystem);
}
#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;

	virtual void onUpdate() override;

	virtual void onDestroy() override;


private:
	SwapChain* m_SwapChain;
	VertexBuffer* m_VertexBuffer;
	VertexShader* m_VertexShader;
	PixelShader* m_PixelShader;
};


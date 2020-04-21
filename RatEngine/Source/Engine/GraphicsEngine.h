#pragma once
#include <d3d11.h>
class SwapChain;
class DeviceContext;

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	// Initialize the Grpahics Engine and DirectX 11 Device
	bool init();

	// Release the resources loaded
	bool release();

	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext() { return m_ImmediateDeviceContext; }

public:
	static GraphicsEngine* get();

private:
	ID3D11Device* m_D3DDevice;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	DeviceContext* m_ImmediateDeviceContext;

	IDXGIDevice* m_DXGIDevice;
	IDXGIAdapter* m_DXGIAdapter;
	IDXGIFactory* m_DXGIFactory;

	friend class SwapChain;
};


#pragma once
#include <d3d11.h>
class SwapChain;
class DeviceContext;
class VertexBuffer;

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
	DeviceContext* getImmediateDeviceContext() { return m_DeviceContext; }
	VertexBuffer* createVertexBuffer();

	bool createShaders();
	bool setShaders();

	void getShaderBufferAndSize(void** bytecode, UINT* size);

public:
	static GraphicsEngine* get();

private:
	ID3D11Device* m_D3DDevice;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	DeviceContext* m_DeviceContext;
	ID3D11DeviceContext* m_ImmediateDeviceContext;

	IDXGIDevice* m_DXGIDevice;
	IDXGIAdapter* m_DXGIAdapter;
	IDXGIFactory* m_DXGIFactory;

	ID3DBlob* m_VSBlob;
	ID3DBlob* m_PSBlob;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	friend class SwapChain;
	friend class VertexBuffer;
};


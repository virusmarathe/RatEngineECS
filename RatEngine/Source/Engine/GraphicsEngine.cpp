#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"

#pragma warning(disable: 26812)

GraphicsEngine::GraphicsEngine() : m_ImmediateDeviceContext(NULL), m_D3DDevice(NULL), m_FeatureLevel(D3D_FEATURE_LEVEL_11_0),
									m_DXGIAdapter(NULL), m_DXGIDevice(NULL), m_DXGIFactory(NULL)
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT numLevels = ARRAYSIZE(featureLevels);
	ID3D11DeviceContext* immContext;

	HRESULT result = 0;
	for (UINT i = 0; i < numDriverTypes; ++i)
	{
		result = D3D11CreateDevice(NULL, driverTypes[i], NULL, NULL, featureLevels, numLevels,
										   D3D11_SDK_VERSION, &m_D3DDevice, &m_FeatureLevel, &immContext);
		if (SUCCEEDED(result)) break;
	}

	if (FAILED(result))
	{
		return false;
	}

	m_ImmediateDeviceContext = new DeviceContext(immContext);

	m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_DXGIDevice);
	m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_DXGIAdapter);
	m_DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory);

	return true;
}

bool GraphicsEngine::release()
{
	m_DXGIDevice->Release();
	m_DXGIAdapter->Release();
	m_DXGIFactory->Release();

	m_ImmediateDeviceContext->release();
	m_D3DDevice->Release();

	return true;
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}

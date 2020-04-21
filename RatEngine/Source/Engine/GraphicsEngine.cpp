#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include <d3dcompiler.h>

#pragma warning(disable: 26812)

GraphicsEngine::GraphicsEngine() : m_DeviceContext(NULL), m_D3DDevice(NULL), m_FeatureLevel(D3D_FEATURE_LEVEL_11_0),
									m_DXGIAdapter(NULL), m_DXGIDevice(NULL), m_DXGIFactory(NULL), m_VSBlob(NULL), m_PSBlob(NULL),
									m_VertexShader(NULL), m_PixelShader(NULL), m_ImmediateDeviceContext(NULL)
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

	HRESULT result = 0;
	for (UINT i = 0; i < numDriverTypes; ++i)
	{
		result = D3D11CreateDevice(NULL, driverTypes[i], NULL, NULL, featureLevels, numLevels,
										   D3D11_SDK_VERSION, &m_D3DDevice, &m_FeatureLevel, &m_ImmediateDeviceContext);
		if (SUCCEEDED(result)) break;
	}

	if (FAILED(result))	return false;

	m_DeviceContext = new DeviceContext(m_ImmediateDeviceContext);

	m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_DXGIDevice);
	m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_DXGIAdapter);
	m_DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory);

	return true;
}

bool GraphicsEngine::release()
{
	if (m_VertexShader) m_VertexShader->Release();
	if (m_PixelShader) m_PixelShader->Release();

	if (m_VSBlob) m_VSBlob->Release();
	if (m_PSBlob) m_PSBlob->Release();

	m_DXGIDevice->Release();
	m_DXGIAdapter->Release();
	m_DXGIFactory->Release();

	m_DeviceContext->release();
	m_D3DDevice->Release();

	return true;
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

bool GraphicsEngine::createShaders()
{
	ID3DBlob* errBlob = nullptr;
	D3DCompileFromFile(L"Source/Shaders/shader.fx", nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &m_VSBlob, &errBlob);
	D3DCompileFromFile(L"Source/Shaders/shader.fx", nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_PSBlob, &errBlob);
	m_D3DDevice->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr, &m_VertexShader);
	m_D3DDevice->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr, &m_PixelShader);
	return true;
}

bool GraphicsEngine::setShaders()
{
	m_ImmediateDeviceContext->VSSetShader(m_VertexShader, nullptr, 0);
	m_ImmediateDeviceContext->PSSetShader(m_PixelShader, nullptr, 0);
	return false;
}

void GraphicsEngine::getShaderBufferAndSize(void** bytecode, UINT* size)
{
	*bytecode = this->m_VSBlob->GetBufferPointer();
	*size = (UINT)this->m_VSBlob->GetBufferSize();
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}

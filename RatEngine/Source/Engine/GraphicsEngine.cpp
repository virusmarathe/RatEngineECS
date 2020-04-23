#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include <d3dcompiler.h>

#pragma warning(disable: 26812)

GraphicsEngine::GraphicsEngine() : m_DeviceContext(NULL), m_D3DDevice(NULL), m_FeatureLevel(D3D_FEATURE_LEVEL_11_0),
									m_DXGIAdapter(NULL), m_DXGIDevice(NULL), m_DXGIFactory(NULL),
									m_ImmediateDeviceContext(NULL), m_VertexShaderBlob(NULL), m_PixelShaderBlob(NULL)
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

ConstantBuffer* GraphicsEngine::createConstantBuffer()
{
	return new ConstantBuffer();
}

VertexShader* GraphicsEngine::createVertexShader(const void* shaderByteCode, SIZE_T byteCodeSize)
{
	VertexShader* shader = new VertexShader();
	if (!shader->init(shaderByteCode, byteCodeSize))
	{
		shader->release();
		return nullptr;
	}

	return shader;
}

bool GraphicsEngine::compileVertexShader(LPCWSTR fileName, LPCSTR entryPointName, void ** shaderByteCode, SIZE_T* byteCodeSize)
{
	ID3DBlob* errBlob = nullptr;
	HRESULT result = D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "vs_5_0", 0, 0, &m_VertexShaderBlob, &errBlob);

	if (FAILED(result))
	{
		errBlob->Release();
		return false;
	}

	*shaderByteCode = m_VertexShaderBlob->GetBufferPointer();
	*byteCodeSize = m_VertexShaderBlob->GetBufferSize();

	return true;
}

PixelShader* GraphicsEngine::createPixelShader(const void* shaderByteCode, SIZE_T byteCodeSize)
{
	PixelShader* shader = new PixelShader();
	if (!shader->init(shaderByteCode, byteCodeSize))
	{
		shader->release();
		return nullptr;
	}
	return shader;
}

bool GraphicsEngine::compilePixelShader(LPCWSTR fileName, LPCSTR entryPointName, void** shaderByteCode, SIZE_T* byteCodeSize)
{
	ID3DBlob* errBlob = nullptr;
	HRESULT result = D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "ps_5_0", 0, 0, &m_PixelShaderBlob, &errBlob);

	if (FAILED(result))
	{
		errBlob->Release();
		return false;
	}

	*shaderByteCode = m_PixelShaderBlob->GetBufferPointer();
	*byteCodeSize = m_PixelShaderBlob->GetBufferSize();

	return true;
}

void GraphicsEngine::releaseCompiledShader()
{
	if (m_VertexShaderBlob)
	{
		m_VertexShaderBlob->Release();
	}

	if (m_PixelShaderBlob)
	{
		m_PixelShaderBlob->Release();
	}
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}

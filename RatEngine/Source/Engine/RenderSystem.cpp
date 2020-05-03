#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include <d3dcompiler.h>
#include <exception>

#pragma warning(disable: 26812)

RenderSystem::RenderSystem() : m_DeviceContext(NULL), m_D3DDevice(NULL), m_FeatureLevel(D3D_FEATURE_LEVEL_11_0),
m_DXGIAdapter(NULL), m_DXGIDevice(NULL), m_DXGIFactory(NULL),
m_ImmediateDeviceContext(NULL), m_VertexShaderBlob(NULL), m_PixelShaderBlob(NULL)
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

	if (FAILED(result))	throw std::exception("RenderSystem::D3D11CreateDevice not found");

	m_DeviceContext = new DeviceContext(m_ImmediateDeviceContext, this);

	m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_DXGIDevice);
	m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_DXGIAdapter);
	m_DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory);
}

RenderSystem::~RenderSystem()
{
	m_DXGIDevice->Release();
	m_DXGIAdapter->Release();
	m_DXGIFactory->Release();

	delete m_DeviceContext;

	m_D3DDevice->Release();
}

SwapChain* RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChain* sc = NULL;
	try
	{
		sc = new SwapChain(hwnd, width, height, this);
	}
	catch (...) {}
	
	return sc;
}

VertexBuffer* RenderSystem::createVertexBuffer(void* listVertices, UINT sizeVertex, UINT numVertices, void* shaderByteCode,
											   SIZE_T shaderByteSize, D3D11_INPUT_ELEMENT_DESC layout[], UINT layoutSize)
{
	VertexBuffer* vb = NULL;
	try
	{
		vb = new VertexBuffer(listVertices, sizeVertex, numVertices, shaderByteCode, shaderByteSize, layout, layoutSize, this);
	}
	catch (...) {}

	return vb;
}

IndexBuffer* RenderSystem::createIndexBuffer(void* listIndices, UINT numIndices)
{
	IndexBuffer* ib = NULL;
	try
	{
		ib = new IndexBuffer(listIndices, numIndices, this);
	}
	catch (...) {}

	return ib;
}

ConstantBuffer* RenderSystem::createConstantBuffer(void* buffer, UINT bufferSize)
{
	ConstantBuffer* cb = NULL;
	try
	{
		cb = new ConstantBuffer(buffer, bufferSize, this);
	}
	catch (...) {}

	return cb;
}

VertexShader* RenderSystem::createVertexShader(const void* shaderByteCode, SIZE_T byteCodeSize)
{
	VertexShader* vs = NULL;
	try
	{
		vs = new VertexShader(shaderByteCode, byteCodeSize, this);
	}
	catch (...) {}

	return vs;
}

bool RenderSystem::compileVertexShader(LPCWSTR fileName, LPCSTR entryPointName, void** shaderByteCode, SIZE_T* byteCodeSize)
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

PixelShader* RenderSystem::createPixelShader(const void* shaderByteCode, SIZE_T byteCodeSize)
{
	PixelShader* ps = NULL;
	try
	{
		ps = new PixelShader(shaderByteCode, byteCodeSize, this);
	}
	catch (...) {}

	return ps;
}

bool RenderSystem::compilePixelShader(LPCWSTR fileName, LPCSTR entryPointName, void** shaderByteCode, SIZE_T* byteCodeSize)
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

void RenderSystem::releaseCompiledShader()
{
	if (m_VertexShaderBlob)
	{
		m_VertexShaderBlob->Release();
		m_VertexShaderBlob = nullptr;
	}

	if (m_PixelShaderBlob)
	{
		m_PixelShaderBlob->Release();
		m_PixelShaderBlob = nullptr;
	}
}
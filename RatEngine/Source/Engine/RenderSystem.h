#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	SwapChain* createSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContext* getImmediateDeviceContext() { return m_DeviceContext; }
	VertexBuffer* createVertexBuffer(void* listVertices, UINT sizeVertex, UINT numVertices, void* shaderByteCode, SIZE_T shaderByteSize);
	IndexBuffer* createIndexBuffer(void* listIndices, UINT numIndices);
	ConstantBuffer* createConstantBuffer(void* buffer, UINT bufferSize);

	VertexShader* createVertexShader(const void* shaderByteCode, SIZE_T byteCodeSize);
	PixelShader* createPixelShader(const void* shaderByteCode, SIZE_T byteCodeSize);

	bool compileVertexShader(LPCWSTR fileName, LPCSTR entryPointName, void** shaderByteCode, SIZE_T* byteCodeSize);
	bool compilePixelShader(LPCWSTR fileName, LPCSTR entryPointName, void** shaderByteCode, SIZE_T* byteCodeSize);

	void releaseCompiledShader();

private:
	ID3D11Device* m_D3DDevice;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	DeviceContext* m_DeviceContext;
	ID3D11DeviceContext* m_ImmediateDeviceContext;

	IDXGIDevice* m_DXGIDevice;
	IDXGIAdapter* m_DXGIAdapter;
	IDXGIFactory* m_DXGIFactory;

	ID3DBlob* m_VertexShaderBlob;
	ID3DBlob* m_PixelShaderBlob;

	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class ConstantBuffer;
	friend class IndexBuffer;
	friend class Texture;
};


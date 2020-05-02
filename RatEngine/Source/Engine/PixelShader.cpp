#include "PixelShader.h"
#include "RenderSystem.h"
#include <exception>

PixelShader::PixelShader(const void* shaderByteCode, SIZE_T byteCodeSize, RenderSystem* system) : m_PixelShader(NULL), m_RenderSystem(system)
{
	HRESULT result = m_RenderSystem->m_D3DDevice->CreatePixelShader(shaderByteCode, byteCodeSize, NULL, &m_PixelShader);

	if (FAILED(result)) throw std::exception("PixelShader not created successfully");
}

PixelShader::~PixelShader()
{
	if (m_PixelShader) m_PixelShader->Release();
}
#include "VertexShader.h"
#include "RenderSystem.h"
#include <exception>

VertexShader::VertexShader(const void* shaderByteCode, SIZE_T byteCodeSize, RenderSystem* system) :
	m_Shader(NULL), m_RenderSystem(system)
{
	HRESULT result = m_RenderSystem->m_D3DDevice->CreateVertexShader(shaderByteCode, byteCodeSize, NULL, &m_Shader);

	if (FAILED(result))	throw std::exception("VertexShader not created successfully");
}

VertexShader::~VertexShader()
{
	if (m_Shader) m_Shader->Release();
}

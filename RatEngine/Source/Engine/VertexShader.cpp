#include "VertexShader.h"
#include "GraphicsEngine.h"

VertexShader::VertexShader() : m_Shader(NULL)
{
}

VertexShader::~VertexShader()
{
}

bool VertexShader::init(const void* shaderByteCode, SIZE_T byteCodeSize)
{
	HRESULT result = GraphicsEngine::get()->m_D3DDevice->CreateVertexShader(shaderByteCode, byteCodeSize, NULL, &m_Shader);

	if (FAILED(result)) return false;

	return true;
}

void VertexShader::release()
{
	m_Shader->Release();
	delete this;
}

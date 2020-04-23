#include "PixelShader.h"
#include "GraphicsEngine.h"

PixelShader::PixelShader() : m_PixelShader(NULL)
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::release()
{
	if (m_PixelShader)
	{
		m_PixelShader->Release();
	}
	delete this;
}

bool PixelShader::init(const void* shaderByteCode, SIZE_T byteCodeSize)
{
	HRESULT result = GraphicsEngine::get()->m_D3DDevice->CreatePixelShader(shaderByteCode, byteCodeSize, NULL, &m_PixelShader);
	if (FAILED(result)) return false;

	return true;
}

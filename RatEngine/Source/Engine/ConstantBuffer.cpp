#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"

ConstantBuffer::ConstantBuffer() : m_Buffer(NULL)
{
}

ConstantBuffer::~ConstantBuffer()
{
}

bool ConstantBuffer::load(void* buffer, UINT bufferSize)
{
	if (m_Buffer) m_Buffer->Release();

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = bufferSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = buffer;
	
	HRESULT result = GraphicsEngine::get()->m_D3DDevice->CreateBuffer(&bufferDesc, &initData, &m_Buffer);

	if (FAILED(result)) return false;

	return true;
}

void ConstantBuffer::update(DeviceContext* context, void* buffer)
{
	context->m_DeviceContext->UpdateSubresource(this->m_Buffer, NULL, NULL, buffer, NULL, NULL);
}

void ConstantBuffer::release()
{
	if (m_Buffer)
	{
		m_Buffer->Release();
	}
	delete this;
}

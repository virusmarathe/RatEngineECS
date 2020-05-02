#include "ConstantBuffer.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include <exception>

ConstantBuffer::ConstantBuffer(void* buffer, UINT bufferSize, RenderSystem* system) : m_Buffer(NULL), m_RenderSystem(system)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = bufferSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = buffer;

	HRESULT result = m_RenderSystem->m_D3DDevice->CreateBuffer(&bufferDesc, &initData, &m_Buffer);

	if (FAILED(result)) throw std::exception("ConstantBuffer not created successfully");
}

ConstantBuffer::~ConstantBuffer()
{
	if (m_Buffer)
	{
		m_Buffer->Release();
	}
}

void ConstantBuffer::update(DeviceContext* context, void* buffer)
{
	context->m_DeviceContext->UpdateSubresource(this->m_Buffer, NULL, NULL, buffer, NULL, NULL);
}

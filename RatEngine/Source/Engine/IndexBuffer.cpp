#include "IndexBuffer.h"
#include "RenderSystem.h"
#include <exception>

IndexBuffer::IndexBuffer(void* listIndices, UINT numIndices, RenderSystem* system) : m_IndexBuffer(0), m_NumIndices(0), m_RenderSystem(system)
{
	D3D11_BUFFER_DESC buffDesc;
	buffDesc.Usage = D3D11_USAGE_DEFAULT; // can be written or read by gpu
	buffDesc.ByteWidth = 4 * numIndices;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = listIndices;

	m_NumIndices = numIndices;

	HRESULT result = m_RenderSystem->m_D3DDevice->CreateBuffer(&buffDesc, &initData, &m_IndexBuffer);

	if (FAILED(result)) throw std::exception("IndexBuffer not created successfully");
}

IndexBuffer::~IndexBuffer()
{
	if (m_IndexBuffer) m_IndexBuffer->Release();
}
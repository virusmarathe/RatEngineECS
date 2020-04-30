#include "IndexBuffer.h"
#include "GraphicsEngine.h"

IndexBuffer::IndexBuffer() : m_IndexBuffer(0), m_NumIndices(0)
{
}

IndexBuffer::~IndexBuffer()
{
}

bool IndexBuffer::load(void* listIndices, UINT numIndices)
{
	if (m_IndexBuffer) m_IndexBuffer->Release();

	D3D11_BUFFER_DESC buffDesc;
	buffDesc.Usage = D3D11_USAGE_DEFAULT; // can be written or read by gpu
	buffDesc.ByteWidth = 4 * numIndices;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = listIndices;

	m_NumIndices = numIndices;

	HRESULT result = GraphicsEngine::get()->m_D3DDevice->CreateBuffer(&buffDesc, &initData, &m_IndexBuffer);

	if (FAILED(result)) return false;

	return true;
}

bool IndexBuffer::release()
{
	m_IndexBuffer->Release();

	delete this;
	return true;
}

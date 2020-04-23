#include "VertexBuffer.h"
#include "GraphicsEngine.h"

VertexBuffer::VertexBuffer() : m_VertexSize(0), m_NumVertices(0), m_VertexBuffer(NULL), m_InputLayout(NULL)
{
}

VertexBuffer::~VertexBuffer()
{
}

bool VertexBuffer::load(void* listVertices, UINT sizeVertex, UINT numVertices, void * shaderByteCode, UINT shaderByteSize)
{
	if (m_VertexBuffer) m_VertexBuffer->Release();
	if (m_InputLayout) m_InputLayout->Release();

	D3D11_BUFFER_DESC buffDesc;
	buffDesc.Usage = D3D11_USAGE_DEFAULT; // can be written or read by gpu
	buffDesc.ByteWidth = sizeVertex * numVertices;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = listVertices;

	m_VertexSize = sizeVertex;
	m_NumVertices = numVertices;

	HRESULT result = GraphicsEngine::get()->m_D3DDevice->CreateBuffer(&buffDesc, &initData, &m_VertexBuffer);

	if (FAILED(result)) return false;

	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		// SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT sizeLayout = ARRAYSIZE(layout);

	result = GraphicsEngine::get()->m_D3DDevice->CreateInputLayout(layout, sizeLayout, shaderByteCode, shaderByteSize, &m_InputLayout);

	if (FAILED(result)) return false;

	return true;
}

bool VertexBuffer::release()
{
	m_InputLayout->Release();
	m_VertexBuffer->Release();

	delete this;

	return true;
}

#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class VertexBuffer
{
public:
	VertexBuffer(void* listVertices, UINT sizeVertex, UINT numVertices, void* shaderByteCode, SIZE_T shaderByteSize,
				 D3D11_INPUT_ELEMENT_DESC layout[], UINT layoutSize, RenderSystem* system);
	~VertexBuffer();

	UINT getNumVertices() { return m_NumVertices; }

private:
	UINT m_VertexSize;
	UINT m_NumVertices;

	ID3D11Buffer* m_VertexBuffer;
	ID3D11InputLayout* m_InputLayout;
	RenderSystem* m_RenderSystem;

	friend class DeviceContext;
};


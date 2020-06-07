#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class VertexBuffer
{
public:
	VertexBuffer(void* listVertices, UINT sizeVertex, UINT numVertices, RenderSystem* system);
	~VertexBuffer();

	UINT getNumVertices() { return m_NumVertices; }

private:
	UINT m_VertexSize;
	UINT m_NumVertices;

	ID3D11Buffer* m_VertexBuffer;
	RenderSystem* m_RenderSystem;

	friend class DeviceContext;
};


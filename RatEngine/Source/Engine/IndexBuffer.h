#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class IndexBuffer
{
public:
	IndexBuffer(void* listIndices, UINT numIndices, RenderSystem* system);
	~IndexBuffer();

	UINT getNumIndices() { return m_NumIndices; }

private:
	UINT m_NumIndices;

	ID3D11Buffer* m_IndexBuffer;
	RenderSystem* m_RenderSystem;

	friend class DeviceContext;
};


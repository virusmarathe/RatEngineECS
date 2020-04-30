#pragma once
#include <d3d11.h>

class DeviceContext;

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	bool load(void* listIndices, UINT numIndices);
	bool release();

	UINT getNumIndices() { return m_NumIndices; }

private:
	UINT m_NumIndices;

	ID3D11Buffer* m_IndexBuffer;

	friend class DeviceContext;
};


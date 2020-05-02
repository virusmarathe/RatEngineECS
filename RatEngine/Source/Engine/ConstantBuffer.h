#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT bufferSize, RenderSystem * system);
	~ConstantBuffer();

	void update(DeviceContext* context, void* buffer);

private:

	ID3D11Buffer* m_Buffer;
	RenderSystem* m_RenderSystem;

	friend class DeviceContext;
};


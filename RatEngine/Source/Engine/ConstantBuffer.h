#pragma once
#include <d3d11.h>

class DeviceContext;

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	bool load(void* buffer, UINT bufferSize);
	void update(DeviceContext* context, void* buffer);
	void release();

private:

	ID3D11Buffer* m_Buffer;

	friend class DeviceContext;
};


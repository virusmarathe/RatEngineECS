#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vector3.h"
#include "Vector2.h"

struct MeshPos0Tex0
{
	MeshPos0Tex0() : m_Position(), m_Texture() { }
	MeshPos0Tex0(Vector3 position, Vector2 tex) : m_Position(position), m_Texture(tex) { }
	MeshPos0Tex0(const MeshPos0Tex0& meshData) : m_Position(meshData.m_Position), m_Texture(meshData.m_Texture) { }

	Vector3 m_Position;
	Vector2 m_Texture;
};

class Mesh : public Resource
{
public:
	Mesh(const wchar_t* fullPath);
	~Mesh();

	VertexBuffer* getVertexBuffer() { return m_VertexBuffer; }
	IndexBuffer* getIndexBuffer() { return m_IndexBuffer; }
	VertexShader* getVertexShader() { return m_VertexShader; }

private:
	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;
	VertexShader* m_VertexShader;
};


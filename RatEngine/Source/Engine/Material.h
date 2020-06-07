#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "Resource.h"
#include "VertexShader.h"
#include "PixelShader.h"

class Material : public Resource
{
public:
    Material(const wchar_t* fullPath);
    ~Material();

    ID3D11InputLayout* m_InputLayout;
    VertexShader* m_VertexShader;
    PixelShader* m_PixelShader;
};


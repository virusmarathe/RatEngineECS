#include "Material.h"
#include "GraphicsEngine.h"
#include "Debug.h"
#include <iostream>
#include <fstream>
#include "StringUtils.h"

Material::Material(const wchar_t* fullPath) : Resource(fullPath)
{
	std::ifstream file(fullPath);
	std::string line;
	std::wstring vertexShaderFullPath;
	std::string vertexShaderFunctionName;
	std::wstring pixelShaderFullPath;
	std::string pixelShaderFunctionName;

	if (file.is_open())
	{
		std::getline(file, line, file.widen(','));
		vertexShaderFullPath = L"Assets/Shaders/" + s2ws(line);
		std::getline(file, line, file.widen('\n'));
		vertexShaderFunctionName = line;
		std::getline(file, line, file.widen(','));
		pixelShaderFullPath = L"Assets/Shaders/" + s2ws(line);
		std::getline(file, line, file.widen('\n'));
		pixelShaderFunctionName = line;
		file.close();
	}

	void* shaderByteCode = nullptr;
	SIZE_T shaderSize = 0;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT sizeLayout = ARRAYSIZE(layout);

	if (!GraphicsEngine::get()->getRenderSystem()->compileVertexShader(vertexShaderFullPath.c_str(), vertexShaderFunctionName.c_str(), &shaderByteCode, &shaderSize))
	{
		DEBUG_LOG("Shader", LOG_ERROR, "Compile of shader %ls failed", vertexShaderFullPath.c_str()); // TODO: add shader compile error logging info
	}
	m_InputLayout = GraphicsEngine::get()->getRenderSystem()->createInputLayout(shaderByteCode, shaderSize, layout, sizeLayout);
	m_VertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shaderByteCode, shaderSize);

	if (!GraphicsEngine::get()->getRenderSystem()->compilePixelShader(pixelShaderFullPath.c_str(), pixelShaderFunctionName.c_str(), &shaderByteCode, &shaderSize))
	{
		DEBUG_LOG("Shader", LOG_ERROR, "Compile of shader %ls failed", pixelShaderFullPath.c_str()); // TODO: add shader compile error logging info
	}
	m_PixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();


}

Material::~Material()
{
	m_InputLayout->Release();
	delete m_VertexShader;
	delete m_PixelShader;
}
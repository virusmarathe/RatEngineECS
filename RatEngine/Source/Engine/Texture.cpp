#include "Texture.h"
#include <DirectXTex.h>
#include <exception>
#include "GraphicsEngine.h"

Texture::Texture(const wchar_t* fullPath) : Resource(fullPath)
{
	DirectX::ScratchImage imageData;
	HRESULT result = DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_NONE, nullptr, imageData);

	if (FAILED(result)) throw std::exception("Texture::LoadFromWICFile not loaded successfully");

	result = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->m_D3DDevice, imageData.GetImages(),
									imageData.GetImageCount(), imageData.GetMetadata(), &m_Texture);

	if (FAILED(result)) throw std::exception("Texture not created successfully");

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = imageData.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = imageData.GetMetadata().mipLevels;
	desc.Texture2D.MostDetailedMip = 0;

	result = GraphicsEngine::get()->getRenderSystem()->m_D3DDevice->CreateShaderResourceView(m_Texture, &desc, &m_ShaderResourceView);

	if (FAILED(result)) throw std::exception("Texture::CreateShaderResourceView not created successfully");
}

Texture::~Texture()
{
	m_ShaderResourceView->Release();
	m_Texture->Release();
}

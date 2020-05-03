#pragma once
#include <memory>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class IndexBuffer;
class RenderSystem;
class GraphicsEngine;
class ResourceManager;
class Resource;
class TextureManager;
class Texture;

typedef std::shared_ptr<Resource> ResourcePointer;
typedef std::shared_ptr<Texture> TexturePointer;
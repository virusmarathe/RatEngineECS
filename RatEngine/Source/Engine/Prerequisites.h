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
class MeshManager;
class Mesh;
class MaterialManager;
class Material;

typedef std::shared_ptr<Resource> ResourcePointer;
typedef std::shared_ptr<Texture> TexturePointer;
typedef std::shared_ptr<Mesh> MeshPointer;
typedef std::shared_ptr<Material> MaterialPointer;
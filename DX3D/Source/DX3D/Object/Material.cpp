#include "DX3D/Object/Material.h"
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/TextureManager.h>	
dx3d::Material::Material(const GraphicsResourceDesc& desc) : GraphicsResource(desc)
{
}
void dx3d::Material::SetPixelShader(const wchar_t* pixel_shader_path)
{
	m_renderSystem->compilePixelShader(pixel_shader_path, pixelBlob);
	m_renderSystem->createPixelShader(pixelBlob, pixelShader);
}

void dx3d::Material::SetVertexShader(const wchar_t* vertex_shader_path)
{
	m_renderSystem->compileVertexShader(vertex_shader_path, vertexBlob);
	m_renderSystem->createVertexShader(vertexBlob, vertexShader);
}
void dx3d::Material::SetTexture(const wchar_t* file_path)
{
	texture = GraphicsEngine::get()->getTextureManager().createTextureFromFile(file_path);
}
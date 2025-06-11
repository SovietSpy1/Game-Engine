#include "DX3D/Object/GameObject.h"
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/ResourceManager/MeshManager/MeshManager.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/TextureManager.h>
#include <DX3D/Graphics/VertexBuffer.h>
dx3d::GameObject::GameObject(const BaseDesc& desc) :Base(desc)
{
	transform.SetIdentity();
}
void dx3d::GameObject::AddMesh(const wchar_t* file_path)
{
	mesh = GraphicsEngine::get()->getMeshManager().createMeshFromFile(file_path);
}
void dx3d::GameObject::AddMesh()
{
	mesh = GraphicsEngine::get()->getMeshManager().createMesh();
}
void dx3d::GameObject::SetTexture(const wchar_t* file_path)
{
	texture = GraphicsEngine::get()->getTextureManager().createTextureFromFile(file_path);
}
void dx3d::GameObject::SetPosition(float x, float y, float z)
{
	transform.SetTranslate(Vector3D(x, y, z));
}

void dx3d::GameObject::SetRotation(float x, float y, float z)
{
	transform.SetRotationX(x);
	transform.SetRotationY(y);
	transform.SetRotationZ(z);
}

void dx3d::GameObject::SetAxisBuffer()
{
	float mag = 2.0f;
	RenderSystem  &renderSystem = GraphicsEngine::get()->getRenderSystem();
	const WCHAR* vertexShaderPath = L"DX3D/Shaders/Axis/VertexShader.hlsl";
	const WCHAR* pixelShaderPath = L"DX3D/Shaders/Axis/PixelShader.hlsl";
	renderSystem.compilePixelShader(pixelShaderPath, pixelBlob);
	renderSystem.compileVertexShader(vertexShaderPath, vertexBlob);
	renderSystem.createVertexShader(vertexBlob, vertexShader);
	renderSystem.createPixelShader(pixelBlob, pixelShader);
	std::vector<vertexWithColor> vertices(6);
	vertices[0] = { Vector3D(0, 0, 0), vec4(1, 0, 0, 1) }; // X axis origin
	vertices[1] = { Vector3D(1,0,0) * mag, vec4(1, 0, 0, 1)}; // X axis end
	vertices[2] = { Vector3D(0, 0, 0), vec4(0, 1, 0, 1) }; // Y axis origin
	vertices[3] = { Vector3D(0,1,0) * mag, vec4(0, 1, 0, 1) }; // Y axis end
	vertices[4] = { Vector3D(0, 0, 0), vec4(0, 0, 1, 1) }; // Z axis origin
	vertices[5] = { Vector3D(0,0,1) * mag, vec4(0, 0, 1, 1) }; // Z axis end
	axisVertexBuffer = GraphicsEngine::get()->getRenderSystem().createVertexBuffer();
	axisVertexBuffer->loadWithColor(vertices.data(), sizeof(vertexWithColor), vertices.size(), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize());
}

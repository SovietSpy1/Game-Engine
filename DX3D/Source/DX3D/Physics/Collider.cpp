#include "DX3D/Physics/Collider.h"
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
dx3d::Collider::Collider(const BaseDesc& desc, GameObject* owner, ColliderType colliderType) : Base(desc), owner(owner), colliderType(colliderType)
{
	transform.SetIdentity();
	scale.SetIdentity();
	if (vertexShader == nullptr) {
		const WCHAR* vertexShaderPath = L"DX3D/Shaders/Collider/VertexShader.hlsl";
		Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob;  
		GraphicsEngine::get()->getRenderSystem()->compileVertexShader(vertexShaderPath, vertexBlob);
		GraphicsEngine::get()->getRenderSystem()->createVertexShader(vertexBlob, vertexShader);
	}
	if (pixelShader == nullptr) {
		const WCHAR* pixelShaderPath = L"DX3D/Shaders/Collider/PixelShader.hlsl";
		Microsoft::WRL::ComPtr<ID3DBlob> pixelBlob;
		GraphicsEngine::get()->getRenderSystem()->compilePixelShader(pixelShaderPath, pixelBlob);
		GraphicsEngine::get()->getRenderSystem()->createPixelShader(pixelBlob, pixelShader);
	}
	vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer();
	indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer();
	constantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer();
	switch (colliderType)
	{
	case ColliderType::Box:
		LoadCubeCollider();
		break;
	case ColliderType::Quad:
		LoadQuadCollider();
		break;
	default:
		DX3DLogError("Collider type not implemented.");
		break;
	}
	vertices.resize(baseVertices.size());
	normals.resize(baseNormals.size());
	edges.resize(baseEdges.size());
	std::vector<vec4_32> colors(baseVertices.size(), vec4_32(0, 255, 0, 255));
	std::vector<vertex> renderedVerts = VertexBuffer::FillInVertexData({ {IA::POSITION, baseVertices}, {IA::COLOR, colors} });
	vertexBuffer->load(renderedVerts.data(), sizeof(vertex), renderedVerts.size());
	indexBuffer->load(indices.data(), indices.size());
	colliderBuffer = { transform, scale };
	constantBuffer->load(colliderBuffer);
}

void dx3d::Collider::LoadCubeCollider()
{
	baseVertices = {
		Vector3D(-0.5f, -0.5f, -0.5f),
		Vector3D(-0.5f, 0.5f, -0.5f),
		Vector3D(0.5f, 0.5f, -0.5f),
		Vector3D(0.5f, -0.5f, -0.5f),
		Vector3D(0.5f, -0.5f, 0.5f),
		Vector3D(0.5f, 0.5f, 0.5f),
		Vector3D(-0.5f, 0.5f, 0.5f),
		Vector3D(-0.5f, -0.5f, 0.5f)
	};
	baseNormals = {
		//back face
		Vector3D(0.0f, 0.0f, 1.0f),
		// top face
		Vector3D(0.0f, 1.0f, 0.0f),
		// right face
		Vector3D(1.0f, 0.0f, 0.0f),
	};
	baseEdges = {
		Vector3D(0.0f, 0.0f, 1.0f),
		// top face
		Vector3D(0.0f, 1.0f, 0.0f),
		// right face
		Vector3D(1.0f, 0.0f, 0.0f),
	};
	indices =
	{
		//bottom
		0,3,
		3,4,
		4,7,
		7,0,
		//top
		1,2,
		2,5,
		5,6,
		6,1,
		//left
		0,1,
		7,6,
		//right
		3,2,
		4,5
	};
}

void dx3d::Collider::LoadSphereCollider()
{
}

void dx3d::Collider::LoadQuadCollider()
{
	baseVertices = {
		Vector3D(-0.5f, 0.0f, -0.5f),
		Vector3D(-0.5f, 0.0f, 0.5f),
		Vector3D(0.5f, 0.0f, -0.5f),
		Vector3D(0.5f, 0.0f, 0.5f)
	};
	baseNormals = {
		Vector3D(0.0f, 1.0f, 0.0f) // Normal pointing up
	};
	baseEdges = {
		Vector3D(1.0f, 0,0),
		Vector3D(0,0,1)
	};
	indices = {
		0, 1,
		1,3,
		3,2,
		2,0
	};
}
void dx3d::Collider::SetScale(const Vector3D& scale)
{
	this->scale.SetScale(scale);
	colliderBuffer = { transform, this->scale };
	constantBuffer->load(colliderBuffer);
}

void dx3d::Collider::SetTransform(const Vector3D& position)
{
	transform.SetTranslate(position);
	colliderBuffer = { transform, scale };
	constantBuffer->load(colliderBuffer);
}

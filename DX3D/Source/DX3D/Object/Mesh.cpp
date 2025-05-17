#include "DX3D/Object/Mesh.h"
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Math/GameMath.h>
dx3d::Mesh::Mesh(const GraphicsResourceDesc& gDesc) : GraphicsResource(gDesc)
{
}

void dx3d::Mesh::LoadHeartMesh()
{
	const WCHAR* vertexShaderPath = L"DX3D/Shaders/Heart/VertexShader.hlsl";
	const WCHAR* pixelShaderPath = L"DX3D/Shaders/Heart/PixelShader.hlsl";
	vertices.clear();
	for (int i = 0; i < 48; i++) {
		vertices.push_back(vertex{ Vector3D{0,0,0}, vec4{(0.1f + (float)std::rand()) / RAND_MAX * 0.8f, 0, 0,1}, vec2{0,0} });
	}
	vertices[0].position = { 0,-0.5f, 0 };
	vertices[1].position = vertices[0].position + Vector3D{ -0.3f,0.3f,0 };
	vertices[2].position = { 0,0,0 };
	vertices[3].position = vertices[1].position;
	vertices[4].position = { -0.25f, 0.25f, 0 };
	vertices[5].position = vertices[2].position;
	vertices[6].position = vertices[1].position;
	vertices[7].position = { -0.5f, 0.1f, 0 };
	vertices[8].position = vertices[4].position;
	vertices[9].position = vertices[7].position;
	vertices[10].position = vertices[9].position + Vector3D{ 0, 0.25f,0 };
	vertices[11].position = vertices[4].position;
	vertices[12].position = vertices[10].position;
	vertices[13].position = vertices[12].position + Vector3D{ 0.15f,0.15f,0 };
	vertices[14].position = vertices[4].position;
	vertices[15].position = vertices[13].position;
	vertices[16].position = vertices[15].position + Vector3D{ 0.2f,0,0 };
	vertices[17].position = vertices[4].position;
	vertices[18].position = vertices[4].position;
	vertices[19].position = vertices[16].position;
	vertices[20].position = Vector3D{ 0,0.35f,0 };
	vertices[21].position = vertices[2].position;
	vertices[22].position = vertices[4].position;
	vertices[23].position = vertices[20].position;
	for (int i = 0; i < 24; i++) {
		vertices[24 + i].position = Vector3D{ -vertices[i].position.x, vertices[i].position.y,0 };
	}
	m_renderSystem->compilePixelShader(pixelShaderPath, pixelBlob);
	m_renderSystem->compileVertexShader(vertexShaderPath, vertexBlob);
	m_renderSystem->createVertexShader(vertexBlob, vertexShader);
	m_renderSystem->createPixelShader(pixelBlob, pixelShader);
}

void dx3d::Mesh::LoadCubeMesh()
{
	const WCHAR* vertexShaderPath = L"DX3D/Shaders/Cube/VertexShader.hlsl";
	const WCHAR* pixelShaderPath = L"DX3D/Shaders/Cube/PixelShader.hlsl";
	m_renderSystem->compilePixelShader(pixelShaderPath, pixelBlob);
	m_renderSystem->compileVertexShader(vertexShaderPath, vertexBlob);
	m_renderSystem->createVertexShader(vertexBlob, vertexShader);
	m_renderSystem->createPixelShader(pixelBlob, pixelShader);
	vertices.clear();
	for (int i = 0; i < 8; i++) {
		vertices.push_back(vertex(Vector3D(0, 0, 0), RandColor(1).rgba, vec2(0, 0)));
	}
	vertices[0].position = Vector3D(-0.5f, -0.5f, -0.5f);
	vertices[1].position = Vector3D(-0.5f, 0.5f, -0.5f);
	vertices[2].position = Vector3D(0.5f, 0.5f, -0.5f);
	vertices[3].position = Vector3D(0.5f, -0.5f, -0.5f);
	vertices[4].position = Vector3D(0.5f, -0.5f, 0.5f);
	vertices[5].position = Vector3D(0.5f, 0.5f, 0.5f);
	vertices[6].position = Vector3D(-0.5f, 0.5f, 0.5f);
	vertices[7].position = Vector3D(-0.5f, -0.5f, 0.5f);
}

void dx3d::Mesh::LoadQuadMesh()
{
	const WCHAR* vertexShaderPath = L"DX3D/Shaders/Cube/VertexShader.hlsl";
	const WCHAR* pixelShaderPath = L"DX3D/Shaders/Cube/PixelShader.hlsl";
	m_renderSystem->compilePixelShader(pixelShaderPath, pixelBlob);
	m_renderSystem->compileVertexShader(vertexShaderPath, vertexBlob);
	m_renderSystem->createVertexShader(vertexBlob, vertexShader);
	m_renderSystem->createPixelShader(pixelBlob, pixelShader);
	vertices.clear();
	for (int i = 0; i < 6; i++) {
		vertices.push_back(vertex(Vector3D(0, 0, 0), RandColor(1).rgba, vec2(0, 0)));
	}
	vertices[0].position = Vector3D(-0.5f, -0.5f, 0);
	vertices[1].position = Vector3D(0.5f, -0.5f, 0);
	vertices[2].position = Vector3D(-0.5f, 0.5f, 0);
}

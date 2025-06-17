#pragma once
#include <DX3D/Core/Base.h>
#include <wrl.h>
namespace dx3d {
	struct ColliderBuffer {
		Matrix4X4 transform;
		Matrix4X4 scale;
	};
	class Collider : public Base
	{
	public:
		Collider(const BaseDesc& desc, GameObject* owner, ColliderType colliderType);
		void LoadCubeCollider();
		void LoadSphereCollider();
		void LoadQuadCollider();
		void SetTransform(const Vector3D& position);
		void SetScale(const Vector3D& scale);
		std::vector<Vector3D> vertices;
		std::vector<Vector3D> baseVertices;
		std::vector<Vector3D> normals;
		std::vector<Vector3D> baseNormals;
		std::vector<Vector3D> baseEdges;
		std::vector<Vector3D> edges;
		std::vector<UINT> indices;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		static inline Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader{};
		static inline Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader{};
		GameObject* owner;
		ColliderType colliderType{ ColliderType::Box }; // Default to Box collider
		Matrix4X4 transform;
		Matrix4X4 scale;
		ColliderBuffer colliderBuffer;
		std::shared_ptr<ConstantBuffer> constantBuffer{};
		bool show = false;
	};
}



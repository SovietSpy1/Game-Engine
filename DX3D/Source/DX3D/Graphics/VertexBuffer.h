#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <unordered_map>
#include <variant>
namespace dx3d {
	using vertexData = std::variant<std::vector<Vector2D>, std::vector<Vector3D>, std::vector<vec4_32>>;
	class VertexBuffer: public GraphicsResource
	{
	public:
		explicit VertexBuffer(const GraphicsResourceDesc& desc);
		std::vector<D3D11_INPUT_ELEMENT_DESC> LayoutFromSemantics(std::vector<IA> semantics);
		void load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code = nullptr, UINT size_byte_shader = 0, std::vector<IA> semantics = {IA::POSITION, IA::TEXCOORD, IA::NORMAL, IA::COLOR});
		static std::vector<vertex> FillInVertexData(std::unordered_map<IA, vertexData> data);
		UINT getSizeVertexList();
	private:
		UINT m_size_vertex{};
		UINT m_size_list{};
	private:
		static inline Microsoft::WRL::ComPtr<ID3DBlob> vertexLayoutBlob{};
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
		friend class DeviceContext;
	};
}



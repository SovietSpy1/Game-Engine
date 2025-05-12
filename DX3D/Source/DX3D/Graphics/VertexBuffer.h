#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
namespace dx3d {
	class VertexBuffer: public GraphicsResource
	{
	public:
		explicit VertexBuffer(const GraphicsResourceDesc& desc);
		void load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader);
		UINT getSizeVertexList();
	private:
		UINT m_size_vertex{};
		UINT m_size_list{};
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
		friend class DeviceContext;
	};
}



#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
namespace dx3d {
	class IndexBuffer : public GraphicsResource
	{
	public:
		explicit IndexBuffer(const GraphicsResourceDesc& desc);
		void load(void* list_indices, UINT size_list);
		UINT getSizeIndexList();
	private:
		UINT m_size_list{};
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		friend class DeviceContext;
	};
}



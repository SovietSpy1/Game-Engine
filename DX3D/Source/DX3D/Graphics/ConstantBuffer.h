#include <DX3D/Graphics/GraphicsResource.h>
namespace dx3d {
	class ConstantBuffer : public GraphicsResource {
	public:
		explicit ConstantBuffer(const GraphicsResourceDesc& desc);
		void load(const ConstantBufferDesc& desc);
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		friend class DeviceContext;
	};
}


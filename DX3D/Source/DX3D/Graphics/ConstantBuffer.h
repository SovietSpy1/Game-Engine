#include <DX3D/Graphics/GraphicsResource.h>
#include <DX3D/Graphics/DeviceContext.h>
#pragma once
namespace dx3d {
	class ConstantBuffer : public GraphicsResource {
	public:
		explicit ConstantBuffer(const GraphicsResourceDesc& desc);
		template<typename T>
		void load(const T& cBuffer)
		{
			if (m_buffer) m_buffer->Release();
			_declspec(align(16)) struct alignedT {
				T value;
			};
			alignedT alignedBuffer = alignedT{ cBuffer };
			D3D11_BUFFER_DESC desc{};
			desc.ByteWidth = sizeof(alignedT);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			D3D11_SUBRESOURCE_DATA initData{};
			initData.pSysMem = &alignedBuffer;
			DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, &initData, m_buffer.GetAddressOf()), "createconstantbuffer failed.");
		}
		void UpdateSubresource(void* subData) {
			DeviceContext::get()->UpdateSubresource(m_buffer.Get(), subData);
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		friend class DeviceContext;
	};
}


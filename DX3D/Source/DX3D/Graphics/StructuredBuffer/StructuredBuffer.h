#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
namespace dx3d {
	struct BufferPair {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav;
	};
	class StructuredBuffer : public GraphicsResource
	{
	public:
		explicit StructuredBuffer(const GraphicsResourceDesc& desc);
		template <typename T>
		Microsoft::WRL::ComPtr<ID3D11Buffer> CreateStagingBuffer(int size) {
			Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
			D3D11_BUFFER_DESC desc{};
			desc.ByteWidth = size * sizeof(T);
			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, buffer.GetAddressOf()), "create staging buffer failed");
			return buffer;
		}
		template <typename T>
		std::unique_ptr<BufferPair> CreateBufferPair(int size) {
			std::unique_ptr<BufferPair> bufferPair = std::make_unique<BufferPair>();
			Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
			Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav;
			//buffer
			D3D11_BUFFER_DESC desc{};
			desc.ByteWidth = size * sizeof(T);
			desc.StructureByteStride = sizeof(T);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.Buffer.NumElements = size;
			srvDesc.Buffer.FirstElement = 0;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Buffer.NumElements = size;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.Flags = 0;

			DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, buffer.GetAddressOf()), "create structured buffer failed");
			DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(buffer.Get(), &srvDesc, srv.GetAddressOf()), "create structured srv failed");
			DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(buffer.Get(), &uavDesc, uav.GetAddressOf()), "create structured uav failed");
			bufferPair->buffer = buffer;
			bufferPair->srv = srv;
			bufferPair->uav = uav;
			return bufferPair;
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> colors;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> colors_srv;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> colors_uav;

		Microsoft::WRL::ComPtr<ID3D11Buffer> colorsStaging;
		//density
		Microsoft::WRL::ComPtr<ID3D11Buffer> dens_last;
		Microsoft::WRL::ComPtr<ID3D11Buffer> dens_current;
		Microsoft::WRL::ComPtr<ID3D11Buffer> dens_start;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dens_srv_last;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dens_srv_current;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dens_srv_start;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> dens_uav_last;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> dens_uav_current;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> dens_uav_start;

		//velocity
		Microsoft::WRL::ComPtr<ID3D11Buffer> velX_last;
		Microsoft::WRL::ComPtr<ID3D11Buffer> velX_current;
		Microsoft::WRL::ComPtr<ID3D11Buffer> velX_start;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> velX_srv_last;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> velX_srv_current;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> velX_srv_start;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> velX_uav_last;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> velX_uav_current;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> velX_uav_start;

		Microsoft::WRL::ComPtr<ID3D11Buffer> velY_last;
		Microsoft::WRL::ComPtr<ID3D11Buffer> velY_current;
		Microsoft::WRL::ComPtr<ID3D11Buffer> velY_start;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> velY_srv_last;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> velY_srv_current;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> velY_srv_start;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> velY_uav_last;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> velY_uav_current;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> velY_uav_start;

		//divergence
		Microsoft::WRL::ComPtr<ID3D11Buffer> div;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> div_srv;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> div_uav;

		//pressure
		Microsoft::WRL::ComPtr<ID3D11Buffer> pressure_current;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pressure_srv_current;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> pressure_uav_current;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pressure_last;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pressure_srv_last;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> pressure_uav_last;
		friend class Smoke;
		friend class Display;
	};
}



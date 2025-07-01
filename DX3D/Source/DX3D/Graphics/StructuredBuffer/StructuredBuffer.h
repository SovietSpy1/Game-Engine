#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
namespace dx3d {
	class StructuredBuffer : public GraphicsResource
	{
	public:
		explicit StructuredBuffer(const GraphicsResourceDesc& desc);
		void loadSmokeBuffers(int resolution);
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



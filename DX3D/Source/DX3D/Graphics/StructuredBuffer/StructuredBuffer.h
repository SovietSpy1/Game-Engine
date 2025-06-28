#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
namespace dx3d {
	class StructuredBuffer : public GraphicsResource
	{
	public:
		explicit StructuredBuffer(const GraphicsResourceDesc& desc);
		void loadSmokeBuffers(int resolution);
	private:
		//density
		Microsoft::WRL::ComPtr<ID3D11Buffer> dens_last;
		Microsoft::WRL::ComPtr<ID3D11Buffer> dens_current;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dens_srv_last;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dens_srv_current;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> dens_uav_last;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> dens_uav_current;

		//velocity
		Microsoft::WRL::ComPtr<ID3D11Buffer> vel_last;
		Microsoft::WRL::ComPtr<ID3D11Buffer> vel_current;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> vel_srv_last;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> vel_srv_current;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> vel_uav_last;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> vel_uav_current;

		//divergence
		Microsoft::WRL::ComPtr<ID3D11Buffer> div;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> div_srv;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> div_uav;

		//pressure
		Microsoft::WRL::ComPtr<ID3D11Buffer> pressure;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pressure_srv;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> pressure_uav;
		friend class Smoke;
	};
}



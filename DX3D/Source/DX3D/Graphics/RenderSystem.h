#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <DX3D/Core/Common.h>
#include <DX3D/Core/Base.h>
#include <d3d11.h>
#include <wrl.h>
namespace dx3d {
	class RenderSystem final: public Base, public std::enable_shared_from_this<RenderSystem>
	{
	public:
		explicit RenderSystem(const RenderSystemDesc& desc);
		virtual ~RenderSystem() override;
		SwapChainPtr createSwapChain(const SwapChainDesc& desc) const;
		std::shared_ptr<DeviceContext> createDeviceContext() const;
		std::shared_ptr<VertexBuffer> createVertexBuffer() const;
		std::shared_ptr<Mesh> createMesh() const;
		std::shared_ptr<IndexBuffer> createIndexBuffer() const;
		std::shared_ptr<ConstantBuffer> createConstantBuffer() const;
		std::shared_ptr<TextureManager> createTextureManager() const;
		std::shared_ptr<MeshManager> createMeshManager() const;
		void compileVertexShader(const WCHAR* vertexShaderPath, Microsoft::WRL::ComPtr<ID3DBlob>& vertexBlob) const;
		void compilePixelShader(const WCHAR* pixelShaderPath, Microsoft::WRL::ComPtr<ID3DBlob>& pixelBlob) const;
		void createVertexShader(Microsoft::WRL::ComPtr<ID3DBlob>& vertexBlob, Microsoft::WRL::ComPtr<ID3D11VertexShader>& vertexShader) const;
		void createPixelShader(Microsoft::WRL::ComPtr<ID3DBlob>& pixelBlob, Microsoft::WRL::ComPtr<ID3D11PixelShader>& pixelShader) const;
	private:
		GraphicsResourceDesc getGraphicsResourceDesc() const noexcept;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice{};
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext{};
		Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgiDevice{};
		Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgiAdapter{};
		Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgiFactory{};
	};
}



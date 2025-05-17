#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
namespace dx3d {
	class DeviceContext : public GraphicsResource {
	public:
		explicit DeviceContext(const DeviceContextDesc& desc, const GraphicsResourceDesc& gDesc);
		void clearRenderTargetColor(std::shared_ptr<SwapChain> swap_chain,vec4 rgba);
		void setVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buffer);
		void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
		void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);
		void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
		void SetViewportSize(UINT width, UINT height);
		void loadShaders(Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader);
		void createTransparentBlendState();
		void setBlendState();
		void createBackfaceRasterizerState();
		void setRasterState();
		void setConstantBuffer(const ConstantBuffer& cBuffer);
		void setIndexBuffer(const IndexBuffer& iBuffer);
	private:
		ID3D11DeviceContext& m_device_context;
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	};
}
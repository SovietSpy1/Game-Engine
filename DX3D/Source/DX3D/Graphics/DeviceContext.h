#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <unordered_map>
namespace dx3d {
	class DeviceContext : public GraphicsResource {
	public:
		explicit DeviceContext(const DeviceContextDesc& desc, const GraphicsResourceDesc& gDesc);
		void clearRenderTargetColor(std::shared_ptr<SwapChain> swap_chain,vec4 rgba);
		void setVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buffer);
		void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
		void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);
		void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
		void DrawLines(UINT vertex_count, UINT start_vertex_index);
		void DrawIndexedLines(UINT index_count, UINT start_vertex_index, UINT start_index_location);
		void SetViewportSize(UINT width, UINT height);
		void loadShaders(Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader);
		void createTransparentBlendState();
		void setBlendState();
		void createBackfaceRasterizerState();
		void createFrontfaceRasterizerState();
		void createAllfaceRasterizerState();
		void setRasterizerState(RasterizerStateType rasterizerType);
		void setConstantBuffers(const std::unordered_map<UINT, std::shared_ptr<ConstantBuffer>> buffers);
		void setIndexBuffer(std::shared_ptr<IndexBuffer> iBuffer);
		void setTexture(std::vector<std::shared_ptr<Texture>> texture);
	private:
		ID3D11DeviceContext& m_device_context;
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> backFaceRasterizerState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> frontFaceRasterizerState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> allFaceRasterizerState;
	};
}
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
		void SetViewportSize(UINT width, UINT height);
		void createVertexShader(ID3DBlob* shaderBlob);
		void createPixelShader(ID3DBlob* shaderBlob);
		void loadShaders();
		void createTransparentBlendState();
		void setBlendState();
		Microsoft::WRL::ComPtr<ID3DBlob> loadShaderBlob(const WCHAR* path);
		Microsoft::WRL::ComPtr<ID3DBlob> loadPixelShaderBlob(const WCHAR* path);
	private:
		ID3D11DeviceContext& m_device_context;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	};
}
#include "DX3D/Graphics/RenderSystem.h"
#include <DX3D/Graphics/GraphicsLogUtils.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/ResourceManager/MeshManager/MeshManager.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/TextureManager.h>
#include <d3dcompiler.h>
#include <DX3D/Object/Axis.h>
#include <DX3D/Object/Material.h>
#include <DX3D/Graphics/StructuredBuffer/StructuredBuffer.h>
#include <iostream>
using namespace dx3d;
dx3d::RenderSystem::RenderSystem(const RenderSystemDesc& desc) : Base(desc.base)
{
	if (S == nullptr) {
		S = this;
	}
	D3D_FEATURE_LEVEL featureLevel{};
	D3D_FEATURE_LEVEL featureLevels[] = {
	D3D_FEATURE_LEVEL_12_2,
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
	D3D_FEATURE_LEVEL_9_2,
	D3D_FEATURE_LEVEL_9_1
	};
	UINT createDeviceFlags{};
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	DX3DGraphicsLogErrorAndThrow(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
		&m_d3dDevice, &featureLevel, &m_d3dContext), "Direct3D11 initialization failed.");
	DX3DGraphicsLogErrorAndThrow(m_d3dDevice->QueryInterface(IID_PPV_ARGS(&m_dxgiDevice)), "Query Interface failed to retrieve IDXGIDevice;");
	DX3DGraphicsLogErrorAndThrow(m_dxgiDevice->GetParent(IID_PPV_ARGS(&m_dxgiAdapter)), "GetParent failed to retrieve IDXGIAdapter;");
	DX3DGraphicsLogErrorAndThrow(m_dxgiAdapter->GetParent(IID_PPV_ARGS(&m_dxgiFactory)), "GetParent failed to retrieve IDXGIFactory;");
	switch (featureLevel) {
	case D3D_FEATURE_LEVEL_12_2:
		std::cout << "Feature Level: 12.2" << std::endl;
		break;
	case D3D_FEATURE_LEVEL_12_1:
		std::cout << "Feature Level: 12.1" << std::endl;
		break;
	case D3D_FEATURE_LEVEL_12_0:
		std::cout << "Feature Level: 12.0" << std::endl;
		break;
	case D3D_FEATURE_LEVEL_11_1:
		std::cout << "Feature Level: 11.1" << std::endl;
		break;
	case D3D_FEATURE_LEVEL_11_0:
		std::cout << "Feature Level: 11.0" << std::endl;
		break;
	case D3D_FEATURE_LEVEL_10_1:
		std::cout << "Feature Level: 10.1" << std::endl;
		break;
	case D3D_FEATURE_LEVEL_10_0:
		std::cout << "Feature Level: 10.0" << std::endl;
		break;
	case D3D_FEATURE_LEVEL_9_3:
		std::cout << "Feature Level: 9.3" << std::endl;
		break;
	case D3D_FEATURE_LEVEL_9_2:
		std::cout << "Feature Level: 9.2" << std::endl;
		break;
	case D3D_FEATURE_LEVEL_9_1:
		std::cout << "Feature Level: 9.1" << std::endl;
		break;
	default:
		std::cout << "Unknown or unsupported feature level." << std::endl;
		break;
	}
}
dx3d::RenderSystem::~RenderSystem()
{
}

SwapChainPtr dx3d::RenderSystem::createSwapChain(const SwapChainDesc& desc) const
{
	return std::make_shared<SwapChain>(desc, getGraphicsResourceDesc());
}

std::shared_ptr<DeviceContext> dx3d::RenderSystem::createDeviceContext() const
{
	return std::make_shared<DeviceContext>(DeviceContextDesc(*m_d3dContext.Get()), getGraphicsResourceDesc());
}

std::shared_ptr<VertexBuffer> dx3d::RenderSystem::createVertexBuffer() const
{
	return std::make_shared<VertexBuffer>(getGraphicsResourceDesc());
}

std::shared_ptr<Mesh> dx3d::RenderSystem::createMesh() const
{
	return std::make_shared<Mesh>(getGraphicsResourceDesc());
}

std::shared_ptr<Material> dx3d::RenderSystem::createMaterial() const
{
	return std::make_shared<Material>(getGraphicsResourceDesc());
}

std::shared_ptr<IndexBuffer> dx3d::RenderSystem::createIndexBuffer() const
{
	return std::make_shared<IndexBuffer>(getGraphicsResourceDesc());
}

std::shared_ptr<ConstantBuffer> dx3d::RenderSystem::createConstantBuffer() const
{
	return std::make_shared<ConstantBuffer>(getGraphicsResourceDesc());
}

std::shared_ptr<TextureManager> dx3d::RenderSystem::createTextureManager() const
{
	return std::make_shared<TextureManager>(getGraphicsResourceDesc());
}

std::shared_ptr<MeshManager> dx3d::RenderSystem::createMeshManager() const
{
	return std::make_shared<MeshManager>(getGraphicsResourceDesc());
}

std::shared_ptr<Axis> dx3d::RenderSystem::createAxis(const AxisDesc& desc) const
{
	return std::make_shared<Axis>(desc, getGraphicsResourceDesc());
}

std::shared_ptr<StructuredBuffer> dx3d::RenderSystem::createStructuredBuffer() const
{
	return std::make_shared<StructuredBuffer>(getGraphicsResourceDesc());
}

void dx3d::RenderSystem::compileVertexShader(const WCHAR* vertexShaderPath, Microsoft::WRL::ComPtr<ID3DBlob>& vertexBlob) const
{
	ID3DBlob* error_blob = nullptr;
	DX3DGraphicsLogErrorAndThrow(D3DCompileFromFile(
		vertexShaderPath,      // shader file name
		nullptr,                         // optional macros
		nullptr,                         // include interface
		"main",                          // entry point function
		"vs_5_0",                        // shader model
		0,                               // compile flags
		0,                               // effect flags
		vertexBlob.GetAddressOf(),                    // compiled shader
		&error_blob                      // errors
	), "compileVertexShader failed.");
	if (error_blob != nullptr) {
		error_blob->Release();
	}
}

void dx3d::RenderSystem::compilePixelShader(const WCHAR* pixelShaderPath, Microsoft::WRL::ComPtr<ID3DBlob>& pixelBlob) const
{
	ID3DBlob* error_blob = nullptr;
	DX3DGraphicsLogErrorAndThrow(D3DCompileFromFile(
		pixelShaderPath,      // shader file name
		nullptr,                         // optional macros
		nullptr,                         // include interface
		"main",                          // entry point function
		"ps_5_0",                        // shader model
		0,                               // compile flags
		0,                               // effect flags
		pixelBlob.GetAddressOf(),                    // compiled shader
		&error_blob                      // errors
	), "compilePixelShader failed.");
	if (error_blob != nullptr) {
		error_blob->Release();
	}
}

void dx3d::RenderSystem::compileComputeShader(const WCHAR* computeShaderPath, Microsoft::WRL::ComPtr<ID3DBlob>& computeBlob) const
{
	ID3DBlob* error_blob = nullptr;
	DX3DGraphicsLogErrorAndThrow(D3DCompileFromFile(
		computeShaderPath,      // shader file name
		nullptr,                         // optional macros
		nullptr,                         // include interface
		"main",                          // entry point function
		"cs_5_0",                        // shader model
		0,                               // compile flags
		0,                               // effect flags
		computeBlob.GetAddressOf(),                    // compiled shader
		&error_blob                      // errors
	), "compileComputeShader failed.");
	if (error_blob != nullptr) {
		error_blob->Release();
	}
}

void dx3d::RenderSystem::createComputeShader(Microsoft::WRL::ComPtr<ID3DBlob>& computeBlob, Microsoft::WRL::ComPtr<ID3D11ComputeShader>& computeShader) const
{
	DX3DGraphicsLogErrorAndThrow(m_d3dDevice->CreateComputeShader(computeBlob->GetBufferPointer(), computeBlob->GetBufferSize(), nullptr, computeShader.GetAddressOf()), "createComputeShader failed.");
}

void dx3d::RenderSystem::createVertexShader(Microsoft::WRL::ComPtr<ID3DBlob>& vertexBlob, Microsoft::WRL::ComPtr<ID3D11VertexShader> &vertexShader) const
{
	DX3DGraphicsLogErrorAndThrow(m_d3dDevice->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf()), "createVertexShader failed.");
}

void dx3d::RenderSystem::createPixelShader(Microsoft::WRL::ComPtr<ID3DBlob>& pixelBlob, Microsoft::WRL::ComPtr<ID3D11PixelShader>& pixelShader) const
{
	DX3DGraphicsLogErrorAndThrow(m_d3dDevice->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf()), "createPixelShader failed.");
}

GraphicsResourceDesc dx3d::RenderSystem::getGraphicsResourceDesc() const noexcept
{
	return { {m_logger}, shared_from_this(),*m_d3dDevice.Get(), *m_dxgiFactory.Get()};
}

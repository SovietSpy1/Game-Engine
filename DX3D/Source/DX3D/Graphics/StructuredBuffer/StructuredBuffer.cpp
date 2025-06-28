#include "DX3D/Graphics/StructuredBuffer/StructuredBuffer.h"

dx3d::StructuredBuffer::StructuredBuffer(const GraphicsResourceDesc& desc) : GraphicsResource(desc)
{
}

void dx3d::StructuredBuffer::loadSmokeBuffers(int resolution)
{
	int size = (resolution + 2) * (resolution + 2);
	//buffers
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = size * sizeof(float);
	desc.StructureByteStride = sizeof(float);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	//density
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, dens_last.GetAddressOf()), "create dens last buffer failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, dens_current.GetAddressOf()), "create dens current buffer failed");
	//pressure
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, pressure.GetAddressOf()), "create pressure buffer failed");
	//divergence
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, div.GetAddressOf()), "create divergence buffer failed");
	//velocity
	desc.ByteWidth = size * sizeof(vec2);
	desc.StructureByteStride = sizeof(vec2);
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, vel_last.GetAddressOf()), "create velocity last buffer failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, vel_current.GetAddressOf()), "create velocity current buffer failed");

	//shader resource views
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Buffer.NumElements = size;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	//density
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(dens_last.Get(), &srvDesc, dens_srv_last.GetAddressOf()), "create dens last srv failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(dens_current.Get(), &srvDesc, dens_srv_current.GetAddressOf()), "create dens current srv failed");
	//pressure
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(pressure.Get(), &srvDesc, pressure_srv.GetAddressOf()), "create pressure srv failed");
	//divergence
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(div.Get(), &srvDesc, div_srv.GetAddressOf()), "create divergence srv failed");
	//velocity 
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(vel_last.Get(), &srvDesc, vel_srv_last.GetAddressOf()), "create velocity last srv failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(vel_current.Get(), &srvDesc, vel_srv_current.GetAddressOf()), "create velocity current srv failed");

	//unordered access views
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = size;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.Flags = 0;
	//density
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(dens_last.Get(), &uavDesc, dens_uav_last.GetAddressOf()), "create dens last uav failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(dens_current.Get(), &uavDesc, dens_uav_current.GetAddressOf()), "create dens current uav failed");
	//pressure
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(pressure.Get(), &uavDesc, pressure_uav.GetAddressOf()), "create pressure uav failed");
	//divergence
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(div.Get(), &uavDesc, div_uav.GetAddressOf()), "create divergence uav failed");
	//velocity 
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(vel_last.Get(), &uavDesc, vel_uav_last.GetAddressOf()), "create velocity last uav failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(vel_current.Get(), &uavDesc, vel_uav_current.GetAddressOf()), "create velocity current uav failed");


}
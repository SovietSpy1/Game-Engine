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

	//std::vector<float> initYVel(size * size, 0.5f);
	D3D11_SUBRESOURCE_DATA initValues{};
	//initValues.pSysMem = initYVel.data();
	initValues.SysMemPitch = resolution * sizeof(float);
	initValues.SysMemSlicePitch = 0;
	
	//density
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, dens_last.GetAddressOf()), "create dens last buffer failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, dens_current.GetAddressOf()), "create dens current buffer failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, dens_start.GetAddressOf()), "create dens start buffer failed");
	//pressure
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, pressure_current.GetAddressOf()), "create pressure buffer failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, pressure_last.GetAddressOf()), "create pressure buffer failed");
	//divergence
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, div.GetAddressOf()), "create divergence buffer failed");
	//velocity
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, velX_last.GetAddressOf()), "create velocity last buffer failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, velX_current.GetAddressOf()), "create velocity current buffer failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, velX_start.GetAddressOf()), "create velocity start buffer failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, velY_last.GetAddressOf()), "create velocity last buffer failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, velY_current.GetAddressOf()), "create velocity current buffer failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, velY_start.GetAddressOf()), "create velocity start buffer failed");

	desc.ByteWidth = resolution * resolution * sizeof(vec4);
	desc.StructureByteStride = sizeof(vec4);
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, colors.GetAddressOf()), "create colors buffer failed");
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, nullptr, colorsStaging.GetAddressOf()), "create colors staging buffer failed");
	//shader resource views
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Buffer.NumElements = size;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	//density
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(dens_last.Get(), &srvDesc, dens_srv_last.GetAddressOf()), "create dens last srv failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(dens_current.Get(), &srvDesc, dens_srv_current.GetAddressOf()), "create dens current srv failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(dens_start.Get(), &srvDesc, dens_srv_start.GetAddressOf()), "create dens start srv failed");
	//pressure
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(pressure_current.Get(), &srvDesc, pressure_srv_current.GetAddressOf()), "create pressure srv failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(pressure_last.Get(), &srvDesc, pressure_srv_last.GetAddressOf()), "create pressure srv failed");
	//divergence
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(div.Get(), &srvDesc, div_srv.GetAddressOf()), "create divergence srv failed");
	//velocity 
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(velX_last.Get(), &srvDesc, velX_srv_last.GetAddressOf()), "create velocity last srv failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(velX_current.Get(), &srvDesc, velX_srv_current.GetAddressOf()), "create velocity current srv failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(velX_start.Get(), &srvDesc, velX_srv_start.GetAddressOf()), "create velocity start srv failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(velY_last.Get(), &srvDesc, velY_srv_last.GetAddressOf()), "create velocity last srv failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(velY_current.Get(), &srvDesc, velY_srv_current.GetAddressOf()), "create velocity current srv failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(velY_start.Get(), &srvDesc, velY_srv_start.GetAddressOf()), "create velocity start srv failed");
	srvDesc.Buffer.NumElements = resolution * resolution;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(colors.Get(), &srvDesc, colors_srv.GetAddressOf()), "create colors srv failed");
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
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(pressure_current.Get(), &uavDesc, pressure_uav_current.GetAddressOf()), "create pressure uav failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(pressure_last.Get(), &uavDesc, pressure_uav_last.GetAddressOf()), "create pressure uav failed");
	//divergence
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(div.Get(), &uavDesc, div_uav.GetAddressOf()), "create divergence uav failed");
	//velocity 
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(velX_last.Get(), &uavDesc, velX_uav_last.GetAddressOf()), "create velocity last uav failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(velX_current.Get(), &uavDesc, velX_uav_current.GetAddressOf()), "create velocity current uav failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(velY_last.Get(), &uavDesc, velY_uav_last.GetAddressOf()), "create velocity last uav failed");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(velY_current.Get(), &uavDesc, velY_uav_current.GetAddressOf()), "create velocity current uav failed");
	uavDesc.Buffer.NumElements = resolution * resolution;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(colors.Get(), &uavDesc, colors_uav.GetAddressOf()), "create colors uav failed");
}
#include "DX3D/Graphics/ConstantBuffer.h"

dx3d::ConstantBuffer::ConstantBuffer(const GraphicsResourceDesc& gDesc) : GraphicsResource(gDesc), m_buffer{}
{

}

void dx3d::ConstantBuffer::load(const ConstantBufferDesc& cBuffer)
{
	if (m_buffer) m_buffer->Release();
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(ConstantBufferDesc);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = &cBuffer;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&desc, &initData, m_buffer.GetAddressOf()), "createconstantbuffer failed.");
}

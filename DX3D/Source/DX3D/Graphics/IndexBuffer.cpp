#include "DX3D/Graphics/IndexBuffer.h"

dx3d::IndexBuffer::IndexBuffer(const GraphicsResourceDesc& desc) : GraphicsResource(desc), m_buffer(0)
{
}

void dx3d::IndexBuffer::load(void* list_indices, UINT size_list)
{
	if (m_buffer)m_buffer->Release();
	D3D11_BUFFER_DESC buff_desc{};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = 4 * size_list;
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA init_data{};
	init_data.pSysMem = list_indices;
	m_size_list = size_list;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&buff_desc, &init_data, m_buffer.GetAddressOf()), "CreteIndexBuffer failed.");
}

UINT dx3d::IndexBuffer::getSizeIndexList()
{
	return m_size_list;
}

#include "DX3D/Graphics/VertexBuffer.h"

dx3d::VertexBuffer::VertexBuffer(const GraphicsResourceDesc& desc): GraphicsResource(desc), m_layout(0), m_buffer(0)
{
}
std::vector<D3D11_INPUT_ELEMENT_DESC> dx3d::VertexBuffer::LayoutFromSemantics(std::vector<dx3d::IA> semantics) {
	int size = 0;
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout_desc;
	if (std::find(semantics.begin(), semantics.end(), dx3d::IA::POSITION) != semantics.end()) {
		D3D11_INPUT_ELEMENT_DESC position_desc{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, size, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout_desc.push_back(position_desc);
		size += 12; // 3 floats for position
	}
	if (std::find(semantics.begin(), semantics.end(), dx3d::IA::TEXCOORD) != semantics.end()) {
		D3D11_INPUT_ELEMENT_DESC texcoord_desc{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, size, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout_desc.push_back(texcoord_desc);
		size += 8;
	}
	if (std::find(semantics.begin(), semantics.end(), dx3d::IA::NORMAL) != semantics.end()) {
		D3D11_INPUT_ELEMENT_DESC texcoord_desc{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, size, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout_desc.push_back(texcoord_desc);
		size += 12;
	}
	if (std::find(semantics.begin(), semantics.end(), dx3d::IA::COLOR) != semantics.end()) {
		D3D11_INPUT_ELEMENT_DESC color_desc{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, size, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout_desc.push_back(color_desc);
		size += 16; // 4 floats for color
	}
	return layout_desc;
}
void dx3d::VertexBuffer::load(void* list_vertices, UINT size_vertex, UINT size_list, void*shader_byte_code, UINT size_byte_shader, std::vector<dx3d::IA> semantics)
{
	if (m_buffer)m_buffer->Release();
	if (m_layout)m_layout->Release();
	D3D11_BUFFER_DESC buff_desc{};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA init_data{};
	init_data.pSysMem =list_vertices;
	m_size_vertex = size_vertex;
	m_size_list = size_list;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateBuffer(&buff_desc, &init_data, m_buffer.GetAddressOf()), "CreteVertexBuffer failed.");
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = LayoutFromSemantics(semantics);
	UINT size_layout = layout.size();
	DX3DGraphicsLogErrorAndThrow(m_device.CreateInputLayout(layout.data(), size_layout, shader_byte_code, size_byte_shader, m_layout.GetAddressOf()), "InputLayoutCreation failed.");
}

UINT dx3d::VertexBuffer::getSizeVertexList()
{
	return m_size_list;
}



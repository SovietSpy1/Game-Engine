#include "DX3D/Graphics/VertexBuffer.h"
#include <DX3D/Graphics/RenderSystem.h>
dx3d::VertexBuffer::VertexBuffer(const GraphicsResourceDesc& desc): GraphicsResource(desc), m_layout(0), m_buffer(0)
{
	if (vertexLayoutBlob == nullptr) {
		const WCHAR* vertexShaderPath = L"DX3D/Shaders/VertexMeshLayout/VertexShader.hlsl";
		m_renderSystem->compileVertexShader(vertexShaderPath, vertexLayoutBlob);
	}
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
		D3D11_INPUT_ELEMENT_DESC color_desc{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, size, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout_desc.push_back(color_desc);
		size += 4; // 4 floats for color
	}
	return layout_desc;
}
void dx3d::VertexBuffer::load(void* list_vertices, UINT size_vertex, UINT size_list, void*shader_byte_code, UINT size_byte_shader, std::vector<dx3d::IA> semantics)
{
	if (shader_byte_code == nullptr || size_byte_shader == 0) {
		shader_byte_code = vertexLayoutBlob->GetBufferPointer();
		size_byte_shader = vertexLayoutBlob->GetBufferSize();
	}
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

std::vector<dx3d::vertex> dx3d::VertexBuffer::FillInVertexData(std::unordered_map<IA, vertexData> data)
{
	std::vector<vertex> verts{};
	auto it = data.find(IA::POSITION);
	if (it == data.end()) {
		
		return verts;
	}
	size_t size = std::get<std::vector<Vector3D>>(it->second).size();
	std::vector<Vector3D> positions(size);
	std::vector<Vector2D> uvs(size);
	std::vector<Vector3D> normals(size);
	std::vector<vec4_32> colors(size);
	verts.resize(size);
	it = data.begin();
	while (it != data.end())
	{
		switch (it->first) {
		case IA::POSITION:
			positions = std::get<std::vector<Vector3D>>(it->second);
			break;
		case IA::TEXCOORD:
			uvs = std::get<std::vector<Vector2D>>(it->second);
			break;
		case IA::NORMAL:
			normals = std::get<std::vector<Vector3D>>(it->second);
			break;
		case IA::COLOR:
			colors = std::get<std::vector<vec4_32>>(it->second);
			break;
		default:
			break;
		}
		it++;
	}
	for (size_t i = 0; i < size; i++)
	{
		vertex vtx{};
		vtx.position = positions[i];
		vtx.uv = uvs[i];
		vtx.normal = normals[i];
		vtx.color = colors[i];
		verts.at(i) = vtx;
	}
	return verts;
}



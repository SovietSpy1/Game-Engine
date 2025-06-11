#include "DX3D/Graphics/ResourceManager/Resource.h"
dx3d::Resource::Resource(const wchar_t* full_path, const GraphicsResourceDesc& desc) : GraphicsResource(desc), full_path{ full_path }
{
}

dx3d::Resource::~Resource()
{
}

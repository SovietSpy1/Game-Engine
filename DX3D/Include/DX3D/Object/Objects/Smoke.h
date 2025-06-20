#pragma once
#include <DX3D/Core/Base.h>
#include <wrl.h>
#include <algorithm>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/TextureManager.h>
namespace dx3d {
	class Smoke : public Base{
	public:
		Smoke(const BaseDesc& desc, int resolution) : Base(desc), resolution(resolution){
			velocities.resize(resolution * resolution);
			temperatures.resize(resolution * resolution);
			densities.resize(resolution * resolution);
			colors.resize(resolution * resolution);
			texture = GraphicsEngine::get()->getTextureManager().create2DSmokeTexture(resolution);
		}
		void AddToSmoke(Vector3D ndcPosition, float radius, vec4 color) {
			Vector3D arrayPos = Vector3D((int)((ndcPosition.x + 1) / 2.0f * resolution), (int)((ndcPosition.y + 1) / 2.0f * resolution), 0);
			int arrayRad = radius / 2.0f * resolution;
			int leftSide = std::clamp((int)arrayPos.x - arrayRad, 0, resolution);
			int rightSide = std::clamp((int)arrayPos.x + arrayRad, 0, resolution);
			int topSide = std::clamp((int)arrayPos.y + arrayRad, 0, resolution);
			int bottomSide = std::clamp((int)arrayPos.y - arrayRad, 0, resolution);
			for (int y = bottomSide; y < topSide; y++) {
				for (int x = leftSide; x < rightSide; x++) {
					if ((Vector3D(x, y, 0) - arrayPos).mag() <= arrayRad) {
						colors.at(y * resolution + x) = color;
					}
				}
			}
		}
		std::vector<Vector3D> velocities;
		std::vector<vec4> colors;
		std::vector<float> temperatures;
		std::vector<float> densities;
		int resolution;
		std::shared_ptr<Texture> texture;
	};
}
#pragma once
#include <DX3D/Core/Base.h>
#include <wrl.h>
#include <algorithm>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/TextureManager.h>
#include <DX3D/Graphics/ResourceManager/MeshManager/MeshManager.h>
#include <DX3D/Object/GameObject.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/Texture.h>
#include <iostream>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/StructuredBuffer/StructuredBuffer.h>
#define IX(x,y) ((y) * (resolution+2) + x)
#define SWAP(x0, x) {std::swap(x0, x);}
namespace dx3d {
	class Smoke : public GameObject{
	public:
		struct vec2_int {
			int x, y;
		};
		struct smokeConstantBufferDesc {
			int resolution;
			float dt;
			float diff;
			float visc;
			vec2_int emissionPoint;
			vec2 emissionVelocity;
			int emissionRadius;
			float emissionDensity;
		};
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> SourceComputeShader;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> AdvectionComputeShader;
		DeviceContext* dC{};
		smokeConstantBufferDesc smokeBuffDesc{};
		std::shared_ptr<StructuredBuffer> structuredBuffer;
		std::shared_ptr<ConstantBuffer> constantBuffer;
		std::vector<float> pressure;
		std::vector<float> div;
		std::vector<float> oldvX;
		std::vector<float> oldvY;
		std::vector<float> vX;
		std::vector<float> vY;
		std::vector<vec4_32> colors;
		std::vector<float> densities;
		std::vector<float> oldDensities;
		float darkAmp = 10.0f;
		float eVX = 0.0f;
		float eVY = 10.0f;
		float visc = 0.0001f;
		float diff = 0.001f;
		float radius = 0.1f;
		int resolution;
		Smoke(const BaseDesc& desc, int res) : GameObject(desc), resolution(res){
			dC = DeviceContext::get();
			constantBuffer = GraphicsEngine::get()->getRenderSystem().createConstantBuffer();
			structuredBuffer = RenderSystem::get()->createStructuredBuffer();
			UINT size = resolution + 2;
			structuredBuffer->loadSmokeBuffers(resolution);
			vX.resize(size * size,0.0f);
			vY.resize(size * size,0.0f);
			densities.resize(size * size,0.0f);
			oldvX.resize(size * size,0.0f);
			oldvY.resize(size * size,0.0f);
			oldDensities.resize(size * size,0.0f);
			pressure.resize(size * size,0.0f);
			div.resize(size * size,0.0f);
			colors.resize(resolution * resolution);
			std::vector<vec4_32> baseColors(resolution * resolution, vec4_32{ 0,0,0,0 });
			colors = baseColors;
			InputSystem::get()->addListener(this);
			smokeBuffDesc.dt = Time::deltaTime;
			smokeBuffDesc.diff = diff;
			smokeBuffDesc.visc = visc;
			smokeBuffDesc.resolution = resolution;
			constantBuffer->load(smokeBuffDesc);
		}
		void Update() override{
			SmokeUpdate();
			for (int y = 1; y <= resolution; y++) {
				for (int x = 1; x <= resolution; x++) {
					colors.at((x-1) + (y-1) * resolution).a = 255 * densities.at(IX(x,y));
				}
			}
			Material* material = GetComponent<Material>();
			if (material != nullptr) {
				material->textures.at(0)->MapToTexture(colors, resolution);
			}
		}
		void SmokeUpdate() {
			AddSource();
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->CSSetConstantBuffers({{0, constantBuffer}});
			dC->CSSetUAVS({ structuredBuffer->dens_uav_current.Get(), structuredBuffer->vel_uav_current.Get() });
			dC->CSSetSRVS({ structuredBuffer->dens_srv_current.Get(), structuredBuffer->vel_srv_current.Get() }	);
			dC->CSSetShader({})
			VelUpdate();
			DensUpdate();
			TextureUpdate();
		}
		void TextureUpdate() {

		}
		void AddSource() {
			UINT xPos = 0.5f * resolution;
			UINT yPos = 0.1f * resolution;
			float ranVelocityY = std::rand() % 2;
			float ranVelocityX = std::rand() % 20;
			AddToSmoke(Vector3D(xPos, yPos, 0), 0.05f, vec4_32{ 0,0,0,255 }, -10 + ranVelocityX, eVY);
		}
		void VelUpdate() {
			SWAP(oldvX, vX);
			SWAP(oldvY, vY);
			Diffuse(vX.data(), oldvX.data(), visc, 1);
			Diffuse(vY.data(), oldvY.data(), visc, 2);
			Project(vX.data(), vY.data(), pressure.data(), div.data());
			SWAP(oldvX, vX);
			SWAP(oldvY, vY);
			Advect(vX.data(), oldvX.data(), oldvX.data(), oldvY.data(), 1);
			Advect(vY.data(), oldvY.data(), oldvX.data(), oldvY.data(), 2);
			Project(vX.data(), vY.data(), pressure.data(), div.data());
		}
		void DensUpdate() {
			SWAP(oldDensities, densities);
			Diffuse(densities.data(), oldDensities.data(), diff, 0);
			SWAP(oldDensities, densities);
			Advect(densities.data(), oldDensities.data(), vX.data(), vY.data(), 0);
		}
		void AddToSmoke(Vector3D position, float radius, vec4_32 color, float xDir = 0, float yDir = 0) {
			int arrayRad = radius * resolution;
			int leftSide = std::clamp((int)position.x - arrayRad+1, 1, resolution);
			int rightSide = std::clamp((int)position.x + arrayRad+1, 1, resolution);
			int topSide = std::clamp((int)position.y + arrayRad + 1, 1, resolution);
			int bottomSide = std::clamp((int)position.y - arrayRad + 1, 1, resolution);
			float distance;
			/*smokeBuffDesc.emissionPoint = vec2_int{(int)position.x, (int)position.y};
			smokeBuffDesc.emissionRadius = radius;
			smokeBuffDesc.emissionDensity = darkAmp;
			smokeBuffDesc.emissionVelocity = vec2{ xDir,yDir };*/
			for (int y = bottomSide; y <= topSide; y++) {
				for (int x = leftSide; x <= rightSide; x++) {
					distance = (Vector3D(x, y, 0) - position).mag();
					if (distance <= arrayRad) {
						float dens = (1.0f - distance / arrayRad);
						densities.at(IX(x, y)) = std::clamp(densities.at(IX(x,y)) + dens * darkAmp * Time::deltaTime, 0.0f, 1.0f);
						vX.at(IX(x, y)) = std::clamp(vX.at(IX(x, y)) + dens * xDir * Time::deltaTime, -10.0f, 10.0f);
						vY.at(IX(x, y)) = std::clamp(vY.at(IX(x, y)) + dens * yDir * Time::deltaTime, -10.0f, 10.0f);
					}
				}
			}
			Material* material = GetComponent<Material>();
			if (material != nullptr) {
				material->textures.at(0)->MapToTexture(colors, resolution);
			}
		}
		virtual void onLeftMouseDown(const Point& mouse_pos) override {
			
		}
		virtual void onHoldLeftMouseDown(const Point& mouse_pos) override {
			Rect d = Display::get()->m_size;
			float o = 1.1f;
			float x = o / d.height * (mouse_pos.x - 0.5f * d.width) + 0.5f;
			float y = (float)(mouse_pos.y - 0.5f * d.height) * o / d.height + 0.5f;
			if (x < 0 || y < 0 || x >= 1 || y >= 1) {
				return;
			}
			int coordX = x * resolution;
			int coordY = y * resolution;
			AddToSmoke(Vector3D(coordX, coordY, 0), radius, vec4_32{ 0,0,0,255 });
		}
		// StamFluidForGames
		void Advect(float* newData, float* oldData, float* xVel, float* yVel, int b) {
			for (int y = 1; y <= resolution; y++) {
				for (int x = 1; x <= resolution; x++) {
					//find positions that current positions came from using current velocity
					float xPos = x - xVel[IX(x, y)] * Time::deltaTime * resolution;
					xPos = std::clamp(xPos, 0.0f, (float)resolution);
					float yPos = y - yVel[IX(x, y)] * Time::deltaTime * resolution;
					yPos = std::clamp(yPos, 0.0f, (float)resolution);
					int i = (int)xPos;
					int j = (int)yPos;
					float iProp = xPos - i;
					float jProp = yPos - j;
					float bottomLeftData = (1-iProp) * (1-jProp) * oldData[IX(i,j)];
					float bottomRightData = (iProp) * (1 - jProp) * oldData[IX(i + 1, j)];
					float topLeftData = (1 - iProp) * (jProp)*oldData[IX(i, j + 1)];
					float topRightData = (iProp) * (jProp)*oldData[IX(i + 1, j + 1)];
					newData[IX(x, y)] = bottomLeftData + bottomRightData + topLeftData + topRightData;
				}
			}
			set_bnd(b, newData);
		}
		void AddSource(float* dest, float* src) {
			for (UINT y = 0; y < resolution + 2; y++) {
				for (UINT x = 0; x < resolution + 2; x++) {
					dest[IX(x, y)] += src[IX(x, y)];
				}
			}
		}
		void Diffuse(float* newData, float* oldData, float diff, int b) {
			float a = resolution * resolution * diff * Time::deltaTime;
			int adaptiveIterations = std::max(5, 20);
			for (int k = 0; k < adaptiveIterations; k++) {
				for (int y = 1; y <= resolution; y++) {
					for (int x = 1; x <= resolution; x++) {
						newData[IX(x, y)] = (oldData[IX(x, y)] + a * (newData[IX(x - 1, y)] + newData[IX(x + 1, y)] + newData[IX(x, y + 1)] + newData[IX(x, y - 1)])) / (4 * a + 1);
					}
				}
			}
			set_bnd(b, newData);
		}
		void Project(float* xVel, float* yVel, float* pressure, float* div) {
			float h = (1.0f / (float)resolution);
			for (UINT y = 1; y <= resolution; y++) {
				for (UINT x = 1; x <= resolution; x++) {
					div[IX(x, y)] = -0.5f *h * (xVel[IX(x + 1, y)] - xVel[IX(x - 1, y)] + yVel[IX(x, y + 1)] - yVel[IX(x, y - 1)]);
					pressure[IX(x, y)] = 0;
				}
			}
			set_bnd(0, div); 
			set_bnd(0, pressure);
			int adaptiveIterations = std::max(5, 20);
			for (int k = 0; k < adaptiveIterations; k++) {
				for (UINT y = 1; y <= resolution; y++) {
					for (UINT x = 1; x <= resolution; x++) {
						pressure[IX(x, y)] = (div[IX(x, y)] + pressure[IX(x + 1, y)] + pressure[IX(x - 1, y)] + pressure[IX(x, y + 1)] + pressure[IX(x, y - 1)]) / 4.0f;
					}
					set_bnd(0, pressure);
				}
			}
			for (UINT y = 1; y <= resolution; y++) {
				for (UINT x = 1; x <= resolution; x++) {
					xVel[IX(x, y)] -= (pressure[IX(x + 1, y)] - pressure[IX(x - 1, y)]) * 0.5f / h;
					yVel[IX(x,y)] -= (pressure[IX(x, y+1)] - pressure[IX(x, y-1)]) * 0.5f / h;
				}
			}
			set_bnd(1, xVel); 
			set_bnd(2, yVel);
		}
		void set_bnd(int b, float* x)
		{
			int i;
			for (i = 1; i <= resolution; i++) {
				x[IX(0, i)] = b == 1 ?  -1 * x[IX(1, i)] : x[IX(1, i)];
				x[IX(resolution + 1, i)] = b == 1 ? -1 * x[IX(resolution, i)] : x[IX(resolution, i)];
				x[IX(i, 0)] = b == 2 ? -1 * x[IX(i, 1)] : x[IX(i, 1)];
				x[IX(i, resolution + 1)] = b == 2 ? -1 * x[IX(i, resolution)] : x[IX(i, resolution)];
			}
			x[IX(0, 0)] = 0.5 * (x[IX(1, 0)] + x[IX(0, 1)]);
			x[IX(0, resolution + 1)] = 0.5 * (x[IX(1, resolution + 1)] + x[IX(0, resolution)]);
			x[IX(resolution + 1, 0)] = 0.5 * (x[IX(resolution, 0)] + x[IX(resolution + 1, 1)]);
			x[IX(resolution + 1, resolution + 1)] = 0.5 * (x[IX(resolution, resolution + 1)] + x[IX(resolution + 1, resolution)]);
		}

	};
}
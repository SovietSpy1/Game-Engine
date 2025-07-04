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
			int emissionRadius;
			float emission;
			float max = 1.0f;
			float min = 0.0f;
			int b = 0;
		};
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> SourceCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> AdvectionCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> DiffusionCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> DivergenceCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> PressureCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> BoundaryCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> ClearCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> ProjectionXCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> ProjectionYCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> MapCS;
		const WCHAR* sourceCP = L"DX3D/Shaders/Smoke/Compute/Source.hlsl";
		const WCHAR* advectionCP = L"DX3D/Shaders/Smoke/Compute/Advection.hlsl";
		const WCHAR* diffusionCP = L"DX3D/Shaders/Smoke/Compute/Diffuse.hlsl";
		const WCHAR* divergenceCP = L"DX3D/Shaders/Smoke/Compute/Divergence.hlsl";
		const WCHAR* pressureCP = L"DX3D/Shaders/Smoke/Compute/Pressure.hlsl";
		const WCHAR* clearCP = L"DX3D/Shaders/Smoke/Compute/Clear.hlsl";
		const WCHAR* boundaryCP = L"DX3D/Shaders/Smoke/Compute/Boundary.hlsl";
		const WCHAR* projectionXCP = L"DX3D/Shaders/Smoke/Compute/ProjectionX.hlsl";
		const WCHAR* projectionYCP = L"DX3D/Shaders/Smoke/Compute/ProjectionY.hlsl";
		const WCHAR* mapCP = L"DX3D/Shaders/Smoke/Compute/Map.hlsl";
		DeviceContext* dC{};
		RenderSystem* rS{};
		smokeConstantBufferDesc smokeBuffDesc{};
		std::shared_ptr<StructuredBuffer> structuredBuffer;
		std::shared_ptr<ConstantBuffer> constantBuffer;

		std::vector<float> pressure;
		std::vector<float> div;
		std::vector<float> oldvX;
		std::vector<float> oldvY;
		std::vector<float> vX;
		std::vector<float> vY;
		std::vector<vec4> colors;
		std::vector<float> densities;
		std::vector<float> oldDensities;

		UINT groupCount;
		UINT borderCount;
		float darkAmp = 10.0f;
		float eVX = 0.0f;
		float eVY = 10.0f;
		float visc = 0.0001f;
		float diff = 0.001f;
		float radius = 0.1f;
		float maxSpeed = 5.0f;
		int resolution;
		Smoke(const BaseDesc& basedesc, int res) : GameObject(basedesc), resolution(res){
			InputSystem::get()->addListener(this);
			//CPUStart();
			GPUStart();
		}
		void Update() override {
			//SmokeUpdate();
			SmokeGraphicsUpdate();
		}
		void GPUStart() {
			groupCount = (resolution + 9) / 10;
			borderCount = (4 * resolution + 13) / 10;
			dC = DeviceContext::get();
			rS = RenderSystem::get();
			constantBuffer = rS->createConstantBuffer();
			structuredBuffer = rS->createStructuredBuffer();
			Microsoft::WRL::ComPtr<ID3DBlob> blob{};

			rS->compileComputeShader(sourceCP, blob);
			rS->createComputeShader(blob, SourceCS);

			rS->compileComputeShader(boundaryCP, blob);
			rS->createComputeShader(blob, BoundaryCS);

			rS->compileComputeShader(clearCP, blob);
			rS->createComputeShader(blob, ClearCS);

			rS->compileComputeShader(diffusionCP, blob);
			rS->createComputeShader(blob, DiffusionCS);

			rS->compileComputeShader(divergenceCP, blob);
			rS->createComputeShader(blob, DivergenceCS);

			rS->compileComputeShader(pressureCP, blob);
			rS->createComputeShader(blob, PressureCS);

			rS->compileComputeShader(projectionXCP, blob);
			rS->createComputeShader(blob, ProjectionXCS);

			rS->compileComputeShader(projectionYCP, blob);
			rS->createComputeShader(blob, ProjectionYCS);

			rS->compileComputeShader(advectionCP, blob);
			rS->createComputeShader(blob, AdvectionCS);

			rS->compileComputeShader(mapCP, blob);
			rS->createComputeShader(blob, MapCS);
			structuredBuffer->loadSmokeBuffers(resolution);
			UINT size = resolution + 2;
			smokeBuffDesc.dt = Time::deltaTime;
			smokeBuffDesc.diff = diff;
			smokeBuffDesc.visc = visc;
			smokeBuffDesc.resolution = resolution;
			smokeBuffDesc.max = 1.0f;
			smokeBuffDesc.min = 0.0f;
			constantBuffer->load(smokeBuffDesc);
		}
		void SmokeGraphicsUpdate() {
			dC->CSSetConstantBuffers({ {0, constantBuffer} });
			smokeBuffDesc.dt = Time::deltaTime;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			GraphicsAdvect();
			dC->CSSetConstantBuffers({ {0, constantBuffer} });
			GraphicsAddSource();
			//GraphicsDiffuse();
			GraphicsProject();
			Finalize();
		}
		void Finalize() {
			dC->CSSetShader(MapCS);
			dC->CSSetSRVS({ structuredBuffer->dens_srv_current.Get() });
			dC->CSSetUAVS({ structuredBuffer->colors_uav.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			dC->CopyResource(structuredBuffer->colorsStaging.Get(), structuredBuffer->colors.Get());
			D3D11_MAPPED_SUBRESOURCE map{};
			map = dC->GetReadableMap(structuredBuffer->colorsStaging);
			vec4* destPtr = reinterpret_cast<vec4*>(map.pData);
			std::vector<vec4> colorData(resolution * resolution);
			std::copy(destPtr, destPtr + (resolution * resolution), colorData.begin());
			GetComponent<Material>()->textures.at(0)->MapToTexture(colorData, resolution);
			dC->UnMap(structuredBuffer->colorsStaging.Get());
		}
		void GraphicsAdvect() {
			SWAP(structuredBuffer->dens_uav_current, structuredBuffer->dens_uav_last);
			SWAP(structuredBuffer->dens_srv_current, structuredBuffer->dens_srv_last);
			SWAP(structuredBuffer->velX_uav_current, structuredBuffer->velX_uav_last);
			SWAP(structuredBuffer->velX_srv_current, structuredBuffer->velX_srv_last);
			SWAP(structuredBuffer->velY_uav_current, structuredBuffer->velY_uav_last);
			SWAP(structuredBuffer->velY_srv_current, structuredBuffer->velY_srv_last);
			dC->CSSetSRVS({ structuredBuffer->dens_srv_last.Get(), structuredBuffer->velX_srv_current.Get(), structuredBuffer->velY_srv_current.Get()});
			dC->CSSetUAVS({ structuredBuffer->dens_uav_current.Get() });
			dC->CSSetShader(AdvectionCS);
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetSRVS({structuredBuffer->velX_srv_last.Get(), structuredBuffer->velX_srv_last.Get(), structuredBuffer->velY_srv_last.Get()});
			dC->CSSetUAVS({ structuredBuffer->velX_uav_current.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetSRVS({ structuredBuffer->velY_srv_last.Get(), structuredBuffer->velX_srv_last.Get(), structuredBuffer->velY_srv_last.Get() });
			dC->CSSetUAVS({ structuredBuffer->velY_uav_current.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
		}
		void GraphicsAddSource() {
			UINT xPos = 0.5f * resolution;
			UINT yPos = 0.1f * resolution;
			float ranVelocityX = std::rand() % 20;
			GraphicsAddToSmoke(Vector3D(xPos, yPos, 0), 0.05f, darkAmp, -10 + ranVelocityX, eVY);
		}
		void GraphicsAddToSmoke(Vector3D position, float radius, float dens, float xDir = 0, float yDir = 0) {
			smokeBuffDesc.emissionPoint = vec2_int{ (int)position.x, (int)position.y };
			smokeBuffDesc.emissionRadius = radius * resolution;
			SWAP(structuredBuffer->dens_uav_current, structuredBuffer->dens_uav_last);
			SWAP(structuredBuffer->dens_srv_current, structuredBuffer->dens_srv_last);
			SWAP(structuredBuffer->velX_uav_current, structuredBuffer->velX_uav_last);
			SWAP(structuredBuffer->velX_srv_current, structuredBuffer->velX_srv_last);
			SWAP(structuredBuffer->velY_uav_current, structuredBuffer->velY_uav_last);
			SWAP(structuredBuffer->velY_srv_current, structuredBuffer->velY_srv_last);

			smokeBuffDesc.emissionPoint = vec2_int{(int)position.x, (int)position.y};
			smokeBuffDesc.emissionRadius = radius * resolution;
			smokeBuffDesc.max = 1.0f;
			smokeBuffDesc.min = 0.0f;
			smokeBuffDesc.emission = dens;
			
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->CSSetShader(SourceCS);
			dC->CSSetSRVS({ structuredBuffer->dens_srv_last.Get() });
			dC->CSSetUAVS({ structuredBuffer->dens_uav_current.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			
			dC->CSSetSRVS({structuredBuffer->velX_srv_last.Get()});
			dC->CSSetUAVS({ structuredBuffer->velX_uav_current.Get() });
			smokeBuffDesc.max = maxSpeed;
			smokeBuffDesc.min = -maxSpeed;
			smokeBuffDesc.emission = xDir;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetSRVS({ structuredBuffer->velY_srv_last.Get() });
			dC->CSSetUAVS({ structuredBuffer->velY_uav_current.Get() });
			smokeBuffDesc.emission = yDir;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
		}
		void Clear() {
			dC->CSSetSRVS({ nullptr });
			dC->CSSetUAVS({ nullptr });
		}
		void GraphicsDiffuse() {
			dC->CopyResource(structuredBuffer->dens_start.Get(), structuredBuffer->dens_current.Get());
			dC->CopyResource(structuredBuffer->velX_start.Get(), structuredBuffer->velX_current.Get());
			dC->CopyResource(structuredBuffer->velY_start.Get(), structuredBuffer->velY_current.Get());
			int adaptiveIterations = 20;
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(structuredBuffer->dens_uav_current, structuredBuffer->dens_uav_last);
				SWAP(structuredBuffer->dens_srv_current, structuredBuffer->dens_srv_last);
				dC->CSSetShader(DiffusionCS);
				dC->CSSetSRVS({ structuredBuffer->dens_srv_last.Get(), structuredBuffer->dens_srv_start.Get() });
				dC->CSSetUAVS({ structuredBuffer->dens_uav_current.Get() });
				dC->Dispatch(groupCount, groupCount, 1);
				Clear();
				GraphicsSetBnd(0, structuredBuffer->dens_srv_current, structuredBuffer->dens_uav_current);
			}
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(structuredBuffer->velX_uav_current, structuredBuffer->velX_uav_last);
				SWAP(structuredBuffer->velX_srv_current, structuredBuffer->velX_srv_last);
				dC->CSSetSRVS({ structuredBuffer->velX_srv_last.Get(), structuredBuffer->velX_srv_start.Get() });
				dC->CSSetUAVS({ structuredBuffer->velX_uav_current.Get() });
				dC->CSSetShader(DiffusionCS);
				dC->Dispatch(groupCount, groupCount, 1);
				Clear();
			GraphicsSetBnd(1, structuredBuffer->velX_srv_current, structuredBuffer->velX_uav_current);
			}
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(structuredBuffer->velY_uav_current, structuredBuffer->velY_uav_last);
				SWAP(structuredBuffer->velY_srv_current, structuredBuffer->velY_srv_last);
				dC->CSSetShader(DiffusionCS);
				dC->CSSetSRVS({ structuredBuffer->velY_srv_last.Get() , structuredBuffer->velY_srv_start.Get()});
				dC->CSSetUAVS({ structuredBuffer->velY_uav_current.Get() });
				dC->Dispatch(groupCount, groupCount, 1);
				Clear();
				GraphicsSetBnd(2, structuredBuffer->velY_srv_current, structuredBuffer->velY_uav_current);
			}
		}
		void GraphicsSetBnd(int b, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav) {
			smokeBuffDesc.b = b;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->CSSetShader(BoundaryCS);
			dC->CSSetSRVS({ srv.Get() });
			dC->CSSetUAVS({ uav.Get() });
			dC->Dispatch(borderCount, 1, 1);
			Clear();
		}
		void GraphicsProject() {
			dC->CSSetShader(DivergenceCS);
			dC->CSSetSRVS({ structuredBuffer->velX_srv_current.Get(), structuredBuffer->velY_srv_current.Get() });
			dC->CSSetUAVS({ structuredBuffer->div_uav.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			//GraphicsSetBnd(0, structuredBuffer->div_srv, structuredBuffer->div_uav);
			dC->CSSetShader(ClearCS);
			dC->CSSetUAVS({ structuredBuffer->pressure_uav_current.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			//GraphicsSetBnd(0, structuredBuffer->pressure_srv_current, structuredBuffer->pressure_uav_current);
			int adaptiveIterations = 10;
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(structuredBuffer->pressure_uav_current, structuredBuffer->pressure_uav_last);
				SWAP(structuredBuffer->pressure_srv_current, structuredBuffer->pressure_srv_last);
				dC->CSSetShader(PressureCS);
				dC->CSSetSRVS({ structuredBuffer->pressure_srv_last.Get(), structuredBuffer->div_srv.Get() });
				dC->CSSetUAVS({ structuredBuffer->pressure_uav_current.Get() });
				dC->Dispatch(groupCount, groupCount, 1);
				Clear();
				GraphicsSetBnd(0, structuredBuffer->pressure_srv_current, structuredBuffer->pressure_uav_current);
			}
			SWAP(structuredBuffer->velX_srv_current, structuredBuffer->velX_srv_last);
			SWAP(structuredBuffer->velX_uav_current, structuredBuffer->velX_uav_last);
			SWAP(structuredBuffer->velY_srv_current, structuredBuffer->velY_srv_last);
			SWAP(structuredBuffer->velY_uav_current, structuredBuffer->velY_uav_last);
			SWAP(structuredBuffer->pressure_srv_current, structuredBuffer->pressure_srv_last);
			SWAP(structuredBuffer->pressure_uav_current, structuredBuffer->pressure_uav_last);
			dC->CSSetShader(ProjectionXCS);
			dC->CSSetSRVS({ structuredBuffer->velX_srv_last.Get(), structuredBuffer->pressure_srv_last.Get() });
			dC->CSSetUAVS({ structuredBuffer->velX_uav_current.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetShader(ProjectionYCS);
			dC->CSSetSRVS({ structuredBuffer->velY_srv_last.Get(), structuredBuffer->pressure_srv_last.Get() });
			dC->CSSetUAVS({ structuredBuffer->velY_uav_current.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			GraphicsSetBnd(1, structuredBuffer->velX_srv_current, structuredBuffer->velX_uav_current);
			GraphicsSetBnd(2, structuredBuffer->velY_srv_current, structuredBuffer->velY_uav_current);
		}
		void CPUStart() {
			UINT size = resolution + 2;
			vX.resize(size * size, 0.0f);
			vY.resize(size * size, 0.0f);
			densities.resize(size * size, 0.0f);
			oldvX.resize(size * size, 0.0f);
			oldvY.resize(size * size, 0.0f);
			oldDensities.resize(size * size, 0.0f);
			pressure.resize(size * size, 0.0f);
			div.resize(size * size, 0.0f);
			colors.resize(resolution * resolution);
			std::vector<vec4> baseColors(resolution * resolution, vec4{ 0,0,0,0 });
			colors = baseColors;
		}
		void SmokeUpdate() {
			AdvectUpdate();
			AddSource();
			VelUpdate();
			DensUpdate();
			TextureUpdate();
		}
		void TextureUpdate() {
			for (int y = 1; y <= resolution; y++) {
				for (int x = 1; x <= resolution; x++) {
					colors.at((x-1) + (y-1) * resolution).a = densities.at(IX(x,y));
				}
			}
			
			GetComponent<Material>()->textures.at(0)->MapToTexture(colors, resolution);
		}
		void AdvectUpdate() {
			SWAP(oldDensities, densities);
			Advect(densities.data(), oldDensities.data(), vX.data(), vY.data(), 0);
			SWAP(oldvX, vX);
			SWAP(oldvY, vY);
			Advect(vX.data(), oldvX.data(), oldvX.data(), oldvY.data(), 1);
			Advect(vY.data(), oldvY.data(), oldvX.data(), oldvY.data(), 2);
		}
		void AddSource() {
			UINT xPos = 0.5f * resolution;
			UINT yPos = 0.1f * resolution;
			float ranVelocityX = std::rand() % 20;
			AddToSmoke(Vector3D(xPos, yPos, 0), 0.05f, vec4{ 0,0,0,1 }, -10 + ranVelocityX, eVY);
		}
		void VelUpdate() {
			SWAP(oldvX, vX);
			SWAP(oldvY, vY);
			Diffuse(vX.data(), oldvX.data(), visc, 1);
			Diffuse(vY.data(), oldvY.data(), visc, 2);
			Project(vX.data(), vY.data(), pressure.data(), div.data());
		}
		void DensUpdate() {
			SWAP(oldDensities, densities);
			Diffuse(densities.data(), oldDensities.data(), diff, 0);
		}
		void AddToSmoke(Vector3D position, float radius, vec4 color, float xDir = 0, float yDir = 0) {
			int arrayRad = radius * resolution;
			int leftSide = std::clamp((int)position.x - arrayRad+1, 1, resolution);
			int rightSide = std::clamp((int)position.x + arrayRad+1, 1, resolution);
			int topSide = std::clamp((int)position.y + arrayRad + 1, 1, resolution);
			int bottomSide = std::clamp((int)position.y - arrayRad + 1, 1, resolution);
			float distance;
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
			AddToSmoke(Vector3D(coordX, coordY, 0), radius, vec4{ 0,0,0,1 });
		}
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
		void Diffuse(float* newData, float* oldData, float diff, int b) {
			float a = resolution * resolution * diff * Time::deltaTime;
			int adaptiveIterations = 20;
			for (int k = 0; k < adaptiveIterations; k++) {
				for (int y = 1; y <= resolution; y++) {
					for (int x = 1; x <= resolution; x++) {
						newData[IX(x, y)] = (oldData[IX(x, y)] + a * (newData[IX(x - 1, y)] + newData[IX(x + 1, y)] + newData[IX(x, y + 1)] + newData[IX(x, y - 1)])) / (4 * a + 1);
					}
				}
				set_bnd(b, newData);
			}
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
			int adaptiveIterations = 20;
			for (int k = 0; k < adaptiveIterations; k++) {
				for (UINT y = 1; y <= resolution; y++) {
					for (UINT x = 1; x <= resolution; x++) {
						pressure[IX(x, y)] = (div[IX(x, y)] + pressure[IX(x + 1, y)] + pressure[IX(x - 1, y)] + pressure[IX(x, y + 1)] + pressure[IX(x, y - 1)]) / 4.0f;
					}
				}
				set_bnd(0, pressure);
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
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
		std::unique_ptr<BufferPair> dens_read;
		std::unique_ptr<BufferPair> dens_write;
		std::unique_ptr<BufferPair> velX_read;
		std::unique_ptr<BufferPair> velX_write;
		std::unique_ptr<BufferPair> velY_read;
		std::unique_ptr<BufferPair> velY_write;
		std::unique_ptr<BufferPair> color;
		std::unique_ptr<BufferPair> pressure_read;
		std::unique_ptr<BufferPair> pressure_write;
		std::unique_ptr<BufferPair> divergence;
		std::unique_ptr<BufferPair> tempBr;
		std::unique_ptr<BufferPair> temp;
		Microsoft::WRL::ComPtr<ID3D11Buffer> colorStaging;
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
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> TransferCS;
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
		const WCHAR* transferCP = L"DX3D/Shaders/Smoke/Compute/Transfer.hlsl";
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

			rS->compileComputeShader(transferCP, blob);
			rS->createComputeShader(blob, TransferCS);
			//loading buffers;
			UINT width = resolution + 2;
			UINT size = width * width;
			UINT smSize = resolution * resolution;
			UINT brSize = resolution * 4 + 4;
			dens_read = structuredBuffer->CreateBufferPair<float>(size);
			dens_write = structuredBuffer->CreateBufferPair<float>(size);

			velX_read = structuredBuffer->CreateBufferPair<float>(size);
			velX_write = structuredBuffer->CreateBufferPair<float>(size);

			velY_read = structuredBuffer->CreateBufferPair<float>(size);
			velY_write = structuredBuffer->CreateBufferPair<float>(size);

			pressure_read = structuredBuffer->CreateBufferPair<float>(size);
			pressure_write = structuredBuffer->CreateBufferPair<float>(size);
			divergence = structuredBuffer->CreateBufferPair<float>(size);
			temp = structuredBuffer->CreateBufferPair<float>(size);
			tempBr = structuredBuffer->CreateBufferPair<float>(brSize);
			color = structuredBuffer->CreateBufferPair<vec4>(smSize);
			colorStaging = structuredBuffer->CreateStagingBuffer<vec4>(smSize);
			
			groupCount = (resolution + 9) / 10;
			borderCount = (4 * resolution + 13) / 10;

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
			GraphicsAddSource();
			GraphicsDiffuse();
			GraphicsProject();
			Finalize();
		}
		void Finalize() {
			dC->CSSetShader(MapCS);
			dC->CSSetSRVS({dens_write->srv.Get()});
			dC->CSSetUAVS({ color->uav.Get()});
			dC->Dispatch(groupCount, groupCount, 1);
			dC->CopyResource(colorStaging.Get(), color->buffer.Get());
			D3D11_MAPPED_SUBRESOURCE map{};
			map = dC->GetReadableMap(colorStaging);
			vec4* destPtr = reinterpret_cast<vec4*>(map.pData);
			std::vector<vec4> colorData(resolution * resolution);
			std::copy(destPtr, destPtr + (resolution * resolution), colorData.begin());
			GetComponent<Material>()->textures.at(0)->MapToTexture(colorData, resolution);
			dC->UnMap(colorStaging);
		}
		void GraphicsAdvect() {
			SWAP(dens_read, dens_write);
			SWAP(velX_read, velX_write);
			SWAP(velY_read, velY_write);
			dC->CSSetSRVS({ dens_read->srv.Get(), velX_read->srv.Get(), velY_read->srv.Get()});
			dC->CSSetUAVS({dens_write->uav.Get() });
			dC->CSSetShader(AdvectionCS);
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetSRVS({ velX_read->srv.Get(), velX_read->srv.Get(), velY_read->srv.Get() });
			dC->CSSetUAVS({ velX_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetSRVS({ velY_read->srv.Get(), velX_read->srv.Get(), velY_read->srv.Get() });
			dC->CSSetUAVS({ velY_write->uav.Get() });
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
			SWAP(dens_read, dens_write);
			SWAP(velX_read, velX_write);
			SWAP(velY_read, velY_write);

			smokeBuffDesc.emissionPoint = vec2_int{(int)position.x, (int)position.y};
			smokeBuffDesc.emissionRadius = radius * resolution;
			smokeBuffDesc.max = 1.0f;
			smokeBuffDesc.min = 0.0f;
			smokeBuffDesc.emission = dens;
			
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->CSSetShader(SourceCS);
			dC->CSSetSRVS({ dens_read->srv.Get()});
			dC->CSSetUAVS({ dens_write->uav.Get()});
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			
			dC->CSSetSRVS({velX_read->srv.Get()});
			dC->CSSetUAVS({ velX_write->uav.Get()});
			smokeBuffDesc.max = maxSpeed;
			smokeBuffDesc.min = -maxSpeed;
			smokeBuffDesc.emission = xDir;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetSRVS({ velY_read->srv.Get()});
			dC->CSSetUAVS({velY_write->uav.Get()});
			smokeBuffDesc.emission = yDir;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
		}
		void Clear() {
			dC->CSSetSRVS({ nullptr, nullptr, nullptr });
			dC->CSSetUAVS({ nullptr });
		}
		void GraphicsDiffuse() {
			dC->CopyResource(temp->buffer.Get(), dens_write->buffer.Get());
			int adaptiveIterations = 20;
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(dens_read, dens_write);
				dC->CSSetShader(DiffusionCS);
				dC->CSSetSRVS({ dens_read->srv.Get(), temp->srv.Get()});
				dC->CSSetUAVS({ dens_write->uav.Get()});
				dC->Dispatch(groupCount, groupCount, 1);
				Clear();
				GraphicsSetBnd(0, dens_write.get());
			}
			dC->CopyResource(temp->buffer.Get(), velX_write->buffer.Get());
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(velX_read, velX_write);
				dC->CSSetSRVS({ velX_read->srv.Get(), temp->srv.Get()});
				dC->CSSetUAVS({ velX_write->uav.Get()});
				dC->CSSetShader(DiffusionCS);
				dC->Dispatch(groupCount, groupCount, 1);
				Clear();
				GraphicsSetBnd(1, velX_write.get());
			}
			dC->CopyResource(temp->buffer.Get(), velY_write->buffer.Get());
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(velY_read, velY_write);
				dC->CSSetShader(DiffusionCS);
				dC->CSSetSRVS({ velY_read->srv.Get(), temp->srv.Get()});
				dC->CSSetUAVS({ velY_write->uav.Get()});
				dC->Dispatch(groupCount, groupCount, 1);
				Clear();
				GraphicsSetBnd(2, velY_write.get());
			}
		}
		void GraphicsSetBnd(int b, BufferPair* pair) {
			smokeBuffDesc.b = b;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->CSSetShader(TransferCS);
			dC->CSSetSRVS({ pair->srv.Get() });
			dC->CSSetUAVS({ tempBr->uav.Get() });
			dC->Dispatch(borderCount, 1, 1);
			Clear();
			dC->CSSetShader(BoundaryCS);
			dC->CSSetSRVS({ tempBr->srv.Get()});
			dC->CSSetUAVS({ pair->uav.Get() });
			dC->Dispatch(borderCount, 1, 1);
			Clear();
		}
		void GraphicsProject() {
			SWAP(velX_read, velX_write);
			SWAP(velY_read, velY_write);
			dC->CSSetShader(DivergenceCS);
			dC->CSSetSRVS({ velX_read->srv.Get(), velY_read->srv.Get()});
			dC->CSSetUAVS({ divergence->uav.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetShader(ClearCS);
			dC->CSSetUAVS({ pressure_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			GraphicsSetBnd(0, divergence.get());
			GraphicsSetBnd(0, pressure_write.get());
			int adaptiveIterations = 20;
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(pressure_read, pressure_write);
				dC->CSSetShader(PressureCS);
				dC->CSSetSRVS({pressure_read->srv.Get(), divergence->srv.Get() });
				dC->CSSetUAVS({ pressure_write->uav.Get() });
				dC->Dispatch(groupCount, groupCount, 1);
				Clear();
				GraphicsSetBnd(0, pressure_write.get());
			}
			dC->CSSetShader(ProjectionXCS);
			dC->CSSetSRVS({ velX_read->srv.Get(), pressure_write->srv.Get()});
			dC->CSSetUAVS({ velX_write->uav.Get()});
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetShader(ProjectionYCS);
			dC->CSSetSRVS({ velY_read->srv.Get(), pressure_write->srv.Get() });
			dC->CSSetUAVS({ velY_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			GraphicsSetBnd(1, velX_write.get());
			GraphicsSetBnd(2, velY_write.get());
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
			Diffuse();
			VelUpdate();
			TextureUpdate();
		}
		void Diffuse() {
			SWAP(oldvX, vX);
			SWAP(oldvY, vY);
			Diffuse(vX.data(), oldvX.data(), visc, 1);
			Diffuse(vY.data(), oldvY.data(), visc, 2);
			SWAP(oldDensities, densities);
			Diffuse(densities.data(), oldDensities.data(), diff, 0);
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
			Project(vX.data(), vY.data(), pressure.data(), div.data());
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
			//AddToSmoke(Vector3D(coordX, coordY, 0), radius, vec4{ 0,0,0,1 });
			GraphicsAddToSmoke(Vector3D(coordX, coordY, 0), radius, darkAmp, 0, 0);
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
			int adaptiveIterations = 20;
			for (int k = 0; k < adaptiveIterations; k++) {
				for (UINT y = 1; y <= resolution; y++) {
					for (UINT x = 1; x <= resolution; x++) {
						pressure[IX(x, y)] = (div[IX(x, y)] + pressure[IX(x + 1, y)] + pressure[IX(x - 1, y)] + pressure[IX(x, y + 1)] + pressure[IX(x, y - 1)]) / 4.0f;
					}
				}
			}
			set_bnd(0, pressure);
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
			x[IX(0, 0)] = b != 0? -x[IX(1, 1)] : x[IX(1,1)];
			x[IX(0, resolution + 1)] = b != 0? -x[IX(1, resolution)] : x[IX(1,resolution)];
			x[IX(resolution + 1, 0)] = b != 0 ? -x[IX(resolution, 1)] : x[IX(resolution, 1)];
			x[IX(resolution + 1, resolution + 1)] = b != 0 ? -x[IX(resolution, resolution)] : x[IX(resolution, resolution)];
		}

	};
}
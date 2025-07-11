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
	std::shared_ptr<Texture> Make3DFTex(int resolution) {
		std::shared_ptr<Texture> tex = GraphicsEngine::get()->getTextureManager()->createTexture();
		tex->Load3DFTex(resolution);
		return tex;
	}
	struct smoke3DConstantBufferDesc {
		int resolution;
		float dt;
		float diff;
		float visc;
		vec3_int emissionPoint;
		int emissionRadius;
		float emission;
		float max = 1.0f;
		float min = 0.0f;
		int b = 0;
	};
	class Smoke3D : public GameObject {
	public:
		//shared variables
		DeviceContext* dC{};
		RenderSystem* rS{};
		TextureManager* tM;
		float darkAmp = 10.0f;
		float eVX = 0.0f;
		float eVY = 10.0f;
		float eVZ = 0.0f;
		float visc = 0.0001f;
		float diff = 0.001f;
		float radius = 0.1f;
		float maxSpeed = 5.0f;
		int resolution;
		//shared methods
		std::shared_ptr<Texture> dens_read;
		std::shared_ptr<Texture> dens_write;
		std::shared_ptr<Texture> velX_read;
		std::shared_ptr<Texture> velX_write;
		std::shared_ptr<Texture> velY_read;
		std::shared_ptr<Texture> velY_write;
		std::shared_ptr<Texture> velZ_read;
		std::shared_ptr<Texture> velZ_write;
		std::shared_ptr<Texture> pressure_read;
		std::shared_ptr<Texture> pressure_write;
		std::shared_ptr<Texture> divergence;
		std::shared_ptr<BufferPair> tempBr;
		std::shared_ptr<Texture> temp;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> SourceCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> AdvectionCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> DiffusionCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> DivergenceCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> PressureCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> BoundaryCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> ClearCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> ProjectionCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> GMapCS;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> TransferCS;
		const WCHAR* sourceCP = L"DX3D/Shaders/Smoke3D/Compute/Source.hlsl";
		const WCHAR* advectionCP = L"DX3D/Shaders/Smoke3D/Compute/Advection.hlsl";
		const WCHAR* diffusionCP = L"DX3D/Shaders/Smoke3D/Compute/Diffuse.hlsl";
		const WCHAR* divergenceCP = L"DX3D/Shaders/Smoke/Compute/Divergence.hlsl";
		const WCHAR* pressureCP = L"DX3D/Shaders/Smoke3D/Compute/Pressure.hlsl";
		const WCHAR* clearCP = L"DX3D/Shaders/Smoke3D/Compute/Clear.hlsl";
		const WCHAR* boundaryCP = L"DX3D/Shaders/Smoke3D/Compute/Boundary.hlsl";
		const WCHAR* projectionCP = L"DX3D/Shaders/Smoke3D/Compute/Projection.hlsl";
		const WCHAR* gMapCP = L"DX3D/Shaders/Smoke3D/Compute/GPUMap.hlsl";
		const WCHAR* transferCP = L"DX3D/Shaders/Smoke3D/Compute/Transfer.hlsl";
		std::shared_ptr<StructuredBuffer> structuredBuffer;
		smoke3DConstantBufferDesc smokeBuffDesc{};
		std::shared_ptr<ConstantBuffer> constantBuffer;

		UINT groupCount;
		UINT borderCount;
		//GPU Methods
		Smoke3D(const BaseDesc& basedesc, int res) : GameObject(basedesc), resolution(res) {
			InputSystem::get()->addListener(this);
			//GPUStart();
		}
		void Update() override {
			//SmokeGraphicsUpdate();
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
			GraphicsAddToSmoke(Vector3D(coordX, coordY, 0), radius, darkAmp, 0, 0);
		}
		void GPUStart() {
			dC = DeviceContext::get();
			rS = RenderSystem::get();
			tM = GraphicsEngine::get()->getTextureManager();
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

			rS->compileComputeShader(projectionCP, blob);
			rS->createComputeShader(blob, ProjectionCS);

			rS->compileComputeShader(advectionCP, blob);
			rS->createComputeShader(blob, AdvectionCS);

			rS->compileComputeShader(mapCP, blob);
			rS->createComputeShader(blob, MapCS);

			rS->compileComputeShader(gMapCP, blob);
			rS->createComputeShader(blob, GMapCS);

			rS->compileComputeShader(transferCP, blob);
			rS->createComputeShader(blob, TransferCS);
			//loading buffers;
			UINT width = resolution + 2;
			UINT size = width * width * width;
			UINT smSize = resolution * resolution * resolution;
			UINT brSize = size - smSize;
			dens_read = Make3DFTex(width);
			dens_write = Make3DFTex(width);

			velX_read = Make3DFTex(width);
			velX_write = Make3DFTex(width);

			velY_read = Make3DFTex(width);
			velY_write = Make3DFTex(width);

			velZ_read = Make3DFTex(width);
			velZ_write = Make3DFTex(width);

			pressure_read = Make3DFTex(width);
			pressure_write = Make3DFTex(width);
			divergence = Make3DFTex(width);
			temp = Make3DFTex(width);
			tempBr = structuredBuffer->CreateBufferPair<float>(brSize);

			groupCount = (resolution + 9) / 10;
			borderCount = (brSize + 9) / 10;

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
			GPUFinalize();
		}
		void GPUFinalize() {
			dC->CSSetShader(GMapCS);
			dC->CSSetSRVS({ dens_write->srv.Get() });
			dC->CSSetUAVS({ GetComponent<Material>()->textures.at(0)->m_uav.Get() });
			dC->Dispatch(groupCount, groupCount, groupCount);
			Clear();
		}
		void GraphicsAdvect() {
			SWAP(dens_read, dens_write);
			SWAP(velX_read, velX_write);
			SWAP(velY_read, velY_write);
			SWAP(velZ_read, velZ_write);
			dC->CSSetSRVS({ dens_read->srv.Get(), velX_read->srv.Get(), velY_read->srv.Get(), velZ_read->srv.Get() });
			dC->CSSetUAVS({ dens_write->uav.Get() });
			dC->CSSetShader(AdvectionCS);
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetSRVS({ velX_read->srv.Get(), velX_read->srv.Get(), velY_read->srv.Get(), velZ_read->srv.Get() });
			dC->CSSetUAVS({ velX_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetSRVS({ velY_read->srv.Get(), velX_read->srv.Get(), velY_read->srv.Get(), velZ_read->srv.Get() });
			dC->CSSetUAVS({ velY_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
			dC->CSSetSRVS({ velZ_read->srv.Get(), velX_read->srv.Get(), velY_read->srv.Get(), velZ_read->srv.Get() });
			dC->CSSetUAVS({ velZ_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, 1);
			Clear();
		}
		void GraphicsAddSource() {
			UINT xPos = 0.5f * resolution;
			UINT yPos = 0.1f * resolution;
			float ranVelocityX = std::rand() % 20;
			GraphicsAddToSmoke(Vector3D(xPos, yPos, 0), 0.05f, darkAmp, -10 + ranVelocityX, eVY);
		}
		void GraphicsAddToSmoke(Vector3D position, float radius, float dens, float xDir = 0, float yDir = 0, float zDir) {
			SWAP(dens_read, dens_write);
			SWAP(velX_read, velX_write);
			SWAP(velY_read, velY_write);
			SWAP(velZ_read, velZ_write);
			smokeBuffDesc.emissionPoint = vec3_int{ (int)position.x, (int)position.y, (int)position.z };
			smokeBuffDesc.emissionRadius = radius * resolution;
			smokeBuffDesc.max = 1.0f;
			smokeBuffDesc.min = 0.0f;
			smokeBuffDesc.emission = dens;

			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->CSSetShader(SourceCS);
			dC->CSSetSRVS({ dens_read->srv.Get() });
			dC->CSSetUAVS({ dens_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, groupCount);
			Clear();

			dC->CSSetSRVS({ velX_read->srv.Get() });
			dC->CSSetUAVS({ velX_write->uav.Get() });
			smokeBuffDesc.max = maxSpeed;
			smokeBuffDesc.min = -maxSpeed;
			smokeBuffDesc.emission = xDir;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->Dispatch(groupCount, groupCount, groupCount);
			Clear();
			dC->CSSetSRVS({ velY_read->srv.Get() });
			dC->CSSetUAVS({ velY_write->uav.Get() });
			smokeBuffDesc.emission = yDir;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->Dispatch(groupCount, groupCount, groupCount);
			Clear();
			dC->CSSetSRVS({ velZ_read->srv.Get() });
			dC->CSSetUAVS({ velZ_write->uav.Get() });
			smokeBuffDesc.emission = yDir;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->Dispatch(groupCount, groupCount, groupCount);
			Clear();
		}
		void Clear() {
			dC->CSSetSRVS({ nullptr, nullptr, nullptr });
			dC->CSSetUAVS({ nullptr });
		}
		void GraphicsDiffuse() {
			dC->CopyResource(temp->texture.Get(), dens_write->texture.Get());
			int adaptiveIterations = 20;
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(dens_read, dens_write);
				dC->CSSetShader(DiffusionCS);
				dC->CSSetSRVS({ dens_read->srv.Get(), temp->srv.Get() });
				dC->CSSetUAVS({ dens_write->uav.Get() });
				dC->Dispatch(groupCount, groupCount, groupCount);
				Clear();
				GraphicsSetBnd(0, dens_write.get());
			}
			dC->CopyResource(temp->texture.Get(), velX_write->texture.Get());
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(velX_read, velX_write);
				dC->CSSetSRVS({ velX_read->srv.Get(), temp->srv.Get() });
				dC->CSSetUAVS({ velX_write->uav.Get() });
				dC->CSSetShader(DiffusionCS);
				dC->Dispatch(groupCount, groupCount, groupCount);
				Clear();
				GraphicsSetBnd(1, velX_write.get());
			}
			dC->CopyResource(temp->texture.Get(), velY_write->texture.Get());
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(velY_read, velY_write);
				dC->CSSetShader(DiffusionCS);
				dC->CSSetSRVS({ velY_read->srv.Get(), temp->srv.Get() });
				dC->CSSetUAVS({ velY_write->uav.Get() });
				dC->Dispatch(groupCount, groupCount, groupCount);
				Clear();
				GraphicsSetBnd(2, velY_write.get());
			}
			dC->CopyResource(temp->texture.Get(), velZ_write->texture.Get());
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(velZ_read, velZ_write);
				dC->CSSetShader(DiffusionCS);
				dC->CSSetSRVS({ velZ_read->srv.Get(), temp->srv.Get() });
				dC->CSSetUAVS({ velZ_write->uav.Get() });
				dC->Dispatch(groupCount, groupCount, groupCount);
				Clear();
				GraphicsSetBnd(3, velZ_write.get());
			}
		}
		void GraphicsSetBnd(int b, Texture* pair) {
			smokeBuffDesc.b = b;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->CSSetShader(TransferCS);
			dC->CSSetSRVS({ pair->srv.Get() });
			dC->CSSetUAVS({ tempBr->uav.Get() });
			dC->Dispatch(borderCount, 1, 1);
			Clear();
			dC->CSSetShader(BoundaryCS);
			dC->CSSetSRVS({ tempBr->srv.Get() });
			dC->CSSetUAVS({ pair->uav.Get() });
			dC->Dispatch(borderCount, 1, 1);
			Clear();
		}
		void GraphicsProject() {
			SWAP(velX_read, velX_write);
			SWAP(velY_read, velY_write);
			SWAP(velZ_read, velZ_write);
			dC->CSSetShader(DivergenceCS);
			dC->CSSetSRVS({ velX_read->srv.Get(), velY_read->srv.Get(), velZ_read->srv.Get()});
			dC->CSSetUAVS({ divergence->uav.Get() });
			dC->Dispatch(groupCount, groupCount, groupCount);
			Clear();
			dC->CSSetShader(ClearCS);
			dC->CSSetUAVS({ pressure_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, groupCount);
			Clear();
			GraphicsSetBnd(0, divergence.get());
			GraphicsSetBnd(0, pressure_write.get());
			int adaptiveIterations = 20;
			for (int k = 0; k < adaptiveIterations; k++) {
				SWAP(pressure_read, pressure_write);
				dC->CSSetShader(PressureCS);
				dC->CSSetSRVS({ pressure_read->srv.Get(), divergence->srv.Get() });
				dC->CSSetUAVS({ pressure_write->uav.Get() });
				dC->Dispatch(groupCount, groupCount, groupCount);
				Clear();
				GraphicsSetBnd(0, pressure_write.get());
			}
			smokeBuffDesc.b = 1;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->CSSetShader(ProjectionCS);
			dC->CSSetSRVS({ velX_read->srv.Get(), pressure_write->srv.Get() });
			dC->CSSetUAVS({ velX_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, groupCount);
			Clear();
			smokeBuffDesc.b = 2;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->CSSetShader(ProjectionCS);
			dC->CSSetSRVS({ velY_read->srv.Get(), pressure_write->srv.Get() });
			dC->CSSetUAVS({ velY_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, groupCount);
			Clear();
			smokeBuffDesc.b = 3;
			constantBuffer->UpdateSubresource(&smokeBuffDesc);
			dC->CSSetShader(ProjectionCS);
			dC->CSSetSRVS({ velZ_read->srv.Get(), pressure_write->srv.Get() });
			dC->CSSetUAVS({ velZ_write->uav.Get() });
			dC->Dispatch(groupCount, groupCount, groupCount);
			Clear();
			GraphicsSetBnd(1, velX_write.get());
			GraphicsSetBnd(2, velY_write.get());
			GraphicsSetBnd(3, velZ_write.get());
		}
	};
}
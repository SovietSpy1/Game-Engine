#pragma once
#include <DX3D/Core/Base.h>
namespace dx3d {
	class GraphicsEngine : public Base
	{
	public:
		explicit GraphicsEngine(const GraphicsEngineDesc& desc);
		virtual ~GraphicsEngine() override;
		static GraphicsEngine* get();
		RenderSystem& getRenderSystem() const noexcept;
		TextureManager& getTextureManager() const noexcept;
		MeshManager& getMeshManager() const noexcept;
		std::shared_ptr<GameObject> createGameObject() const;
	public:
		//publicly accesable functions
		std::shared_ptr<Mesh> createMesh() const;
	private:
		static GraphicsEngine* S;
		std::shared_ptr<RenderSystem> m_renderSystem{};
		std::shared_ptr<TextureManager> m_textureManager{};
		std::shared_ptr<MeshManager> m_meshManager{};
	};
}



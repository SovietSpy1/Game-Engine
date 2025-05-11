#pragma once
#include <DX3D/Window/Window.h>
#include <DX3D/Math/GameMath.h>
namespace dx3d {
	class Display final: public Window
	{
	public:
		explicit Display(const DisplayDesc& desc);
		virtual void onUpdate() override;
	private:
		SwapChainPtr m_swapChain{};
		std::shared_ptr<DeviceContext> m_device_context{};
		Color startCol;
		Color endCol;
		Color currentCol;
		float u{0};
	};
}



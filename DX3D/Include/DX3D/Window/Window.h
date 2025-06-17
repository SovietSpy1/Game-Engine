#pragma once  
#include <DX3D/Core/Base.h>  
#include <DX3D/Core/Common.h>  
namespace dx3d {  
	class Window : public Base  
	{  
	public:  
		explicit Window(const WindowDesc& desc);  
		virtual ~Window() override;  
		virtual void onFocus();  
		virtual void onKillFocus();  
		virtual void toggleFullScreen();
		virtual void onResize(const Rect& new_size);
		Rect m_size{};
	protected:  
		void* m_handle{};
		bool fullScreen;
		Rect m_fullScreenSize{};
		Rect m_windowedSize{};
	};  
}

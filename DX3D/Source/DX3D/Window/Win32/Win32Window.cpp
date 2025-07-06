#include <DX3D/Window/Window.h>
#include <Windows.h>
#include <stdexcept>
#include <DX3D/Game/Display.h>
#include <dwmapi.h>
static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}	
	case WM_SETFOCUS: 
	{
		dx3d::Window* window = (dx3d::Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		dx3d::Window* window = (dx3d::Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_SETCURSOR:
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}
dx3d::Window::Window(const WindowDesc& desc) : Base(desc.base), m_size(desc.size), m_windowedSize(desc.size), m_fullScreenSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)), fullScreen(false)
{
	if (m_size.width <= 0 || m_size.height <= 0) {
		DX3DLogErrorAndThrow("Window size is invalid.");
	}
	HICON hIcon = (HICON)LoadImage(NULL, L"Assets/favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
	auto registerWindowClassFunction = [&]() {
		WNDCLASSEX wc{};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpszClassName = L"DX3DWindow";
		wc.lpfnWndProc = &WindowProcedure;
		wc.hIcon = hIcon;
		return RegisterClassEx(&wc);
		};
	static const auto windowClassId = std::invoke(registerWindowClassFunction);
	if (!windowClassId) {
		DX3DLogErrorAndThrow("RegisterClassEx failed.");
	}
		
	RECT rc{ 0, 0, m_size.width, m_size.height};
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false);
	m_handle = CreateWindowEx(NULL, MAKEINTATOM(windowClassId), L"C++ Game Engine", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, NULL, NULL);
	if (!m_handle) {
		DX3DLogErrorAndThrow("CreateWindowEx failed.");
	}
	SetWindowLongPtr(static_cast<HWND>(m_handle), GWLP_USERDATA, (LONG_PTR)this);
	COLORREF captionColor = RGB(0, 0, 0);
	COLORREF textColor = RGB(255, 255, 255);
	HRESULT hrCaption = DwmSetWindowAttribute(static_cast<HWND>(m_handle), DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));
	if (FAILED(hrCaption)) {
		DX3DLogErrorAndThrow("Set Window Caption Color Failed!");
	}
	HRESULT hrText = DwmSetWindowAttribute(static_cast<HWND>(m_handle), DWMWA_TEXT_COLOR, &textColor, sizeof(textColor));
	if (FAILED(hrCaption)) {
		DX3DLogErrorAndThrow("Set Window Text Color Failed!");
	}

	ShowWindow(static_cast<HWND>(m_handle), SW_SHOW);
}
dx3d::Window::~Window()
{
	DestroyWindow(static_cast<HWND>(m_handle));
}
void dx3d::Window::onFocus()
{
}

void dx3d::Window::onKillFocus()
{
}


void dx3d::Window::toggleFullScreen()
{
	if (fullScreen) {
		fullScreen = false;
		m_size = m_windowedSize;
		SetWindowLong(static_cast<HWND>(m_handle), GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
		SetWindowPos(static_cast<HWND>(m_handle), HWND_TOP, 100, 100, m_size.width, m_size.height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
	else {
		fullScreen = true;
		m_size = m_fullScreenSize;
		SetWindowLong(static_cast<HWND>(m_handle), GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(static_cast<HWND>(m_handle), HWND_TOP, 0, 0, m_fullScreenSize.width, m_fullScreenSize.height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
	onResize(m_size);
}

void dx3d::Window::onResize(const Rect& new_size)
{
}

dx3d::Point dx3d::Window::GetClientPosition()
{
	RECT clientRect{};
	GetClientRect(static_cast<HWND>(m_handle), &clientRect);
	POINT clientTopLeft = { clientRect.left, clientRect.top };
	ClientToScreen(static_cast<HWND>(m_handle), &clientTopLeft);
	Point pos = { clientTopLeft.x, clientTopLeft.y };
	return pos;
}

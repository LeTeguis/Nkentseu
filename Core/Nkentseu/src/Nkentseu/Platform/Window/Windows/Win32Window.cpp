//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-12 at 05:16:22 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Win32Window.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include <Logger/Formatter.h>

#include "Nkentseu/Platform/Window/WindowData.h"
#include "Nkentseu/Platform/PlatformState.h"
#include "Nkentseu/Core/NkentseuLogger.h"
#include <Ntsm/Random.h>

#include "dwmapi.h"
#include <Dbt.h>

namespace nkentseu {
	uint64 Win32Window::s_WindowIDCounter = 0;
	const GUID guidDevinterfaceHid = { 0x4d1e55b2, 0xf16f, 0x11cf, {0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30} };

	Win32Window::Win32Window(const WindowProperties& properties) : m_WindowID(++s_WindowIDCounter), m_Properties(properties)
	{
		m_IsWindowCreated = CreateWin32Window();
	}

	std::string Win32Window::GetTitle() const
	{
		return m_Properties.title;
	}

	void Win32Window::SetTitle(std::string title)
	{
		if (IsValidDisplay()) {
            m_Properties.title = title;
            std::wstring wtitle(title.begin(), title.end());
            SetWindowText(m_Data->windowHandle, wtitle.c_str());
        }
	}

	Vector2i Win32Window::GetPosition() const
	{
		return m_Properties.position;
	}

	void Win32Window::SetPosition(int32 x, int32 y)
	{
		if (IsValidDisplay()) {
			SetWindowPos(m_Data->windowHandle, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			m_Properties.position = Vector2i(x, y);
		}
	}

	void Win32Window::SetPosition(const Vector2i& pos)
	{
		SetPosition(pos.x, pos.y);
	}

	Vector2u Win32Window::GetSize()
	{
		return m_Properties.size;
	}

	void Win32Window::SetSize(uint32 width, uint32 height)
	{
		if (IsValidDisplay()) {
			RECT rect, frame, border;
			GetWindowRect(m_Data->windowHandle, &rect);
			DwmGetWindowAttribute(m_Data->windowHandle, DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(RECT));

			border.left = frame.left - rect.left;
			border.top = frame.top - rect.top;
			border.right = rect.right - frame.right;
			border.bottom = rect.bottom - frame.bottom;

			int32 titlebarHeight = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER));

			SetWindowPos(m_Data->windowHandle, nullptr,
				-1, -1,
				width + border.right + border.left, // + m_WinFrame.w,
				height + border.top + border.bottom + titlebarHeight, // + m_WinFrame.height,
				SWP_NOMOVE | SWP_NOREDRAW);
			m_Properties.size = Vector2u(width, height);
		}
	}

	void Win32Window::SetSize(const Vector2u& size)
	{
		SetSize(size.width, size.height);
	}

	float32 Win32Window::GetDpiScale() const
	{
		if (IsValidDisplay()) {
			float32 currentDpi = static_cast<float32>(GetDpiForWindow(m_Data->windowHandle));
			float32 defaultDpi = USER_DEFAULT_SCREEN_DPI;

			return currentDpi / defaultDpi;
		}
		return 0.0f;
	}

	Vector2 Win32Window::ConvertPixelToDpi(const Vector2& pixel) const
	{
		Vector2 pixel_(pixel);
		return pixel_ * GetDpiScale();
	}

	float32 Win32Window::GetDpiAspect()
	{
		Vector2f size = ConvertPixelToDpi(m_Properties.size);
		return size.width / size.height;
	}

	float32 Win32Window::GetAspect()
	{
		return (float32)m_Properties.size.width / m_Properties.size.height;
	}

	Vector2u Win32Window::CurrentDisplaySize() const
	{
		if (IsValidDisplay()) {
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);
			return Vector2u(static_cast<uint32>(screenWidth), static_cast<uint32>(screenHeight));
		}
		return {};
	}

	Vector2i Win32Window::CurrentDisplayPosition() const
	{
		if (IsValidDisplay()) {
			WINDOWPLACEMENT lpwndpl = { 0 };
			GetWindowPlacement(m_Data->windowHandle, &lpwndpl);
			return Vector2i(lpwndpl.ptMinPosition.x, lpwndpl.ptMinPosition.y);
		}
		return Vector2i();
	}

	void Win32Window::ShowMouse(bool show)
	{
		if (IsValidDisplay()) {
			// ShowCursor(show ? TRUE : FALSE);
			m_Data->isCursorVisible = show;
			SetCursor(m_Data->isCursorVisible ? m_Data->lastCursor : nullptr);
		}
	}

	void Win32Window::SetMouseCursorGrabbed(bool grabbed)
	{
		if (IsValidDisplay()) {
			m_Data->isCursorGrabbed = grabbed;
			GrabWindowCursor(m_Data->isCursorGrabbed);
		}
	}

	void Win32Window::SetMouseCursor(const WindowCursor& cursor)
	{
		if (IsValidDisplay()) {
			m_Data->lastCursor = static_cast<HCURSOR>(cursor.GetCursor());
			SetCursor(m_Data->isCursorVisible ? m_Data->lastCursor : nullptr);
		}
	}

	bool Win32Window::IsMouseInside()
	{
		return m_Data == nullptr ? false : m_Data->isMouseInside;
	}

	Color Win32Window::GetBackgroundColor()
	{
		return m_Properties.backgroundColor;
	}

	void Win32Window::SetBackgroundColor(const Color& color)
	{
		if (m_Data != nullptr) {
			m_Properties.backgroundColor = color;
#if defined(NKENTSUU_GAPI_SOFTWARE)
			m_Data->backgroundColor.SetColor(color);
#endif
		}
	}

	void Win32Window::Minimize()
	{
		if (IsValidDisplay()) {
			ShowWindow(m_Data->windowHandle, SW_MINIMIZE);
		}
	}

	void Win32Window::Maximize()
	{
		if (IsValidDisplay()) {
			if (!IsZoomed(m_Data->windowHandle)) {
				ShowWindow(m_Data->windowHandle, SW_MAXIMIZE);
			}
			else {
				ShowWindow(m_Data->windowHandle, SW_RESTORE);
			}
		}
	}

	void Win32Window::Close()
	{
		if (IsValidDisplay()) {
			if (m_Data->icon)
				DestroyIcon(m_Data->icon);

			// m_Data->Unregister();

			DestroyWindow(m_Data->windowHandle);

			auto it = windowMap.find(m_Data->windowHandle);
			if (it != windowMap.end()) {
				windowMap.erase(it);
			}

			m_Data->windowHandle = nullptr;
			Memory::Reset(m_Data);
			m_IsWindowClosed = true;
			m_IsWindowCreated = false;
		}
	}

	bool Win32Window::IsClosed()
	{
		return m_IsWindowClosed;
	}

	bool Win32Window::IsOpen()
	{
		return !IsClosed();
	}

	void Win32Window::Show()
	{
		if (!IsValidDisplay() || m_Properties.visible) {
			return;
		}

		m_Properties.visible = true;

		ShowWindow(m_Data->windowHandle, SW_SHOW);
		SetForegroundWindow(m_Data->windowHandle);
		SetFocus(m_Data->windowHandle);
	}

	void Win32Window::Hide()
	{
		if (!IsValidDisplay() || !m_Properties.visible) {
			return;
		}

		m_Properties.visible = false;

		ShowWindow(m_Data->windowHandle, SW_HIDE);
		SetForegroundWindow(m_Data->windowHandle);
		SetFocus(m_Data->windowHandle);
	}

	void Win32Window::SetIcon(const Vector2u& size, const uint8* pixels)
	{
		if (IsValidDisplay()) {
			// First destroy the previous one
			if (m_Data->icon)
				DestroyIcon(m_Data->icon);

			// Windows wants BGRA pixels: swap red and blue channels
			std::vector<std::uint8_t> iconPixels(size.x * size.y * 4);
			for (std::size_t i = 0; i < iconPixels.size() / 4; ++i) {
				iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
				iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
				iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
				iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
			}

			// Create the icon from the pixel array
			m_Data->icon = CreateIcon(GetModuleHandleW(nullptr),
				static_cast<int>(size.x),
				static_cast<int>(size.y),
				1,
				32,
				nullptr,
				iconPixels.data());

			// Set it as both big and small icon of the window
			if (m_Data->icon) {
				SendMessageW(m_Data->windowHandle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(m_Data->icon));
				SendMessageW(m_Data->windowHandle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(m_Data->icon));
			}
			else {
				Log_nts.Error("Failed to set the window's icon");
			}
		}
	}

	const WindowProperties& Win32Window::GetProperties()
	{
		return m_Properties;
	}

	WindowProperties& Win32Window::GetProps()
	{
		return m_Properties;
	}

	uint64 Win32Window::ID()
	{
		return m_WindowID;
	}

	void Win32Window::Clear(const Color& color)
	{
		m_Properties.backgroundColor = color;
	}

	void Win32Window::Swapbuffer()
	{
#if defined(NKENTSEU_GAPI_SOFTWARE)
		if (m_Data == nullptr || m_Data->windowHandle == nullptr) {
			return;
		}
		InvalidateRect(m_Data->windowHandle, NULL, TRUE);
		UpdateWindow(m_Data->windowHandle);
#endif
	}

	void Win32Window::SetAlwaysOnTop(bool alwaysOnTop)
	{
		if (!IsValidDisplay()) {
			return;
		}
		if (alwaysOnTop) {
			SetWindowPos(m_Data->windowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		else {
			SetWindowPos(m_Data->windowHandle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
	}

	void Win32Window::AcceptDragAndDrop(bool accept)
	{
		if (!IsValidDisplay()) {
			return;
		}
		DragAcceptFiles(m_Data->windowHandle, accept ? TRUE : FALSE);
	}

	void Win32Window::SetOpacity(float32 opacity)
	{
		if (!IsValidDisplay()) {
			return;
		}
		const HWND hwnd = m_Data->windowHandle;
		const LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);

		if (opacity == 1.0f) {
			if (style & WS_EX_LAYERED) {
				if (SetWindowLong(hwnd, GWL_EXSTYLE, style & ~WS_EX_LAYERED) == 0) {
					//return WIN_SetError("SetWindowLong()");
					return;
				}
			}
		}
		else {
			const BYTE alpha = (BYTE)((int)(opacity * 255.0f));
			if ((style & WS_EX_LAYERED) == 0) {
				if (SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED) == 0) {
					//return WIN_SetError("SetWindowLong()");
					return;
				}
			}

			if (SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA) == 0) {
				//return WIN_SetError("SetLayeredWindowAttributes()");
				return;
			}
		}
	}

	WindowData* Win32Window::GetData()
	{
		return m_Data.get();
	}

	bool Win32Window::IsCreate()
	{
		return m_IsWindowCreated;
	}

	Win32Window* Win32Window::GetCurrent(HWND hwnd)
	{
		Win32Window* current;
		if (currentWindow != nullptr) {
			windowMap.emplace(hwnd, currentWindow);
			currentWindow->m_Data->windowHandle = hwnd;
			current = currentWindow;
			currentWindow = nullptr;
		}
		else {
			auto existing = windowMap.find(hwnd);
			current = existing->second;
		}
		return current;
	}

	void Win32Window::SetProgress(float32 progress)
	{
		if (m_Data == nullptr) {
			return;
		}
		if (IsValidDisplay() && m_Data->taskbarList != nullptr) {
			unsigned max = 10000;
			unsigned cur = (unsigned)(progress * (float32)max);
			m_Data->taskbarList->SetProgressValue(m_Data->windowHandle, cur, max);
		}
	}

	bool Win32Window::CreateWin32Window()
	{
		if (m_IsWindowCreated) return false;

		m_Data = Memory::Alloc<WindowData>();

		if (m_Data == nullptr) {
			ErrorMessaging.PushError(NTSErrorCode::Window_CreateNative);
			return false;
		}

		currentWindow = this;

		m_Data->instanceHandle = PlatformState.HInstance;
		HINSTANCE hPrevInstance = PlatformState.HPrevInstance;
		LPSTR lpCmdLine = PlatformState.LPCmdLine;
		int32 nCmdShow = PlatformState.NCmdShow;

		m_Data->backgroundColor.SetColor(m_Properties.backgroundColor);

		if (!m_Properties.fullscreen) {
			//InitWindowPosition(m_Properties.position, m_Properties.size, m_Properties.positionType);
		}

		Vector2i position = m_Properties.position;
		Vector2u size = m_Properties.size;// +m_WinFrame;

		if (!m_Data->Register(m_Properties.doubleClick, m_Properties)) {
			/**
				* Either an OS Error or a window with the same "name" id will cause
				* this to fail:
				*/
			ErrorMessaging.PushError(NTSErrorCode::Window_RegisterWindowClass);
			return false;
		}

		Vector2i screenSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

		if (m_Properties.fullscreen) {
			DEVMODE dmScreenSettings;
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = screenSize.width;
			dmScreenSettings.dmPelsHeight = screenSize.height;
			dmScreenSettings.dmBitsPerPel = m_Properties.bitsPerPixel;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if ((size.width != screenSize.width) && (size.height != screenSize.height)) {
				if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
					// Stay in Windowed mode
					ErrorMessaging.PushError(NTSErrorCode::Window_StayInWindowMode);
				}
			}
			m_Properties.size = screenSize;
			size = m_Properties.size;
			m_Properties.position = Vector2i();
		}
		DWORD dwStyle = GetWindowStyleInternal(m_Properties);
		DWORD dwExStyle = GetWindowExStyleInternal(m_Properties);

		// Store the current thread's DPI-awareness context
		DPI_AWARENESS_CONTEXT previousDpiContext = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		RECT windowRect = UpdateWindowExtension(position, size, dwStyle, dwExStyle);

		std::wstring title(m_Properties.title.begin(), m_Properties.title.end());

		m_Data->windowHandle = CreateWindowEx(0,
			m_Data->GetWindowClassName(), title.c_str(),
			dwStyle,
			windowRect.left, windowRect.top,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL, NULL,
			m_Data->instanceHandle, NULL);

		if (m_Data->windowHandle == nullptr) {
			// Failed to create window...
			ErrorMessaging.PushError(NTSErrorCode::Window_Create);
			return false;
		}

		GetCurrent(m_Data->windowHandle);

		if (!m_Properties.fullscreen) {
			InitWindowPosition(Vector2i(windowRect.left, windowRect.top), Vector2u(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top), m_Properties.positionType);
		}

		/*if (!m_NativeWindow->CreateHDC()) {
			ErrorMessaging.PushError(NTSErrorCode::Window_Context_Graphics);
			return;
		}*/

		BOOL isNCRenderingEnabled{ TRUE };
		DwmSetWindowAttribute(m_Data->windowHandle, DWMWA_NCRENDERING_ENABLED, &isNCRenderingEnabled, sizeof(isNCRenderingEnabled));

		if (m_Properties.visible) {
			ShowWindow(m_Data->windowHandle, SW_SHOW);
			SetForegroundWindow(m_Data->windowHandle);
			SetFocus(m_Data->windowHandle);
		}

		// Register to receive device interface change notifications (used for joystick connection handling)
		DEV_BROADCAST_DEVICEINTERFACE deviceInterface =
		{ sizeof(DEV_BROADCAST_DEVICEINTERFACE), DBT_DEVTYP_DEVICEINTERFACE, 0, guidDevinterfaceHid, {0} };
		RegisterDeviceNotification(m_Data->windowHandle, &deviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);

		static const DWM_BLURBEHIND blurBehind{ {0}, {TRUE}, {NULL}, {TRUE} };
		DwmEnableBlurBehindWindow(m_Data->windowHandle, &blurBehind);
		static const MARGINS shadow_state[2]{ {0, 0, 0, 0}, {1, 1, 1, 1} };
		DwmExtendFrameIntoClientArea(m_Data->windowHandle, &shadow_state[0]);

		RegisterWindowMessage(L"TaskbarButtonCreated");
		HRESULT hrf = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
			IID_ITaskbarList3, (LPVOID*)&m_Data->taskbarList);
		SetProgress(0.0f);
		m_IsWindowCreated = true;
		m_IsWindowClosed = false;

		UpdateWindow(m_Data->windowHandle);

		return m_IsWindowCreated;
	}

	bool Win32Window::IsValidDisplay() const
	{
		return m_Data != nullptr && m_Data->windowHandle != 0;
	}

	DWORD Win32Window::GetWindowStyleInternal(const WindowProperties& props)
	{
		DWORD style = 0;

		if (props.fullscreen) {
			style |= WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		}
		else {
			// Fenetre standard avec bordure (WS_OVERLAPPEDWINDOW)
			style |= (props.frame) ? WS_OVERLAPPEDWINDOW : WS_POPUP;

			if (props.frame) {
				// Supprime le bouton de fermeture
				style = (props.closable) ? style | WS_SYSMENU : style & (~WS_SYSMENU);

				// Supprime le bouton de minimisation
				style = (props.minimizable) ? style | WS_MINIMIZEBOX : style & (~WS_MINIMIZEBOX);

				// Supprime le bouton de maximisation
				style = (props.maximizable) ? style | WS_MAXIMIZEBOX : style & (~WS_MAXIMIZEBOX);
			}

			// Supprime le bouton de maximisation
			style = (props.movable) ? style | WS_POPUP : style;

			// Fenetre redimensionnable
			style = (props.resizable) ? style | WS_THICKFRAME : style & (~WS_THICKFRAME);

			style = (props.visible) ? style | WS_VISIBLE : style;

			style = (props.modal) ? style | WS_POPUP : style;
		}
		return style;
	}

	DWORD Win32Window::GetWindowExStyleInternal(const WindowProperties& props)
	{
		DWORD exStyle = 0;
		// Plein ecran
		if (props.fullscreen) {
			exStyle |= WS_EX_APPWINDOW;
		}
		else {
			// Ajouter des styles etendus supplementaires pour les fenetres non plein ecran si necessaire
			// Par exemple, WS_EX_CLIENTEDGE, WS_EX_STATICEDGE, WS_EX_DLGMODALFRAME etc.
			exStyle |= WS_EX_APPWINDOW | WS_EX_STATICEDGE;

			// Bordure et style etendu de base
			exStyle |= (props.frame) ? WS_EX_WINDOWEDGE : WS_EX_TOOLWINDOW;

			// Transparence
			exStyle |= (props.transparent) ? WS_EX_TRANSPARENT : 0;

			// Effets visuels
			exStyle |= (props.hasShadow) ? WS_EX_LAYERED : 0;

			// Supprime le bouton de maximisation
			exStyle |= (props.movable) ? WS_EX_TOOLWINDOW : 0;

			exStyle |= (props.acceptFile) ? WS_EX_ACCEPTFILES : 0;

			exStyle |= (props.stayInTop) ? WS_EX_TOPMOST : 0;

		}
		return exStyle;
	}

	RECT Win32Window::UpdateWindowExtension(const Vector2i& position, const Vector2u& size, DWORD style, DWORD styleEx)
	{
		RECT windowRect;
		windowRect.left = position.x;
		windowRect.top = position.y;
		windowRect.right = position.x + (long)size.width;
		windowRect.bottom = position.y + (long)size.height;

		AdjustWindowRectEx(&windowRect, style, FALSE, styleEx);

		if (windowRect.left < 0) {
			windowRect.right = windowRect.right - windowRect.left;
			windowRect.left = 0;
		}
		if (windowRect.top < 0) {
			windowRect.bottom = windowRect.bottom - windowRect.top;
			windowRect.top = 0;
		}
		return windowRect;
	}

	void Win32Window::InitWindowPosition(const Vector2i& position, const Vector2u& size, WindowPositionType positionType)
	{
		// Adjust size to match DPI
		float32 scaleFactor = GetDpiScale();
		Vector2i size_win((int32)(size.width * scaleFactor), (int32)(size.height * scaleFactor));
		Vector2i pos_win((int32)(position.x * scaleFactor), (int32)(position.y * scaleFactor));

		if (positionType == WindowPositionType::CenteredPosition) {
			pos_win.x = ((long)GetSystemMetrics(SM_CXSCREEN) - size_win.width) / 2;
			pos_win.y = ((long)GetSystemMetrics(SM_CYSCREEN) - size_win.height) / 2;
		}
		else if (positionType == WindowPositionType::RandomPosition) {
			pos_win.x = Random.NextInt32((long)GetSystemMetrics(SM_CXSCREEN) - size_win.width);
			pos_win.y = Random.NextInt32((long)GetSystemMetrics(SM_CYSCREEN) - size_win.height);
		}

		m_Properties.position = pos_win;
		m_Properties.size = size_win;
		SetWindowPos(m_Data->windowHandle, 0, pos_win.x, pos_win.y, size_win.width, size_win.height, 0);
	}

	void Win32Window::GrabWindowCursor(bool grabbed)
	{
		if (!IsValidDisplay()) {
			return;
		}

		if (grabbed) {
			RECT rect;
			GetClientRect(m_Data->windowHandle, &rect);
			MapWindowPoints(m_Data->windowHandle, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
			ClipCursor(&rect);
		}
		else {
			ClipCursor(nullptr);
		}
	}

}  //  nkentseu

#endif
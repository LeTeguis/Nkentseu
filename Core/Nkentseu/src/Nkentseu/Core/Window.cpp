//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 5:04:02 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Window.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS
#include "Nkentseu/Platform/Window/Windows/WindowInternal.h"
#elif defined(NKENTSEU_PLATFORM_ANDROID)
#include "Nkentseu/Platform/Window/Android/WindowInternal.h"
#elif defined(NKENTSEU_PLATFORM_LINUX)
	#ifdef NKENTSEU_LINUX_WIN_API_XCB
    #include "Nkentseu/Platform/Window/Linux/XCB/WindowInternal.h"
    #elif defined(NKENTSEU_LINUX_WIN_API_XLIB)
    #include "Nkentseu/Platform/Window/Linux/XLIB/WindowInternal.h"
    #endif
#elif defined(NKENTSEU_PLATFORM_IOS)
#include "Nkentseu/Platform/Window/iOS/WindowInternal.h"
#elif defined(NKENTSEU_PLATFORM_MACOS)
#include "Nkentseu/Platform/Window/Macos/WindowInternal.h"
#elif defined(NKENTSEU_PLATFORM_NDS)
#include "Nkentseu/Platform/Window/NDS/WindowInternal.h"
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported"
#endif

#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {

	Window::Window() : m_WindowInternal(nullptr) {}

	Window::Window(const WindowProperties& properties) : m_WindowInternal(nullptr) {
		Initialize(properties);
	}

	NTSErrorCode Window::Initialize(const WindowProperties& properties) {
		if (m_WindowInternal == nullptr) {
			m_WindowInternal = Memory::Alloc<WindowInternal>(this, properties);
			if (m_WindowInternal != nullptr) {
				m_IsInitialized = m_WindowInternal->IsCreate();

				if (!m_IsInitialized) {
					Memory::Reset(m_WindowInternal);
				}
			}
		}
		return NTSErrorCode::NoError;
	}

	Window::~Window() {
		//Close();
	}

	std::string Window::GetTitle() const {
		ValidateObject(m_WindowInternal, true, "", GetTitle);
	}

	void Window::SetTitle(std::string title) {
		ValidateObject(m_WindowInternal, false, , SetTitle, title);
	}

	Vector2i Window::GetPosition() const {
		ValidateObject(m_WindowInternal, true, Vector2i(), GetPosition);
	}

	void Window::SetPosition(int32 x, int32 y) {
		ValidateObject(m_WindowInternal, false, , SetPosition, x, y);
	}

	void Window::SetPosition(const Vector2i& pos) {
		ValidateObject(m_WindowInternal, false, , SetPosition, pos);
	}

	Vector2u Window::GetSize() {
		ValidateObject(m_WindowInternal, true, Vector2u(), GetSize);
	}

	void Window::SetSize(uint32 width, uint32 height) {
		ValidateObject(m_WindowInternal, false, , SetSize, width, height);
	}

	void Window::SetSize(const Vector2u& size) {
		ValidateObject(m_WindowInternal, false, , SetSize, size);
	}

	float32 Window::GetDpiScale() const {
		ValidateObject(m_WindowInternal, true, 0, GetDpiScale);
	}

	Vector2 Window::ConvertPixelToDpi(const Vector2& pixel) const {
		ValidateObject(m_WindowInternal, true, Vector2(), ConvertPixelToDpi, pixel);
	}

	Vector2u Window::CurrentDisplaySize() const {
		ValidateObject(m_WindowInternal, true, Vector2u(), CurrentDisplaySize);
	}

	Vector2i Window::CurrentDisplayPosition() const {
		ValidateObject(m_WindowInternal, true, Vector2i(), CurrentDisplayPosition);
	}

	void Window::ShowMouse(bool show) {
		ValidateObject(m_WindowInternal, false, , ShowMouse, show);
	}

	void Window::SetMouseCursorGrabbed(bool grabbed) {
		ValidateObject(m_WindowInternal, false, , SetMouseCursorGrabbed, grabbed);
	}

	void Window::SetMouseCursor(const WindowCursor& cursor) {
		ValidateObject(m_WindowInternal, false, , SetMouseCursor, cursor);
	}

	bool Window::IsMouseInside() {
		ValidateObject(m_WindowInternal, true, false, IsMouseInside);
	}

	Color Window::GetBackgroundColor() {
		ValidateObject(m_WindowInternal, true, Color(), GetBackgroundColor);
	}

	void Window::SetBackgroundColor(const Color& color) {
		ValidateObject(m_WindowInternal, false, , SetBackgroundColor, color);
	}

	void Window::Minimize() {
		ValidateObject(m_WindowInternal, false, , Minimize);
	}

	void Window::Maximize() {
		ValidateObject(m_WindowInternal, false, , Maximize);
	}

	void Window::Close() {
		if (m_WindowInternal != nullptr) {
			ValidateObject(m_WindowInternal, false, , Close);
			if (m_IsInitialized) {
				m_IsInitialized = false;
				Memory::Reset(m_WindowInternal);
			}
		}
	}

	bool Window::IsClosed() {
		ValidateObject(m_WindowInternal, true, true, IsClosed);
	}

	bool Window::IsOpen() {
		ValidateObject(m_WindowInternal, true, false, IsOpen);
	}

	void Window::Show() {
		ValidateObject(m_WindowInternal, false, , Show);
	}

	void Window::Hide() {
		ValidateObject(m_WindowInternal, false, , Hide);
	}

	void Window::SetIcon(const Vector2u& size, const uint8* pixels) {
		ValidateObject(m_WindowInternal, false, , SetIcon, size, pixels);
	}

	WindowInternal* Window::GetInternal() {
		return m_WindowInternal.get();
	}

	const WindowProperties& Window::GetProperties() {
		static WindowProperties prop;
		ValidateObject(m_WindowInternal, true, prop, GetProperties);
		return prop;
	}

	uint64 Window::ID() {
		ValidateObject(m_WindowInternal, true, 0, ID);
	}

	void Window::Clear(const Color& color) {
		Assert_nts.ATrue(m_WindowInternal == nullptr || m_WindowInternal == nullptr);
		ValidateObject(m_WindowInternal, false, , Clear, color);
	}

	void Window::Swapbuffer() {
		ValidateObject(m_WindowInternal, false, , Swapbuffer);
	}

	void Window::SetAlwaysOnTop(bool alwaysOnTop) {
		ValidateObject(m_WindowInternal, false, , SetAlwaysOnTop, alwaysOnTop);
	}

	void Window::AcceptDragAndDrop(bool accept) {
		ValidateObject(m_WindowInternal, false, , AcceptDragAndDrop, accept);
	}

	void Window::SetOpacity(float32 opacity) {
		ValidateObject(m_WindowInternal, false, , SetOpacity, opacity);
	}

}    // namespace nkentseu
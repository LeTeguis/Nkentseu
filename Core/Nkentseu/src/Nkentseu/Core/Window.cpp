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
#include "Nkentseu/Platform/Window/Linux/WindowInternal.h"
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

	Window::Window() : m_WindowInternalInfo(nullptr) {}

	Window::Window(const WindowProperties& properties) : m_WindowInternalInfo(nullptr) {
		Initialize(properties);
	}

	NTSErrorCode Window::Initialize(const WindowProperties& properties) {
		if (m_WindowInternalInfo == nullptr) {
			m_WindowInternalInfo = Memory::Alloc<WindowInternalInfo>();
			if (m_WindowInternalInfo != nullptr) {
				m_WindowInternalInfo->InternalInfo = Memory::Alloc<WindowInternal>(this, properties);
				if (m_WindowInternalInfo->InternalInfo != nullptr) {
					m_WindowInternalInfo->IsLoad = m_WindowInternalInfo->InternalInfo->IsCreate();

					if (!m_WindowInternalInfo->IsLoad) {
						Memory::Reset(m_WindowInternalInfo->InternalInfo);
						m_WindowInternalInfo->IsLoad = false;
						Memory::Reset(m_WindowInternalInfo);
					}
					else {
						/*/m_Context = PtrAlloc(Context, this, properties.ContextInfo);

						if (m_Context == nullptr || !m_Context->Create()) {
							Close();
						}*/
					}
				}
			}
		}
		return NTSErrorCode::Error_No;
	}

	Window::~Window() {
		Close();
	}

	std::string Window::GetTitle() const {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, "", GetTitle);
	}

	void Window::SetTitle(std::string title) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetTitle, title);
	}

	Vector2i Window::GetPosition() const {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, Vector2i(), GetPosition);
	}

	void Window::SetPosition(int32 x, int32 y) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetPosition, x, y);
	}

	void Window::SetPosition(const Vector2i& pos) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetPosition, pos);
	}

	Vector2u Window::GetSize() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, Vector2u(), GetSize);
	}

	void Window::SetSize(uint32 width, uint32 height) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetSize, width, height);
	}

	void Window::SetSize(const Vector2u& size) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetSize, size);
	}

	float32 Window::GetDpiScale() const {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, 0, GetDpiScale);
	}

	Vector2 Window::ConvertPixelToDpi(const Vector2& pixel) const {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, Vector2(), ConvertPixelToDpi, pixel);
	}

	Vector2u Window::CurrentDisplaySize() const {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, Vector2u(), CurrentDisplaySize);
	}

	Vector2i Window::CurrentDisplayPosition() const {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, Vector2i(), CurrentDisplayPosition);
	}

	void Window::ShowMouse(bool show) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , ShowMouse, show);
	}

	void Window::SetMouseCursorGrabbed(bool grabbed) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetMouseCursorGrabbed, grabbed);
	}

	void Window::SetMouseCursor(const WindowCursor& cursor) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetMouseCursor, cursor);
	}

	bool Window::IsMouseInside() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, false, IsMouseInside);
	}

	Color Window::GetBackgroundColor() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, Color(), GetBackgroundColor);
	}

	void Window::SetBackgroundColor(const Color& color) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetBackgroundColor, color);
	}

	void Window::Minimize() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , Minimize);
	}

	void Window::Maximize() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , Maximize);
	}

	void Window::Close() {
		if (m_WindowInternalInfo != nullptr) {
			if (m_WindowInternalInfo->InternalInfo != nullptr) {
				ValidateObject(m_WindowInternalInfo, InternalInfo, false, , Close);
				if (m_WindowInternalInfo->IsLoad) {
					m_WindowInternalInfo->IsLoad = false;
					Memory::Reset(m_WindowInternalInfo->InternalInfo);
				}
			}
			Memory::Reset(m_WindowInternalInfo);
		}
	}

	bool Window::IsClosed() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, true, IsClosed);
	}

	bool Window::IsOpen() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, false, IsOpen);
	}

	void Window::Show() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , Show);
	}

	void Window::Hide() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , Hide);
	}

	void Window::SetIcon(const Vector2u& size, const uint8* pixels) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetIcon, size, pixels);
	}

	WindowInternal* Window::GetWindowInternal() {
		return m_WindowInternalInfo->InternalInfo.get();
	}

	const WindowProperties& Window::GetProperties() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, WindowProperties(), GetProperties);
	}

	uint64 Window::ID() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, true, 0, ID);
	}

	void Window::Clear(const Color& color) {
		Assert_nts.ATrue(m_WindowInternalInfo == nullptr || m_WindowInternalInfo->InternalInfo == nullptr);
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , Clear, color);
	}

	void Window::Swapbuffer() {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , Swapbuffer);
	}

	void Window::SetAlwaysOnTop(bool alwaysOnTop) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetAlwaysOnTop, alwaysOnTop);
	}

	void Window::AcceptDragAndDrop(bool accept) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , AcceptDragAndDrop, accept);
	}

	void Window::SetOpacity(float32 opacity) {
		ValidateObject(m_WindowInternalInfo, InternalInfo, false, , SetOpacity, opacity);
	}

}    // namespace nkentseu
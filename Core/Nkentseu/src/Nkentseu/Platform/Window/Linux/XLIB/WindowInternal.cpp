//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 9:54:56 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowInternal.h"

#if defined(NKENTSEU_PLATFORM_LINUX_XLIB)

#include "Nkentseu/Platform/PlatformState.h"
#include "Nkentseu/Core/NkentseuLogger.h"

#include <string.h>
#include <Ntsm/Random.h>

namespace nkentseu {
    uint64 WindowInternal::s_WindowIDCounter = 0;

    #define PLATFORM_DECLARATION()      ::Display *display = PlatformState.display; \
                                        int32 screen = PlatformState.screen; \
                                        unsigned long rootWindow = PlatformState.rootWindow; \
                                        ::Visual *visual = PlatformState.visual; \
                                        ::Colormap colormap = PlatformState.colormap 

    WindowInternal::WindowInternal(class Window* window, const WindowProperties& properties) : m_WindowID(++s_WindowIDCounter), m_MainWindow(window), m_IsWindowCreated(false), m_Properties(properties) {
        if (!IsValidPlatform()) {
            Log_nts.Error("Failed to get display connexion");
            return;
        }

        PLATFORM_DECLARATION();

        m_NativeWindow = Memory::Alloc<WindowDisplay>();

        if (m_NativeWindow == nullptr){
            ErrorMessaging.PushError(NTSErrorCode::Window_CreateNative);
            Log_nts.Error("Native Window is not created");
            return;
        }

        if (!m_NativeWindow->Register(m_Properties.doubleClick)) {
            ErrorMessaging.PushError(NTSErrorCode::Window_RegisterWindowClass);
            Log_nts.Error("Native Window registration failled");
            return;
        }

        m_NativeWindow->attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;
        m_NativeWindow->attributes.colormap = colormap;

        Vector2i position = m_Properties.position;
        Vector2u size = m_Properties.size;

        m_NativeWindow->windowHandle = XCreateWindow(display, rootWindow, position.x, position.y, size.width, size.height, 0, DefaultDepth(display, screen), InputOutput, visual, CWColormap | CWEventMask, &m_NativeWindow->attributes);

        currentWindowInternal = this;
        GetCurrent(m_NativeWindow->windowHandle);

        if (!m_NativeWindow->windowHandle) {
            ErrorMessaging.PushError(NTSErrorCode::Window_Create);
            Log_nts.Error("Unable to create window");
            return;
        }

        Log_nts.Debug("Window id = {0}", m_NativeWindow->windowHandle);

        XMapWindow(display, m_NativeWindow->windowHandle);
        XStoreName(display, m_NativeWindow->windowHandle, m_Properties.title.c_str());

        m_NativeWindow->SetProtocol();

        m_IsWindowClosed = false;
        m_IsWindowCreated = true;
    }

    WindowInternal::~WindowInternal() {
        Close();
    }

    void WindowInternal::InitWindowPosition() {
    }

    std::string WindowInternal::GetTitle() const {
        return "";
    }

    void WindowInternal::SetTitle(std::string title) {
    }

    Vector2i WindowInternal::GetPosition() const {
        return {};
    }

    void WindowInternal::SetPosition(int32 x, int32 y) {
    }

    void WindowInternal::SetPosition(const Vector2i& pos) { SetPosition(pos.x, pos.y); }

    Vector2u WindowInternal::GetSize() {
        return {};
    }

    void WindowInternal::SetSize(uint32 width, uint32 height) {
    }

    void WindowInternal::SetSize(const Vector2u& size) { SetSize(size.width, size.height); }

    void WindowInternal::SetMinSize(const Vector2u& size) {
    }

    void WindowInternal::SetMaxSize(const Vector2u& size) {
    }

    float32 WindowInternal::GetDpiScale() const {
        return 1.0f;
    }

    Vector2 WindowInternal::ConvertPixelToDpi(const Vector2& pixel) const {
        return {};
    }

    Vector2u WindowInternal::CurrentDisplaySize() const {
        return {};
    }

    Vector2i WindowInternal::CurrentDisplayPosition() const {
        return Vector2i();
    }

    void WindowInternal::ShowMouse(bool show) {
    }

    void WindowInternal::SetMouseCursorGrabbed(bool grabbed) {
    }

    void WindowInternal::SetMouseCursor(const WindowCursor& cursor) {
    }

    bool WindowInternal::IsMouseInside() {
        return false;
    }

    Color WindowInternal::GetBackgroundColor() {
        return {};
    }

    void WindowInternal::SetBackgroundColor(const Color& color) {
    }

    void WindowInternal::Minimize() {
    }

    void WindowInternal::Maximize() {
    }

    void WindowInternal::Close() {
        if (!IsValidDisplay()) return;
        PLATFORM_DECLARATION();
        
        if (m_IsWindowClosed == false){
            /* if window was created then destroy it */
            ::Window id_window = m_NativeWindow->windowHandle;
            XDestroyWindow(PlatformState.display, m_NativeWindow->windowHandle);
            windowHandleMap.erase(m_NativeWindow->windowHandle);
            m_IsWindowClosed = true;
        }
    }

    bool WindowInternal::IsClosed() {
        return false;
    }

    bool WindowInternal::IsOpen() {
        return false;
    }

    void WindowInternal::Show() {
    }

    void WindowInternal::Hide() {
    }

    void WindowInternal::SetIcon(const Vector2u& size, const uint8* pixels) {
    }

    Memory::Shared<WindowDisplay> WindowInternal::GetWindowDisplay() {
        if (!IsValidDisplay()) return nullptr;
        return m_NativeWindow;
    }

    const WindowProperties& WindowInternal::GetProperties() {
        return WindowProperties::GetInValid();
    }

    uint64 WindowInternal::ID() {
        return m_WindowID;
    }

    void WindowInternal::Clear(const Color& color) {
    }

    void WindowInternal::Swapbuffer() {
    }

    void WindowInternal::SetProgress(float32 progress) {
    }

    void WindowInternal::SetAlwaysOnTop(bool alwaysOnTop) {
    }

    void WindowInternal::AcceptDragAndDrop(bool accept) {
    }

    void WindowInternal::SetOpacity(float32 opacity) {
    }

    bool WindowInternal::IsCreate() {
        return false;
    }

    void WindowInternal::SetBorder(uint32 border) {
    }

    WindowInternal* WindowInternal::GetCurrent(::Window window) {
        WindowInternal* _this;
        if (currentWindowInternal != nullptr) {
            windowHandleMap.emplace(window, currentWindowInternal);
            currentWindowInternal->m_NativeWindow->windowHandle = window;
            _this = currentWindowInternal;
            currentWindowInternal = nullptr;
        }
        else {
            auto existing = windowHandleMap.find(window);
            _this = existing->second;
        }
        return _this;
    }

    bool WindowInternal::IsValidPlatform() const {
        PLATFORM_DECLARATION();
        if (display == nullptr || rootWindow == 0 || visual == nullptr || colormap == 0) {
            Log_nts.Error("Failed to get screen iterator and connexion");
            return false;
        }
        return true;
    }

    bool WindowInternal::IsValidDisplay() const {
        return IsValidPlatform() && m_NativeWindow != nullptr && m_NativeWindow->windowHandle != 0;
    }
}    // namespace nkentseu

#endif
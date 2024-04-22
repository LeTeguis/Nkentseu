//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 9:54:56 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowInternal.h"

#ifdef NKENTSEU_PLATFORM_LINUX
#include "Nkentseu/Platform/PlatformState.h"
#include "Nkentseu/Core/NkentseuLogger.h"

namespace nkentseu {

    uint64 WindowInternal::s_WindowIDCounter = 0;

    WindowInternal::WindowInternal(class Window* window, const WindowProperties& properties) : m_WindowID(++s_WindowIDCounter), m_MainWindow(window), m_IsWindowCreated(false), m_Properties(properties) {
        if (window == nullptr) {
            ErrorMessaging.PushError(NTSErrorCode::Window_ExternalAssign);
            return;
        }

        m_NativeWindow = Memory::Alloc<WindowDisplay>();

        if (m_NativeWindow == nullptr) {
            ErrorMessaging.PushError(NTSErrorCode::Window_CreateNative);
            return;
        }

        Vector2i position = m_Properties.position;
        Vector2u size = m_Properties.size;

        if (!m_NativeWindow->Register(m_Properties.doubleClick)) {
            ErrorMessaging.PushError(NTSErrorCode::Window_RegisterWindowClass);
            return;
        }
        // Log_nts.Debug();

        xcb_connection_t *connection = PlatformState_::Instance().connection;

        // mask
        uint32 mask;
        mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

        // Stocker la valeur dans une variable locale
        uint32 values[2];
        values[0] = m_Properties.backgroundColor.ToUint32A();
        values[1] = XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS         |
                    XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION       |
                    XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW         |
                    XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE          |
                    XCB_EVENT_MASK_FOCUS_CHANGE   | XCB_EVENT_MASK_VISIBILITY_CHANGE    |
                    XCB_EVENT_MASK_OWNER_GRAB_BUTTON;
        // Log_nts.Debug();

        xcb_create_window(connection,
                                m_Properties.bitsPerPixel,
                                m_NativeWindow->windowHandle,
                                m_NativeWindow->screenHandle->root,
                                position.x, position.y, size.width, size.height,
                                1,
                                XCB_WINDOW_CLASS_INPUT_OUTPUT,
                                m_NativeWindow->screenHandle->root_visual,
                                mask, values
                            );

        if (m_Properties.visible){
            xcb_map_window(connection, m_NativeWindow->windowHandle);
        }

        Assert_nts.ATrue(xcb_flush(connection) <= 0);
    }

    WindowInternal::~WindowInternal() {
    }

    std::string WindowInternal::GetTitle() const {
        return "";
    }

    void WindowInternal::SetTitle(std::string title) {
    }

    Vector2i WindowInternal::GetPosition() const {
        return m_Properties.position;
    }

    void WindowInternal::SetPosition(int32 x, int32 y) {
        m_Properties.position = Vector2i{x, y};
        const uint32 values[] = {x,y};
        xcb_configure_window(PlatformState_::Instance().connection, m_NativeWindow->windowHandle, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, values);
    }

    void WindowInternal::SetPosition(const Vector2i& pos) { SetPosition(pos.x, pos.y); }

    Vector2u WindowInternal::GetSize() {
        if (m_NativeWindow == nullptr) {
            return {};
        }
        return m_Properties.size;
    }

    void WindowInternal::SetSize(uint32 width, uint32 height) {    
        const uint32 values[] = {m_Properties.size.x, m_Properties.size.y};
        xcb_configure_window(PlatformState_::Instance().connection, m_NativeWindow->windowHandle, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
    
    }

    void WindowInternal::SetSize(const Vector2u& size) { SetSize(size.width, size.height); }

    float32 WindowInternal::GetDpiScale() const {
        return 1.0f;
    }

    Vector2 WindowInternal::ConvertPixelToDpi(const Vector2& pixel) const {
        Vector2 pixel_(pixel);
        return pixel_ / GetDpiScale();
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
        return m_NativeWindow == nullptr ? false :(*m_NativeWindow).isMouseInside;
    }

    Color WindowInternal::GetBackgroundColor() {
        return m_NativeWindow == nullptr ? 0x00000000 : m_Properties.backgroundColor;
    }

    void WindowInternal::SetBackgroundColor(const Color& color) {
    }

    void WindowInternal::Minimize() {
    }

    void WindowInternal::Maximize() {
    }

    void WindowInternal::Close() {
    }

    bool WindowInternal::IsClosed() {
        return m_IsWindowClosed;
    }

    bool WindowInternal::IsOpen() {
        return !IsClosed();
    }

    void WindowInternal::Show() {
        if (!m_Properties.visible){
            m_Properties.visible = true;
            xcb_map_window(PlatformState_::Instance().connection, m_NativeWindow->windowHandle);
        }
    }

    void WindowInternal::Hide() {
    }

    void WindowInternal::SetIcon(const Vector2u& size, const uint8* pixels) {
    }

    Memory::Shared<WindowDisplay> WindowInternal::GetWindowDisplay() {
        return m_NativeWindow;
    }

    const WindowProperties& WindowInternal::GetProperties() {
        if (m_NativeWindow != nullptr) {
            return m_Properties;
        }
        return WindowProperties::GetInValid();
    }

    uint64 WindowInternal::ID() {
        return m_WindowID;
    }

    void WindowInternal::Clear(const Color& color) {
    }

    void WindowInternal::Swapbuffer() {
        xcb_flush(PlatformState.connection);
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
        return m_IsWindowCreated;
    }

    WindowInternal* WindowInternal::GetCurrent(xcb_window_t window) {
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
}    // namespace nkentseu

#endif
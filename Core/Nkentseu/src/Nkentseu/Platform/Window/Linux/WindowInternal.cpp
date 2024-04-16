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

    WindowInternal::WindowInternal(class Window* window, const WindowProperties& properties) : m_WindowID(++s_WindowIDCounter), m_MainWindow(window), m_IsWindowCreated(false) {
        if (window == nullptr) {
            ErrorMessaging.PushError(NTSErrorCode::Window_ExternalAssign);
            return;
        }

        m_NativeWindow = Memory::Alloc<WindowDisplay>();

        if (m_NativeWindow == nullptr) {
            ErrorMessaging.PushError(NTSErrorCode::Window_CreateNative);
            return;
        }

        m_NativeWindow->windowProperties = properties;
        Vector2i position = m_NativeWindow->windowProperties.position;
        Vector2u size = m_NativeWindow->windowProperties.size;
        WindowProperties properties_ = m_NativeWindow->windowProperties;

        Log_nts.Debug();
        if (!m_NativeWindow->Register(properties_.doubleClick)) {
            ErrorMessaging.PushError(NTSErrorCode::Window_RegisterWindowClass);
            Log_nts.Debug();
            return;
        }
        Log_nts.Debug();

        xcb_connection_t *connection = PlatformState_::Instance().connection;

        // Stocker la valeur dans une variable locale
        xcb_create_window_value_list_t valueList = {
            .background_pixel = properties_.backgroundColor.ToUint32A(),
            .event_mask = XCB_EVENT_MASK_EXPOSURE
        };
        Log_nts.Debug();

        xcb_create_window_aux(connection,
                                m_NativeWindow->screenHandle->root_depth,
                                m_NativeWindow->windowHandle,
                                m_NativeWindow->screenHandle->root,
                                position.x, position.y, size.width, size.height,
                                1,
                                XCB_WINDOW_CLASS_INPUT_OUTPUT,
                                m_NativeWindow->screenHandle->root_visual,
                                XCB_CW_EVENT_MASK | XCB_CW_BACK_PIXEL, &valueList
                            );

        xcb_atom_t xcb_atom = {m_NativeWindow->windowManagerWindowDeleteProtocol};

        xcb_icccm_set_wm_name(connection, m_NativeWindow->windowHandle, XCB_ATOM_STRING, 8, properties_.title.size(), properties_.title.c_str());
        xcb_icccm_set_wm_protocols(connection, m_NativeWindow->windowHandle, m_NativeWindow->windowManagerProtocolsProperty, 1, &xcb_atom);

        if (!properties_.resizable) {
            xcb_size_hints_t window_size_hints;
            xcb_icccm_size_hints_set_min_size(&window_size_hints, properties_.minSize.width, properties_.minSize.height);
            xcb_icccm_size_hints_set_max_size(&window_size_hints, properties_.maxSize.width, properties_.maxSize.height);
            xcb_icccm_set_wm_size_hints(connection, m_NativeWindow->windowHandle, XCB_ATOM_WM_NORMAL_HINTS, &window_size_hints);
        }
        Log_nts.Debug();
        xcb_map_window(connection, m_NativeWindow->windowHandle);
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
        return {};
    }

    void WindowInternal::SetPosition(int32 x, int32 y) {
    }

    void WindowInternal::SetPosition(const Vector2i& pos) { SetPosition(pos.x, pos.y); }

    Vector2u WindowInternal::GetSize() {
        if (m_NativeWindow == nullptr) {
            return {};
        }
        xcb_get_geometry_cookie_t cokie = xcb_get_geometry(PlatformState_::Instance().connection, m_NativeWindow->windowHandle);
        xcb_get_geometry_reply_t* reply = xcb_get_geometry_reply(PlatformState_::Instance().connection, cokie, NULL);
        
        if (!reply){
            return m_NativeWindow->windowProperties.size;
        }

        Vector2u size = {reply->width, reply->height};
        m_NativeWindow->windowProperties.size = size;
        return size;
    }

    void WindowInternal::SetSize(uint32 width, uint32 height) {
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
        return false;
    }

    Color WindowInternal::GetBackgroundColor() {
        return m_NativeWindow == nullptr ? 0x00000000 : m_NativeWindow->windowProperties.backgroundColor;
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
            return m_NativeWindow->windowProperties;
        }
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
        return m_IsWindowCreated;
    }

}    // namespace nkentseu

#endif
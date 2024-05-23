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

        m_NativeWindow->attributes.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask          |
                                                PointerMotionMask/* | PointerMotionHintMask*/ | Button1MotionMask | Button2MotionMask | Button3MotionMask        |
                                                Button4MotionMask | Button5MotionMask | ButtonMotionMask | KeymapStateMask | ExposureMask | VisibilityChangeMask |
                                                StructureNotifyMask | ResizeRedirectMask | SubstructureNotifyMask /*| SubstructureRedirectMask*/ | FocusChangeMask   |
                                                PropertyChangeMask | ColormapChangeMask | OwnerGrabButtonMask;
        m_NativeWindow->attributes.colormap = colormap;

        Vector2i position = m_Properties.position;
        Vector2u size = m_Properties.size;

        position = InitWindowPosition(position, size, m_Properties.positionType);
        Log_nts.Debug("{0}", position);

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

        SendWindowCreatedEvent();
    }

    WindowInternal::~WindowInternal() {
        Close();
    }

    Vector2i WindowInternal::InitWindowPosition(const Vector2i& position, const Vector2u& size, WindowPositionType positionType) {
        PLATFORM_DECLARATION();
        // Récupérer la taille de l'écran
        Vector2i screenSize(XDisplayWidth(display, screen), XDisplayHeight(display, screen));

        Vector2i windowPosition(position);

        if (positionType == WindowPositionType::CenteredPosition) {
            windowPosition.x = (screenSize.width - size.width) / 2;
            windowPosition.y = (screenSize.height - size.height) / 2;
            Log_nts.Debug("Center : {0}, size : {1}, reel_size : {2}", windowPosition, screenSize, size);
        }
        else if (positionType == WindowPositionType::RandomPosition) {
            windowPosition.x = Random.NextInt32(screenSize.width - size.width);
            windowPosition.y = Random.NextInt32(screenSize.height - size.height);
        }
        return windowPosition;
    }

    std::string WindowInternal::GetTitle() const {
        if (!IsValidDisplay()) return "";
        return m_Properties.title;
    }

    void WindowInternal::SetTitle(std::string title) {
        if (!IsValidDisplay()) return;
        PLATFORM_DECLARATION();

        XStoreName(display, m_NativeWindow->windowHandle, title.c_str());
        XFlush(display);
    }

    Vector2i WindowInternal::GetPosition() const {
        if (!IsValidDisplay()) return {};
        return m_Properties.position;
    }

    void WindowInternal::SetPosition(int32 x, int32 y) {
        if (!IsValidDisplay()) return;
        PLATFORM_DECLARATION();

        XMoveWindow(display, m_NativeWindow->windowHandle, x, y);
        XFlush(display);
    }

    void WindowInternal::SetPosition(const Vector2i& pos) { SetPosition(pos.x, pos.y); }

    Vector2u WindowInternal::GetSize() {
        if (!IsValidDisplay()) return {};
        return m_Properties.size;
    }

    void WindowInternal::SetSize(uint32 width, uint32 height) {
        if (!IsValidDisplay()) return;
        PLATFORM_DECLARATION();

        XResizeWindow(display, m_NativeWindow->windowHandle, width, height);
        XFlush(display);
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
        return pixel;
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
        if (!IsValidDisplay()) return {};
        return m_Properties.backgroundColor;
    }

    void WindowInternal::SetBackgroundColor(const Color& color) {
        if (!IsValidDisplay()) return;
        m_Properties.backgroundColor = color;
    }

    void WindowInternal::Minimize() {
        if (!IsValidDisplay()) return;
        PLATFORM_DECLARATION();

        XEvent event;
        memset(&event, 0, sizeof(event));
        event.type = ClientMessage;
        event.xclient.window = m_NativeWindow->windowHandle;
        event.xclient.message_type = PlatformState.protocolAtom.wmChangeState;
        event.xclient.format = 32;
        event.xclient.data.l[0] = 1;  // Activate
        event.xclient.data.l[1] = PlatformState.protocolAtom.wmMinimize;
        event.xclient.data.l[2] = 0;

        XSendEvent(display, rootWindow, False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
        XFlush(display);
    }

    void WindowInternal::Maximize() {
        if (!IsValidDisplay()) return;
        PLATFORM_DECLARATION();

        XEvent event;
        memset(&event, 0, sizeof(event));
        event.type = ClientMessage;
        event.xclient.window = m_NativeWindow->windowHandle;
        event.xclient.message_type = PlatformState.protocolAtom.wmChangeState;
        event.xclient.format = 32;
        event.xclient.data.l[0] = 1;  // Activate
        event.xclient.data.l[1] = PlatformState.protocolAtom.wmMaximizeHorz;
        event.xclient.data.l[2] = PlatformState.protocolAtom.wmMaximizeVert;

        XSendEvent(display, rootWindow, False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
        XFlush(display);
    }

    void WindowInternal::Close() {
        if (!IsValidDisplay()) return;
        PLATFORM_DECLARATION();
        
        if (m_IsWindowClosed == false){
            /* if window was created then destroy it */
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

    WindowInternal* WindowInternal::GetCurrent(unsigned long window) {
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

    void WindowInternal::SendWindowCreatedEvent(){
        PLATFORM_DECLARATION();
        XClientMessageEvent event;
        event.type = ClientMessage;
        event.message_type = PlatformState.protocolAtom.windowCreatedAtom;
        event.format = 32;
        event.window = m_NativeWindow->windowHandle;
        event.data.l[0] = 1; // Utilisation d'une valeur arbitraire pour indiquer l'état créé

        XSendEvent(display, m_NativeWindow->windowHandle, False, NoEventMask, (XEvent*)&event);
        XFlush(display);
    }
}    // namespace nkentseu

#endif
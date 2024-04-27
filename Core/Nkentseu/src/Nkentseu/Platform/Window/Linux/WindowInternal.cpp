//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 9:54:56 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowInternal.h"

#ifdef NKENTSEU_PLATFORM_LINUX
#include "Nkentseu/Platform/PlatformState.h"
#include "Nkentseu/Core/NkentseuLogger.h"

#include <string.h>

namespace nkentseu {
    #define NATIVE_WINDOW_IS_VALID(return_value)        xcb_connection_t* connection = PlatformState.connection; \
                                                        xcb_screen_t *screenHandle = PlatformState.screen; \
                                                        if (screenHandle == nullptr || connection == nullptr) {\
                                                            Log_nts.Error("Failed to get screen iterator and connexion");\
                                                            return return_value;\
                                                        }\
                                                        if (m_NativeWindow == nullptr || m_NativeWindow->windowHandle == (xcb_window_t)-1){\
                                                            return return_value;\
                                                        }

    uint64 WindowInternal::s_WindowIDCounter = 0;

    WindowInternal::WindowInternal(class Window* window, const WindowProperties& properties) : m_WindowID(++s_WindowIDCounter), m_MainWindow(window), m_IsWindowCreated(false), m_Properties(properties) {
        xcb_connection_t* connection = PlatformState.connection;
        xcb_screen_t *screenHandle = PlatformState.screen;

        if (screenHandle == nullptr || connection == nullptr) {
            Log_nts.Error("Failed to get screen iterator and connexion");
            return;
        }

        XCBStateTools& stateTools = PlatformState.stateTools;

        if (window == nullptr) {
            ErrorMessaging.PushError(NTSErrorCode::Window_ExternalAssign);
            Log_nts.Error("Window External assign failled");
            return;
        }

        m_NativeWindow = Memory::Alloc<WindowDisplay>();

        if (m_NativeWindow == nullptr || m_NativeWindow->windowHandle == (xcb_window_t)-1) {
            ErrorMessaging.PushError(NTSErrorCode::Window_CreateNative);
            Log_nts.Error("Native Window is not created");
            return;
        }

        currentWindowInternal = this;

        Vector2i position = m_Properties.position;
        Vector2u size = m_Properties.size;

        if (!m_NativeWindow->Register(m_Properties.doubleClick)) {
            ErrorMessaging.PushError(NTSErrorCode::Window_RegisterWindowClass);
            Log_nts.Error("NAtive Window registration failled");
            return;
        }

        // mask
        uint32 mask;
        mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

        // Stocker la valeur dans une variable locale
        uint32 values[2];
        values[0] = m_Properties.backgroundColor.ToUint32A();
        values[1] = XCB_EVENT_MASK_EXPOSURE             | XCB_EVENT_MASK_KEY_PRESS          | 
                    XCB_EVENT_MASK_KEY_RELEASE          | XCB_EVENT_MASK_BUTTON_PRESS       | 
                    XCB_EVENT_MASK_BUTTON_RELEASE       | XCB_EVENT_MASK_ENTER_WINDOW       |
                    XCB_EVENT_MASK_LEAVE_WINDOW         | XCB_EVENT_MASK_FOCUS_CHANGE       |
                    XCB_EVENT_MASK_VISIBILITY_CHANGE    | XCB_EVENT_MASK_STRUCTURE_NOTIFY   |
                    XCB_EVENT_MASK_PROPERTY_CHANGE      | XCB_EVENT_MASK_POINTER_MOTION     |
                    XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY  | XCB_EVENT_MASK_RESIZE_REDIRECT;
        // Log_nts.Debug();

        xcb_create_window(      connection,
                                m_Properties.bitsPerPixel,
                                m_NativeWindow->windowHandle,
                                screenHandle->root,
                                position.x, position.y, size.width, size.height,
                                1,
                                XCB_WINDOW_CLASS_INPUT_OUTPUT,
                                screenHandle->root_visual,
                                mask, values
                            );
        /* create new atom to help us detect close window event messages */
        xcb_change_property (
            connection,                      /* xcb connection */
            XCB_PROP_MODE_REPLACE,     /* replace the property with new value */
            m_NativeWindow->windowHandle,       /* xcb id of window object */
            stateTools.WM_PROTOCOLS,     /* change something in protocl */
            XCB_ATOM_ATOM,             /* change an atom */
            32,                        /* process data in chunks of 32 bits */
            1,                         /* length of data */
            &stateTools.WM_DELETE_WINDOW /* data */
        );

        SetTitle(properties.title);
        Log_nts.Debug("wind id = {0}", m_NativeWindow->windowHandle);

        xcb_map_window(connection, m_NativeWindow->windowHandle);
        Assert_nts.ATrue(xcb_flush(connection) <= 0);

        SetMinSize(m_Properties.minSize);
        SetMinSize(m_Properties.maxSize);
        Log_nts.Debug();
        //m_NativeWindow->SetSate(NtsWindowStateMask::);
        //m_NativeWindow->SetPermission(NtsWindowActionPermissionMask::NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_RESIZE);
    }

    WindowInternal::~WindowInternal() {
    }

    std::string WindowInternal::GetTitle() const {
        NATIVE_WINDOW_IS_VALID("");
        return m_Properties.title;
    }

    void WindowInternal::SetTitle(std::string title) {
        if (title == m_Properties.title) return;
        NATIVE_WINDOW_IS_VALID();

        char* set_title = strdup(title.c_str());
        if (set_title == nullptr) return;

        m_Properties.title = set_title;

        /* set title */
        xcb_change_property (
            connection,   /* xcb connection */
            XCB_PROP_MODE_REPLACE, /* replace the property with new value */
            m_NativeWindow->windowHandle,   /* id of object */
            XCB_ATOM_WM_NAME,      /* property is window name */
            XCB_ATOM_STRING,       /* atom type is string */
            8,                     /* process data in chunks of 8 bits */
            m_Properties.title.size(),  /* length of data */
            m_Properties.title.c_str()            /* data */
        );

        xcb_flush(connection);
    }

    Vector2i WindowInternal::GetPosition() const {
        NATIVE_WINDOW_IS_VALID({});
        return m_Properties.position;
    }

    void WindowInternal::SetPosition(int32 x, int32 y) {
        if (m_Properties.position.x == x && m_Properties.position.y == y) return;
        NATIVE_WINDOW_IS_VALID();

        m_Properties.position = Vector2i{x, y};
        const int32 values[] = {x , y};
        xcb_configure_window(PlatformState_::Instance().connection, m_NativeWindow->windowHandle, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, values);
        xcb_flush (connection);
    }

    void WindowInternal::SetPosition(const Vector2i& pos) { SetPosition(pos.x, pos.y); }

    Vector2u WindowInternal::GetSize() {
        NATIVE_WINDOW_IS_VALID({});
        return m_Properties.size;
    }

    void WindowInternal::SetSize(uint32 width, uint32 height) { 
        if (m_Properties.size.width == width && m_Properties.size.height == height) return;
        NATIVE_WINDOW_IS_VALID();

         /* make sure the size is in bounds */
        if (width < m_Properties.minSize.width || width > m_Properties.maxSize.width ||
            height < m_Properties.minSize.height || height > m_Properties.maxSize.height) {
            return;
        }

        /* store new size */
        m_Properties.size = Vector2u(width, height);
        const uint32 values[] = {width , height};

        /* set new size */
        xcb_configure_window (
            connection,
            m_NativeWindow->windowHandle,
            XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
            values
        );

        /* commit changes */
        xcb_flush (connection);
    }

    void WindowInternal::SetSize(const Vector2u& size) { SetSize(size.width, size.height); }

    void WindowInternal::SetMinSize(const Vector2u& size) {
        if (m_Properties.minSize.width == size.width && m_Properties.minSize.height == size.height) return;
        NATIVE_WINDOW_IS_VALID();

        if (m_Properties.minSize.width > m_Properties.maxSize.width || m_Properties.minSize.width > m_Properties.maxSize.width){
            Log_nts.Info("Min size bound cannot be greater than max size bound of window");
            return;
        }

        /* prepare hints */
        xcb_size_hints_t hints;
        xcb_icccm_size_hints_set_min_size (&hints, size.width, size.height);

        /* set hints */
        xcb_icccm_set_wm_size_hints (
            connection,
            m_NativeWindow->windowHandle,
            XCB_ATOM_WM_NORMAL_HINTS,
            &hints
        );

        xcb_flush (connection);
        m_Properties.minSize = size;
    }

    void WindowInternal::SetMaxSize(const Vector2u& size) {
        if (m_Properties.minSize.width == size.width && m_Properties.minSize.height == size.height) return;
        NATIVE_WINDOW_IS_VALID();

        if (m_Properties.minSize.width > m_Properties.maxSize.width || m_Properties.minSize.width > m_Properties.maxSize.width){
            Log_nts.Info("Min size bound cannot be greater than max size bound of window");
            return;
        }

        /* prepare hints */
        xcb_size_hints_t hints;
        xcb_icccm_size_hints_set_max_size (&hints, size.width, size.height);

        /* set hints */
        xcb_icccm_set_wm_size_hints (
            connection,
            m_NativeWindow->windowHandle,
            XCB_ATOM_WM_NORMAL_HINTS,
            &hints
        );

        xcb_flush (connection);
        m_Properties.minSize = size;
    }

    float32 WindowInternal::GetDpiScale() const {
        NATIVE_WINDOW_IS_VALID(0.0f);
        return 1.0f;
    }

    Vector2 WindowInternal::ConvertPixelToDpi(const Vector2& pixel) const {
        NATIVE_WINDOW_IS_VALID({});
        Vector2 pixel_(pixel);
        return pixel_ / GetDpiScale();
    }

    Vector2u WindowInternal::CurrentDisplaySize() const {
        NATIVE_WINDOW_IS_VALID({});
        return {};
    }

    Vector2i WindowInternal::CurrentDisplayPosition() const {
        NATIVE_WINDOW_IS_VALID({});
        return Vector2i();
    }

    void WindowInternal::ShowMouse(bool show) {
        NATIVE_WINDOW_IS_VALID();
    }

    void WindowInternal::SetMouseCursorGrabbed(bool grabbed) {
        NATIVE_WINDOW_IS_VALID();
    }

    void WindowInternal::SetMouseCursor(const WindowCursor& cursor) {
        NATIVE_WINDOW_IS_VALID();
    }

    bool WindowInternal::IsMouseInside() {
        NATIVE_WINDOW_IS_VALID(false);
        return m_NativeWindow == nullptr ? false :(*m_NativeWindow).isMouseInside;
    }

    Color WindowInternal::GetBackgroundColor() {
        NATIVE_WINDOW_IS_VALID(0x00000000);
        return m_NativeWindow == nullptr ? 0x00000000 : m_Properties.backgroundColor;
    }

    void WindowInternal::SetBackgroundColor(const Color& color) {
        NATIVE_WINDOW_IS_VALID();
    }

    void WindowInternal::Minimize() {
        NATIVE_WINDOW_IS_VALID();
    }

    void WindowInternal::Maximize() {
        NATIVE_WINDOW_IS_VALID();
    }

    void WindowInternal::Close() {
        NATIVE_WINDOW_IS_VALID();
        if (m_IsWindowClosed == false){
            /* if window was created then destroy it */
            xcb_window_t id_window = m_NativeWindow->windowHandle; 
            xcb_destroy_window (connection, m_NativeWindow->windowHandle);
            windowHandleMap.erase(m_NativeWindow->windowHandle);
            m_IsWindowClosed = true;
        }
    }

    bool WindowInternal::IsClosed() {
        NATIVE_WINDOW_IS_VALID(true);
        return m_IsWindowClosed;
    }

    bool WindowInternal::IsOpen() {
        NATIVE_WINDOW_IS_VALID(false);
        return !IsClosed();
    }

    void WindowInternal::Show() {
        NATIVE_WINDOW_IS_VALID();
        if (!m_Properties.visible){
            m_Properties.visible = true;
            xcb_map_window(connection, m_NativeWindow->windowHandle);
            xcb_flush(connection);
        }
    }

    void WindowInternal::Hide() {
        NATIVE_WINDOW_IS_VALID();
        if (m_Properties.visible){
            xcb_unmap_window (connection, m_NativeWindow->windowHandle);
            xcb_flush(connection);
        }
    }

    void WindowInternal::SetIcon(const Vector2u& size, const uint8* pixels) {
        NATIVE_WINDOW_IS_VALID();
    }

    Memory::Shared<WindowDisplay> WindowInternal::GetWindowDisplay() {
        NATIVE_WINDOW_IS_VALID(nullptr);
        return m_NativeWindow;
    }

    const WindowProperties& WindowInternal::GetProperties() {
        NATIVE_WINDOW_IS_VALID(WindowProperties::GetInValid());
        if (m_NativeWindow != nullptr) {
            return m_Properties;
        }
        return WindowProperties::GetInValid();
    }

    uint64 WindowInternal::ID() {
        NATIVE_WINDOW_IS_VALID(0);
        return m_WindowID;
    }

    void WindowInternal::Clear(const Color& color) {
        NATIVE_WINDOW_IS_VALID();
    }

    void WindowInternal::Swapbuffer() {
        NATIVE_WINDOW_IS_VALID();
        xcb_flush(PlatformState.connection);
    }

    void WindowInternal::SetProgress(float32 progress) {
        NATIVE_WINDOW_IS_VALID();
    }

    void WindowInternal::SetAlwaysOnTop(bool alwaysOnTop) {
        NATIVE_WINDOW_IS_VALID();
    }

    void WindowInternal::AcceptDragAndDrop(bool accept) {
        NATIVE_WINDOW_IS_VALID();
    }

    void WindowInternal::SetOpacity(float32 opacity) {
        NATIVE_WINDOW_IS_VALID();
    }

    bool WindowInternal::IsCreate() {
        NATIVE_WINDOW_IS_VALID(false);
        return m_IsWindowCreated;
    }

    void WindowInternal::SetBorder(uint32 border) {
        NATIVE_WINDOW_IS_VALID();
        XCBStateTools& stateTools = PlatformState.stateTools;

        if (!stateTools._MOTIF_WM_HINTS){
            Log_nts.Warning("Cannot change window decoration. _MOTIF_WM_HINTS atom not available. This means your "
            "window manager does not allow me to remove my window decoration");
            return;
        }

        struct {
            unsigned long flags;
            unsigned long functions;
            unsigned long decorations;
            long          input_mode;
            unsigned long status;
        } MWMHints = {(1L << 1), 0, border & 1, 0, 0};

        xcb_change_property (
            stateTools.connection,
            XCB_PROP_MODE_REPLACE,
            m_NativeWindow->windowHandle,
            stateTools._MOTIF_WM_HINTS,
            stateTools._MOTIF_WM_HINTS,
            32,
            sizeof (MWMHints) / sizeof (long),
            &MWMHints
        );
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
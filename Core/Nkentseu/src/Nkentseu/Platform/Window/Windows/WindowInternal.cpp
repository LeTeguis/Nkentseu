//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 9:55:04 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowInternal.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include <Nkentseu/Core/NkentseuLogger.h>
#include <Ntsm/Random.h>
#include "Nkentseu/Platform/PlatformState.h"

#include "dwmapi.h"
#include <windowsx.h>

#include <hidsdi.h>
#include <Dbt.h>
#pragma comment(lib, "dwmapi.lib")

namespace nkentseu {

    uint64 WindowInternal::s_WindowIDCounter = 0;
    const GUID guidDevinterfaceHid = { 0x4d1e55b2, 0xf16f, 0x11cf, {0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30} };

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

        currentWindowInternal = this;

        m_NativeWindow->instanceHandle = PlatformState.HInstance;
        HINSTANCE hPrevInstance = PlatformState.HPrevInstance;
        LPSTR lpCmdLine = PlatformState.LPCmdLine;
        int32 nCmdShow = PlatformState.NCmdShow;

        m_NativeWindow->backgroundColor.SetColor(m_Properties.backgroundColor);

        if (!m_Properties.fullscreen) {
            //InitWindowPosition(m_Properties.position, m_Properties.size, m_Properties.positionType);
        }

        Vector2i position = m_Properties.position;
        Vector2u size = m_Properties.size;// +m_WinFrame;

        if (!m_NativeWindow->Register(m_Properties.doubleClick, m_Properties)) {
            /**
                * Either an OS Error or a window with the same "name" id will cause
                * this to fail:
                */
            ErrorMessaging.PushError(NTSErrorCode::Window_RegisterWindowClass);
            return;
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

        m_NativeWindow->windowHandle = CreateWindowEx(0,
            m_NativeWindow->GetWindowClassName(), title.c_str(),
            dwStyle,
            windowRect.left, windowRect.top,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            NULL, NULL,
            m_NativeWindow->instanceHandle, NULL);

        if (m_NativeWindow->windowHandle == nullptr) {
            // Failed to create window...
            ErrorMessaging.PushError(NTSErrorCode::Window_Create);
            return;
        }

        GetCurrent(m_NativeWindow->windowHandle);

        if (!m_Properties.fullscreen) {
            InitWindowPosition(Vector2i(windowRect.left, windowRect.top), Vector2u(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top), m_Properties.positionType);
        }

        /*if (!m_NativeWindow->CreateHDC()) {
            ErrorMessaging.PushError(NTSErrorCode::Window_Context_Graphics);
            return;
        }*/

        BOOL isNCRenderingEnabled{ TRUE };
        DwmSetWindowAttribute(m_NativeWindow->windowHandle, DWMWA_NCRENDERING_ENABLED, &isNCRenderingEnabled, sizeof(isNCRenderingEnabled));

        if (m_Properties.visible) {
            ShowWindow(m_NativeWindow->windowHandle, SW_SHOW);
            SetForegroundWindow(m_NativeWindow->windowHandle);
            SetFocus(m_NativeWindow->windowHandle);
        }

        // Register to receive device interface change notifications (used for joystick connection handling)
        DEV_BROADCAST_DEVICEINTERFACE deviceInterface =
        { sizeof(DEV_BROADCAST_DEVICEINTERFACE), DBT_DEVTYP_DEVICEINTERFACE, 0, guidDevinterfaceHid, {0} };
        RegisterDeviceNotification(m_NativeWindow->windowHandle, &deviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);

        static const DWM_BLURBEHIND blurBehind{ {0}, {TRUE}, {NULL}, {TRUE} };
        DwmEnableBlurBehindWindow(m_NativeWindow->windowHandle, &blurBehind);
        static const MARGINS shadow_state[2]{ {0, 0, 0, 0}, {1, 1, 1, 1} };
        DwmExtendFrameIntoClientArea(m_NativeWindow->windowHandle, &shadow_state[0]);

        RegisterWindowMessage(L"TaskbarButtonCreated");
        HRESULT hrf = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
            IID_ITaskbarList3, (LPVOID*)&m_NativeWindow->taskbarList);
        SetProgress(0.0f);
        m_IsWindowCreated = true;

        UpdateWindow(m_NativeWindow->windowHandle);
    }

    WindowInternal::~WindowInternal() {
        if (IsValidDisplay()) {
            Close();
        }
    }

    void WindowInternal::InitWindowPosition(const Vector2i& position, const Vector2u& size, WindowPositionType positionType) {
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
        SetWindowPos(m_NativeWindow->windowHandle, 0, pos_win.x, pos_win.y, size_win.width, size_win.height, 0);
    }

    /*void WindowInternal::InitWindowPosition(const Vector2i& position, const Vector2u& size, WindowPositionType positionType) {
        // Adjust size to match DPI (avoid division by zero)
        float32 scaleFactor = GetDpiScale();
        if (scaleFactor == 0.0f) {
            // Handle case where DPI cannot be retrieved (use default or log error)
            scaleFactor = 1.0f;
        }
        Vector2i size_win = static_cast<Vector2i>(size * scaleFactor);

        // Don't scale position for non-centered placement
        Vector2i pos_win = position;

        if (positionType == WindowPositionType::CenteredPosition) {
            int32 screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int32 screenHeight = GetSystemMetrics(SM_CYSCREEN);
            if (screenWidth > 0 && screenHeight > 0) { // Handle potential errors
                pos_win.x = (screenWidth - size_win.width) / 2;
                pos_win.y = (screenHeight - size_win.height) / 2;
            }
        }
        else if (positionType == WindowPositionType::RandomPosition) {
            int32 maxPositionX = GetSystemMetrics(SM_CXSCREEN) - size_win.width;
            int32 maxPositionY = GetSystemMetrics(SM_CYSCREEN) - size_win.height;

            // Assuming Random provides a function for random integers within a range:
            pos_win.x = Random.NextInt32(0, maxPositionX);  // Random between 0 (inclusive) and maxPositionX (exclusive)
            pos_win.y = Random.NextInt32(0, maxPositionY);  // Random between 0 (inclusive) and maxPositionY (exclusive)
        }

        m_Properties.position = pos_win;
        m_Properties.size = size_win;

        // Set window position and size using actual pixel values
        SetWindowPos(m_NativeWindow->windowHandle, 0, pos_win.x, pos_win.y, size_win.width, size_win.height, SWP_NOZORDER | SWP_NOACTIVATE);
    }*/


    RECT WindowInternal::UpdateWindowExtension(const Vector2i& position, const Vector2u& size, DWORD style, DWORD styleEx) {
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

    // Fonction pour convertir les types Vector2u et Vector2i de votre projet en types Windows
    DWORD WindowInternal::GetWindowStyleInternal(const nkentseu::WindowProperties& props) {
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

    DWORD WindowInternal::GetWindowExStyleInternal(const nkentseu::WindowProperties& props) {
        DWORD exStyle = 0;
        // Plein ecran
        if (props.fullscreen) {
            exStyle |= WS_EX_APPWINDOW;
        }
        else {
            // Ajouter des styles etendus supplementaires pour les fen�tres non plein ecran si necessaire
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

    std::string WindowInternal::GetTitle() const {
        if (IsValidDisplay()) {
            char str[1024];
            memset(str, 0, sizeof(char) * 1024);
            GetWindowTextA(m_NativeWindow->windowHandle, str, 1024);
            std::string outStr = std::string(str);
            return outStr;
        }
        return "";
    }

    void WindowInternal::SetTitle(std::string title) {
        if (IsValidDisplay()) {
            m_Properties.title = title;
            std::wstring wtitle(title.begin(), title.end());
            SetWindowText(m_NativeWindow->windowHandle, wtitle.c_str());
        }
    }

    Vector2i WindowInternal::GetPosition() const {
        if (IsValidDisplay()) {
            return m_Properties.position;
        }
        return {};
    }

    void WindowInternal::SetPosition(int32 x, int32 y) {
        if (IsValidDisplay()) {
            SetWindowPos(m_NativeWindow->windowHandle, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            m_Properties.position = Vector2i(x, y);
        }
    }

    void WindowInternal::SetPosition(const Vector2i& pos) { SetPosition(pos.x, pos.y); }

    Vector2u WindowInternal::GetSize() {
        if (IsValidDisplay()) {
            return m_Properties.size;
        }
        return Vector2u();
    }

    void WindowInternal::SetSize(uint32 width, uint32 height) {
        if (IsValidDisplay()) {
            RECT rect, frame, border;
            GetWindowRect(m_NativeWindow->windowHandle, &rect);
            DwmGetWindowAttribute(m_NativeWindow->windowHandle, DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(RECT));

            border.left = frame.left - rect.left;
            border.top = frame.top - rect.top;
            border.right = rect.right - frame.right;
            border.bottom = rect.bottom - frame.bottom;

            int32 titlebarHeight = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER));

            SetWindowPos(m_NativeWindow->windowHandle, nullptr,
                -1, -1,
                width + border.right + border.left, // + m_WinFrame.w,
                height + border.top + border.bottom + titlebarHeight, // + m_WinFrame.height,
                SWP_NOMOVE | SWP_NOREDRAW);
            m_Properties.size = Vector2u(width, height);
        }
    }

    void WindowInternal::SetSize(const Vector2u& size) { SetSize(size.width, size.height); }

    float32 WindowInternal::GetDpiScale() const {
        if (IsValidDisplay()) {
            float32 currentDpi = static_cast<float32>(GetDpiForWindow(m_NativeWindow->windowHandle));
            float32 defaultDpi = USER_DEFAULT_SCREEN_DPI;

            return currentDpi / defaultDpi;
        }
        return 0.0f;
    }

    Vector2 WindowInternal::ConvertPixelToDpi(const Vector2& pixel) const {
        Vector2 pixel_(pixel);
        return pixel_ * GetDpiScale();
    }

    Vector2u WindowInternal::CurrentDisplaySize() const {
        if (IsValidDisplay()) {
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
            return Vector2u(static_cast<uint32>(screenWidth), static_cast<uint32>(screenHeight));
        }
        return {};
    }

    Vector2i WindowInternal::CurrentDisplayPosition() const {
        if (IsValidDisplay()) {
            WINDOWPLACEMENT lpwndpl = { 0 };
            GetWindowPlacement(m_NativeWindow->windowHandle, &lpwndpl);
            return Vector2i(lpwndpl.ptMinPosition.x, lpwndpl.ptMinPosition.y);
        }
        return Vector2i();
    }

    void WindowInternal::ShowMouse(bool show) {
        if (IsValidDisplay()) {
            // ShowCursor(show ? TRUE : FALSE);
            m_NativeWindow->isCursorVisible = show;
            SetCursor(m_NativeWindow->isCursorVisible ? m_NativeWindow->lastCursor : nullptr);
        }
    }

    void WindowInternal::SetMouseCursorGrabbed(bool grabbed) {
        if (IsValidDisplay()) {
            m_NativeWindow->isCursorGrabbed = grabbed;
            GrabWindowCursor(m_NativeWindow->isCursorGrabbed);
        }
    }

    void WindowInternal::SetMouseCursor(const WindowCursor& cursor) {
        if (IsValidDisplay()) {
            m_NativeWindow->lastCursor = static_cast<HCURSOR>(cursor.GetCursor());
            SetCursor(m_NativeWindow->isCursorVisible ? m_NativeWindow->lastCursor : nullptr);
        }
    }

    bool WindowInternal::IsMouseInside() {
        return m_NativeWindow == nullptr ? false : m_NativeWindow->isMouseInside;
    }

    Color WindowInternal::GetBackgroundColor() {
        return m_NativeWindow == nullptr ? 0x00000000 : m_Properties.backgroundColor;
    }

    void WindowInternal::SetBackgroundColor(const Color& color) {
        if (m_NativeWindow != nullptr) {
            m_Properties.backgroundColor = color;
#if defined(NKENTSUU_GAPI_SOFTWARE)
            m_NativeWindow->backgroundColor.SetColor(color);
#endif
        }
    }

    void WindowInternal::Minimize() {
        if (IsValidDisplay()) {
            ShowWindow(m_NativeWindow->windowHandle, SW_MINIMIZE);
        }
    }

    void WindowInternal::Maximize() {
        if (IsValidDisplay()) {
            if (!IsZoomed(m_NativeWindow->windowHandle)) {
                ShowWindow(m_NativeWindow->windowHandle, SW_MAXIMIZE);
            }
            else {
                ShowWindow(m_NativeWindow->windowHandle, SW_RESTORE);
            }
        }
    }

    void WindowInternal::Close() {
        if (IsValidDisplay()) {
            if (m_NativeWindow->icon)
                DestroyIcon(m_NativeWindow->icon);

            m_NativeWindow->Unregister();

            DestroyWindow(m_NativeWindow->windowHandle);

            auto it = windowHandleMap.find(m_NativeWindow->windowHandle);
            if (it != windowHandleMap.end()) {
                windowHandleMap.erase(it);
            }

            m_NativeWindow->windowHandle = nullptr;
            Memory::Reset(m_NativeWindow);
            m_IsWindowClosed = true;
        }
    }

    bool WindowInternal::IsClosed() {
        return m_IsWindowClosed;
    }

    bool WindowInternal::IsOpen() {
        return !IsClosed();
    }

    void WindowInternal::Show() {
        if (!IsValidDisplay() || m_Properties.visible) {
            return;
        }

        m_Properties.visible = true;

        ShowWindow(m_NativeWindow->windowHandle, SW_SHOW);
        SetForegroundWindow(m_NativeWindow->windowHandle);
        SetFocus(m_NativeWindow->windowHandle);
    }

    void WindowInternal::Hide() {
        if (!IsValidDisplay() || !m_Properties.visible) {
            return;
        }

        m_Properties.visible = false;

        ShowWindow(m_NativeWindow->windowHandle, SW_HIDE);
        SetForegroundWindow(m_NativeWindow->windowHandle);
        SetFocus(m_NativeWindow->windowHandle);
    }

    void WindowInternal::SetIcon(const Vector2u& size, const uint8* pixels) {
        if (IsValidDisplay()) {
            // First destroy the previous one
            if (m_NativeWindow->icon)
                DestroyIcon(m_NativeWindow->icon);

            // Windows wants BGRA pixels: swap red and blue channels
            std::vector<std::uint8_t> iconPixels(size.x * size.y * 4);
            for (std::size_t i = 0; i < iconPixels.size() / 4; ++i) {
                iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
                iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
                iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
                iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
            }

            // Create the icon from the pixel array
            m_NativeWindow->icon = CreateIcon(GetModuleHandleW(nullptr),
                static_cast<int>(size.x),
                static_cast<int>(size.y),
                1,
                32,
                nullptr,
                iconPixels.data());

            // Set it as both big and small icon of the window
            if (m_NativeWindow->icon) {
                SendMessageW(m_NativeWindow->windowHandle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(m_NativeWindow->icon));
                SendMessageW(m_NativeWindow->windowHandle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(m_NativeWindow->icon));
            }
            else {
                Log_nts.Error("Failed to set the window's icon");
            }
        }
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
#if defined(NKENTSEU_GAPI_SOFTWARE)
        if (m_NativeWindow == nullptr) {
            return;
        }
        m_Properties.BackgroundColor = color;
#endif
    }

    void WindowInternal::Swapbuffer() {
#if defined(NKENTSEU_GAPI_SOFTWARE)
        if (!NATIVE_WIN32_WINDOW_IS_VALID(windowHandle)) {
            return;
        }
        InvalidateRect(m_NativeWindow->windowHandle, NULL, TRUE);
        UpdateWindow(m_NativeWindow->windowHandle);
#endif
    }

    void WindowInternal::SetProgress(float32 progress) {
        if (m_NativeWindow == nullptr) {
            return;
        }
        if (IsValidDisplay() && m_NativeWindow->taskbarList != nullptr) {
            unsigned max = 10000;
            unsigned cur = (unsigned)(progress * (float32)max);
            m_NativeWindow->taskbarList->SetProgressValue(m_NativeWindow->windowHandle, cur, max);
        }
    }

    WindowInternal* WindowInternal::GetCurrent(HWND hwnd) {
        WindowInternal* _this;
        if (currentWindowInternal != nullptr) {
            windowHandleMap.emplace(hwnd, currentWindowInternal);
            currentWindowInternal->m_NativeWindow->windowHandle = hwnd;
            _this = currentWindowInternal;
            currentWindowInternal = nullptr;
        }
        else {
            auto existing = windowHandleMap.find(hwnd);
            _this = existing->second;
        }
        return _this;
    }

    void WindowInternal::SetAlwaysOnTop(bool alwaysOnTop) {
        if (!IsValidDisplay()) {
            return;
        }
        if (alwaysOnTop) {
            SetWindowPos(m_NativeWindow->windowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }
        else {
            SetWindowPos(m_NativeWindow->windowHandle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }
    }

    void WindowInternal::AcceptDragAndDrop(bool accept) {
        if (!IsValidDisplay()) {
            return;
        }
        DragAcceptFiles(m_NativeWindow->windowHandle, accept ? TRUE : FALSE);
    }

    void WindowInternal::SetOpacity(float32 opacity) {
        if (!IsValidDisplay()) {
            return;
        }
        const HWND hwnd = m_NativeWindow->windowHandle;
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

    bool WindowInternal::IsCreate() {
        return m_IsWindowCreated;
    }

    void WindowInternal::GrabWindowCursor(bool grabbed) {
        if (!IsValidDisplay()) {
            return;
        }

        if (grabbed) {
            RECT rect;
            GetClientRect(m_NativeWindow->windowHandle, &rect);
            MapWindowPoints(m_NativeWindow->windowHandle, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
            ClipCursor(&rect);
        }
        else {
            ClipCursor(nullptr);
        }
    }

    bool WindowInternal::IsValidDisplay() const {
        return m_NativeWindow != nullptr && m_NativeWindow->windowHandle != 0;
    }

}    // namespace nkentseu

#endif
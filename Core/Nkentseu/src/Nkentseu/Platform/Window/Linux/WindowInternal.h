//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 9:54:56 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once

#include "System/System.h"

#ifdef NKENTSEU_PLATFORM_LINUX


#ifndef __NKENTSEU_WINDOW_INTERNAL_H__
#define __NKENTSEU_WINDOW_INTERNAL_H__

#include "System/Definitions/Memory.h"

#include <Nkentseu/Core/WindowProperties.h>
#include <Nkentseu/ErrorInfos/NkentseuError.h>
#include <Nkentseu/Core/WindowCursor.h>
#include "WindowDisplay.h"

namespace nkentseu {
    class Window;
    
    class NKENTSEU_API WindowInternal
    {
    public:
        WindowInternal();
        WindowInternal(class Window*, const WindowProperties& properties);
        ~WindowInternal();

        virtual std::string GetTitle() const;
        virtual void SetTitle(std::string title);

        virtual Vector2i GetPosition() const;
        virtual void SetPosition(int32 x, int32 y);
        virtual void SetPosition(const Vector2i& pos);

        virtual Vector2u GetSize();
        virtual void SetSize(uint32 width, uint32 height);
        virtual void SetSize(const Vector2u& size);

        virtual float32 GetDpiScale() const;

        Vector2 ConvertPixelToDpi(const Vector2& pixel) const;

        virtual Vector2u CurrentDisplaySize() const;
        virtual Vector2i CurrentDisplayPosition() const;

        virtual void ShowMouse(bool show);
        virtual void SetMouseCursorGrabbed(bool grabbed);
        virtual void SetMouseCursor(const WindowCursor& cursor);

        virtual bool IsMouseInside();

        virtual Color GetBackgroundColor();
        virtual void SetBackgroundColor(const Color& color);

        virtual void Minimize();
        virtual void Maximize();

        virtual void Close();
        virtual bool IsClosed();
        virtual bool IsOpen();

        virtual void Show();
        virtual void Hide();

        virtual void SetIcon(const Vector2u& size, const uint8* pixels);

        Memory::Shared<WindowDisplay> GetWindowDisplay();

        virtual const WindowProperties& GetProperties();

        virtual uint64 ID();

        virtual void Clear(const Color& color);
        virtual void Swapbuffer();

        virtual void SetAlwaysOnTop(bool alwaysOnTop);
        virtual void AcceptDragAndDrop(bool accept);
        virtual void SetOpacity(float32 opacity);

        virtual bool IsCreate();
        void SetProgress(float32 progress);

        static WindowInternal* GetCurrent(xcb_window_t window);

    private:
        friend class WindowEventInternal;
        
        Vector2u m_WindowFrameSize = Vector2u(6, 30);
        Memory::Shared<WindowDisplay> m_NativeWindow = nullptr;
        bool m_IsWindowClosed = false;
        bool m_IsEventPick = false;
        uint64 m_WindowID = 0;
        bool m_IsWindowCreated = false;
        WindowProperties m_Properties;

        class Window* m_MainWindow = nullptr;

        static uint64 s_WindowIDCounter;

        void SetMinSize(const Vector2u& size);
        void SetMaxSize(const Vector2u& size);
        void SetBorder(uint32 border);
    };

    static thread_local WindowInternal* currentWindowInternal = nullptr;
    static thread_local std::unordered_map<xcb_window_t, WindowInternal*> windowHandleMap = {};
} // namespace nkentseu

#endif

#endif    // __NKENTSEU_LINUX_WINDOW_INTERNAL_H__
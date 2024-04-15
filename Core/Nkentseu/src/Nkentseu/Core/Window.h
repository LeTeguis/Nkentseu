//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 5:04:02 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_WINDOW_H__
#define __NKENTSEU_WINDOW_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include <Ntsm/Vector/Vector2.h>
#include <System/Definitions/Memory.h>

#include "WindowProperties.h"
#include "WindowCursor.h"
#include "Nkentseu/ErrorInfos/NkentseuError.h"

namespace nkentseu {
    class WindowInternal;

    struct WindowInternalInfo {
        Memory::Shared<WindowInternal> InternalInfo;
        bool IsLoad;
    };

    class NKENTSEU_API Window {
    public:
        Window();
        Window(const WindowProperties& properties);
        NTSErrorCode Initialize(const WindowProperties& properties);
        ~Window();

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

        virtual const WindowProperties& GetProperties();

        virtual uint64 ID();

        virtual void Clear(const Color& color);
        virtual void Swapbuffer();

        virtual void SetAlwaysOnTop(bool alwaysOnTop);
        virtual void AcceptDragAndDrop(bool accept);
        virtual void SetOpacity(float32 opacity);

        virtual WindowInternal* GetWindowInternal();

    private:
        Memory::Shared<WindowInternalInfo> m_WindowInternalInfo;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_H__
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
    struct WindowData;

    class NKENTSEU_API Window {
    public:

        static Memory::Shared<Window> Create(const WindowProperties& properties);

        virtual std::string GetTitle() const = 0;
        virtual void SetTitle(std::string title) = 0;

        virtual Vector2i GetPosition() const = 0;
        virtual void SetPosition(int32 x, int32 y) = 0;
        virtual void SetPosition(const Vector2i& pos) = 0;

        virtual Vector2u GetSize() = 0;
        virtual void SetSize(uint32 width, uint32 height) = 0;
        virtual void SetSize(const Vector2u& size) = 0;

        virtual float32 GetDpiScale() const = 0;
        virtual Vector2 ConvertPixelToDpi(const Vector2& pixel) const = 0;

        virtual float32 GetDpiAspect() = 0;
        virtual float32 GetAspect() = 0;

        virtual Vector2u CurrentDisplaySize() const = 0;
        virtual Vector2i CurrentDisplayPosition() const = 0;

        virtual void ShowMouse(bool show) = 0;
        virtual void SetMouseCursorGrabbed(bool grabbed) = 0;
        virtual void SetMouseCursor(const WindowCursor& cursor) = 0;

        virtual bool IsMouseInside() = 0;

        virtual Color GetBackgroundColor() = 0;
        virtual void SetBackgroundColor(const Color& color) = 0;

        virtual void Minimize() = 0;
        virtual void Maximize() = 0;

        virtual void Close() = 0;
        virtual bool IsClosed() = 0;
        virtual bool IsOpen() = 0;

        virtual void Show() = 0;
        virtual void Hide() = 0;

        virtual void SetIcon(const Vector2u& size, const uint8* pixels) = 0;

        virtual const WindowProperties& GetProperties() = 0;

        virtual uint64 ID() = 0;

        virtual void Clear(const Color& color) = 0;
        virtual void Swapbuffer() = 0;

        virtual void SetAlwaysOnTop(bool alwaysOnTop) = 0;
        virtual void AcceptDragAndDrop(bool accept) = 0;
        virtual void SetOpacity(float32 opacity) = 0;

        virtual WindowData* GetData() = 0;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_H__
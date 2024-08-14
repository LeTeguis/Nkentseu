//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-12 at 05:16:22 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __WIN32_WINDOW_H__
#define __WIN32_WINDOW_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include "Nkentseu/Core/Window.h"
#include "System/Definitions/Memory.h"
#include "Nkentseu/Platform/Window/WindowData.h"

namespace nkentseu {
    
    class NKENTSEU_API Win32Window : public Window {
        public:
            Win32Window(const WindowProperties& properties);

            std::string GetTitle() const override;
            void SetTitle(std::string title) override;

            maths::Vector2i GetPosition() const override;
            void SetPosition(int32 x, int32 y) override;
            void SetPosition(const maths::Vector2i& pos) override;

            maths::Vector2u GetSize() override;
            void SetSize(uint32 width, uint32 height) override;
            void SetSize(const maths::Vector2u& size) override;

            float32 GetDpiScale() const override;
            maths::Vector2f ConvertPixelToDpi(const maths::Vector2f& pixel) const override;

            float32 GetDpiAspect() override;
            float32 GetAspect() override;

            maths::Vector2u CurrentDisplaySize() const override;
            maths::Vector2i CurrentDisplayPosition() const override;

            void ShowMouse(bool show) override;
            void ConfineMouse(bool confine) override;
            void SetMouseCursorGrabbed(bool grabbed) override;
            void SetMouseCursor(const WindowCursor& cursor) override;

            bool IsMouseInside() override;

            Color GetBackgroundColor() override;
            void SetBackgroundColor(const Color& color) override;

            void Minimize() override;
            void Maximize() override;

            void Close() override;
            bool IsClosed() override;
            bool IsOpen() override;

            void Show() override;
            void Hide() override;

            void SetIcon(const maths::Vector2u& size, const uint8* pixels) override;

            const WindowProperties& GetProperties() override;
            WindowProperties& GetProps();

            uint64 ID() override;

            void Clear(const Color& color) override;
            void Swapbuffer() override;

            void SetAlwaysOnTop(bool alwaysOnTop) override;
            void AcceptDragAndDrop(bool accept) override;
            void SetOpacity(float32 opacity) override;

            WindowData* GetData() override;
            bool IsCreate();

            static Win32Window* GetCurrent(HWND hwnd);

            void SetProgress(float32 progress);
        private:

            Memory::Shared<WindowData> m_Data = nullptr;
            WindowProperties m_Properties;
            bool m_IsWindowCreated = false;
            bool m_IsWindowClosed = true;
            bool m_CursorIsEnabled = true;
            static uint64 s_WindowIDCounter;
            uint64 m_WindowID;

            bool CreateWin32Window();
            bool IsValidDisplay() const;

            // Méthodes pour convertir les types Vector2u et Vector2i en types Windows
            DWORD GetWindowStyleInternal(const WindowProperties& props);
            DWORD GetWindowExStyleInternal(const WindowProperties& props);

            // Ajuster la taille de la fenêtre
            RECT UpdateWindowExtension(const maths::Vector2i& position, const maths::Vector2u& size, DWORD style, DWORD styleEx);
            void InitWindowPosition(const maths::Vector2i& position, const maths::Vector2u& size, WindowPositionType positionType);
            void InitWindowPosition(DWORD style, DWORD styleEx, const maths::Vector2f& desiredPos, const maths::Vector2f& desiredSize);

            // Méthode pour capturer le curseur
            void GrabWindowCursor(bool grabbed);
    };

    static thread_local Win32Window* currentWindow = nullptr;
    static thread_local std::unordered_map<HWND, Win32Window*> windowMap = {};

}  //  nkentseu

#endif

#endif  // __WIN32_WINDOW_H__!
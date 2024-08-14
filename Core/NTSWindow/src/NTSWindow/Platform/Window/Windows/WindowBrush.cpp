//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 4:33:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "WindowBrush.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

namespace nkentseu {

    WindowBrush::WindowBrush(const maths::Color& c) : m_Color(c), m_BrushHandle(CreateSolidBrush(RGB(m_Color.r, m_Color.g, m_Color.b))) { }

    WindowBrush::~WindowBrush() {
        if (m_BrushHandle != nullptr) {
            DeleteObject(m_BrushHandle);
        }
    }

    void WindowBrush::SetColor(const maths::Color& c) {
        m_Color = c;

        if (m_BrushHandle != nullptr) {
            DeleteObject(m_BrushHandle);
        }

        m_BrushHandle = CreateSolidBrush(RGB(m_Color.r, m_Color.g, m_Color.b));
    }

    maths::Color WindowBrush::GetColor() {
        return m_Color;
    }

    HBRUSH WindowBrush::GetBrushHandle() {
        return m_BrushHandle;
    }

    WindowBrush::WindowBrush() : m_BrushHandle(CreateSolidBrush(RGB(23, 23, 23))), m_Color(23 / 255.0f, 23 / 255.0f, 23 / 255.0f) {}

}    // namespace nkentseu

#endif
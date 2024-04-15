//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 4:33:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowBrush.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

namespace nkentseu {

    WindowBrush::WindowBrush(const Color& c) : m_Color(c), m_BrushHandle(CreateSolidBrush(RGB(m_Color.R(), m_Color.G(), m_Color.B()))) { }

    WindowBrush::~WindowBrush() {
        if (m_BrushHandle != nullptr) {
            DeleteObject(m_BrushHandle);
        }
    }

    void WindowBrush::SetColor(const Color& c) {
        m_Color = c;

        if (m_BrushHandle != nullptr) {
            DeleteObject(m_BrushHandle);
        }

        m_BrushHandle = CreateSolidBrush(RGB(m_Color.R(), m_Color.G(), m_Color.B()));
    }

    Color WindowBrush::GetColor() {
        return m_Color;
    }

    HBRUSH WindowBrush::GetBrushHandle() {
        return m_BrushHandle;
    }

    WindowBrush::WindowBrush() : m_BrushHandle(CreateSolidBrush(RGB(23, 23, 23))), m_Color(23, 23, 23) {}

}    // namespace nkentseu

#endif
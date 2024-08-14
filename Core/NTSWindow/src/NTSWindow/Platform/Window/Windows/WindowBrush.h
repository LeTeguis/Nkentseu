//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 4:33:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once


#include "NTSCore/System.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS


#ifndef __NKENTSEU_WINDOW_BRUSH_H__
#define __NKENTSEU_WINDOW_BRUSH_H__

#include <Windows.h>
#include <ShObjIdl.h>
#include "NTSMaths/Color.h"

namespace nkentseu {

    // Class representing a window brush (Windows specific)
    class NKENTSEU_API WindowBrush {
    private:
        // Color of the brush
        maths::Color m_Color;

        // Handle to the native Windows brush object
        HBRUSH m_BrushHandle;

    public:
        // Constructor that takes a Color object to set the initial brush color
        WindowBrush(const maths::Color& color);

        // Destructor responsible for cleaning up the brush handle
        ~WindowBrush();

        // Sets the color of the brush and recreates the brush handle
        void SetColor(const maths::Color& color);

        // Returns the current color of the brush
        maths::Color GetColor();

        // Returns the handle to the native Windows brush object
        HBRUSH GetBrushHandle();

        // Default constructor (might be useful for specific use cases)
        WindowBrush();
    };
} // namespace nkentseu


#endif

#endif  // __NKENTSEU_WINDOW_BRUSH_H__

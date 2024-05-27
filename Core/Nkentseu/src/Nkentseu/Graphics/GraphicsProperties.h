//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 10:02:13 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_GRAPHICSPROPERTIES_H__
#define __NKENTSEU_GRAPHICSPROPERTIES_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include <Ntsm/Vector/Vector2.h>

#include "Nkentseu/Core/WindowProperties.h"
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    enum class NKENTSEU_API GraphicsBufferCount {
        Simple,
        Double,
        Triple
    };

    struct NKENTSEU_API GraphicsAttribute {
        using Attributes = ulong;
        
        enum : Attributes {
            AttributeDefault = 0,
            AttributeCore = 1 << 1,
            AttributeDebug = 1 << 2
        };
    };

    struct NKENTSEU_API GraphicsFlag {
        using Flags = ulong;

        enum : Flags {
            DrawToWindow = 1 << 1,
            DrawToBitmap = 1 << 2,
            SupportGDI = 1 << 3,
            SupportOpenGL = 1 << 4,
            GenericAccelerated = 1 << 5,
            GenericFormat = 1 << 6,
            NeedPalette = 1 << 7,
            NeedSystemPalette = 1 << 8,
            DoubleBuffer = 1 << 9,
            TripleBuffer = 1 << 10,
            Stereo = 1 << 11,
            SwapLayerBuffer = 1 << 12,
            DepthDontCare = 1 << 13,
            DoubleBufferDontCare = 1 << 14,
            StereoDontCare = 1 << 15,
            SwapExchange = 1 << 16,
            SwapCopy = 1 << 17,
            SupportDirectDraw = 1 << 18,
            Direct3DAccelerated = 1 << 19,
            SupportComposition = 1 << 20,
            TripeBufferDontCare = 1 << 21
        };
    };

    struct NKENTSEU_API GraphicsMainLayer {
        using MainLayer = uint8;

        enum : MainLayer {
            MainPlane,
            OverlayPlane,
            UnderlayPlane
        };
    };

    struct NKENTSEU_API GraphicsPixelType {
        using PixelType = ulong;

        enum : PixelType {
            RgbaArb = 1 << 1,
            RgbaExt = 1 << 2,
            RgbaFloatArb = 1 << 3,
            RgbaFloatAti = 1 << 4,
            RgbaUnsignedFloatExt = 1 << 5,
            ColorIndexArb = 1 << 6,
            ColorIndexExt = 1 << 7
        };
    };

    struct NKENTSEU_API GraphicsInfos {
        std::string vendor = "";
        std::string renderer = "";
        std::string version = "";
        std::string langageVersion = "";
        std::string extension = "";
        std::string profilMask = "";

        std::string ToString() const;

        friend std::string ToString(const GraphicsInfos& gi) {
            return gi.ToString();
        }

        // GraphicsInfos
        GraphicsInfos& operator=(const GraphicsInfos& other) = default;
        GraphicsInfos(const GraphicsInfos& other) = default;
        GraphicsInfos() = default;
    };

    struct NKENTSEU_API GraphicsPixelFormat {
        uint32 redColorBits = 8;
        uint32 greenColorBits = 8;
        uint32 blueColorBits = 8;
        uint32 alphaColorBits = 8;
        uint32 bitsPerPixel = 4;
        uint32 depthBits = 24; // 24
        uint32 stencilBits = 8;
        uint32 antialiasingLevel = 8;
        uint32 accumBits = 0;
        uint32 auxBuffers = 0;
        bool   pbuffer;

        //uint32 attributeFlags = (uint32)GraphicsAttribute::AttributeDefault;
        uint32 attributeFlags = (uint32)GraphicsAttribute::AttributeCore | (uint32)GraphicsAttribute::AttributeDebug;
        bool   sRgb = false;

        GraphicsFlag::Flags flags = GraphicsFlag::DrawToWindow | GraphicsFlag::DoubleBuffer | GraphicsFlag::SupportOpenGL;
        GraphicsMainLayer::MainLayer mainLayer = GraphicsMainLayer::MainPlane;
        GraphicsPixelType::PixelType pixelType = GraphicsPixelType::RgbaArb;

        // GraphicsPixelFormat
        GraphicsPixelFormat& operator=(const GraphicsPixelFormat& other) = default;
        GraphicsPixelFormat(const GraphicsPixelFormat& other) = default;
        GraphicsPixelFormat() = default;
    };

    struct NKENTSEU_API ContextProperties {
        Vector2u offScreenSize;
        GraphicsPixelFormat pixelFormat;
        Vector2i version = InitVersion();

        // ContextProperties
        ContextProperties& operator=(const ContextProperties& other);
        //ContextProperties(const ContextProperties& other) = default;

        ContextProperties();

        ContextProperties(Vector2u size, GraphicsPixelFormat format);

        ContextProperties(GraphicsPixelFormat format);

        ContextProperties(const ContextProperties& properties);

        static Vector2i InitVersion();
    };
} // namespace nkentseu

#endif    // __NKENTSEU_GRAPHICSPROPERTIES_H__
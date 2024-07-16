//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-13 at 09:24:44 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __GRAPHICS_ENUM_H__
#define __GRAPHICS_ENUM_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <System/Nature/Enumeration.h>

namespace nkentseu {

	ENUMERATION(ShaderStage, uint32,
		ENUM_TO_STRING_BEGIN
		ENUM_TO_STRING_CONTENT(Enum::Vertex)
		ENUM_TO_STRING_CONTENT(Enum::Fragment)
		ENUM_TO_STRING_CONTENT(Enum::Geometry)
		ENUM_TO_STRING_CONTENT(Enum::TesControl)
		ENUM_TO_STRING_CONTENT(Enum::TesEvaluation)
		ENUM_TO_STRING_CONTENT(Enum::Compute)
		ENUM_TO_STRING_END(Enum::NotDefine),
		,
		Vertex = 1 << 0,  // 0x01
		Fragment = 1 << 1,  // 0x02
		Geometry = 1 << 2,  // 0x04
		TesControl = 1 << 3,  // 0x08
		TesEvaluation = 1 << 4,  // 0x10
		Compute = 1 << 5,  // 0x20
		NotDefine
	);

	ENUMERATION(ShaderInternalType, uint32,
		ENUM_TO_STRING_BEGIN
		ENUM_TO_STRING_CONTENT(Enum::Boolean)
		ENUM_TO_STRING_CONTENT(Enum::Float)
		ENUM_TO_STRING_CONTENT(Enum::Float2)
		ENUM_TO_STRING_CONTENT(Enum::Float3)
		ENUM_TO_STRING_CONTENT(Enum::Float4)
		ENUM_TO_STRING_CONTENT(Enum::Int)
		ENUM_TO_STRING_CONTENT(Enum::Int2)
		ENUM_TO_STRING_CONTENT(Enum::Int3)
		ENUM_TO_STRING_CONTENT(Enum::Int4)
		ENUM_TO_STRING_CONTENT(Enum::Byte)
		ENUM_TO_STRING_CONTENT(Enum::Byte2)
		ENUM_TO_STRING_CONTENT(Enum::Byte3)
		ENUM_TO_STRING_CONTENT(Enum::Byte4)
		ENUM_TO_STRING_CONTENT(Enum::Mat2)
		ENUM_TO_STRING_CONTENT(Enum::Mat3)
		ENUM_TO_STRING_CONTENT(Enum::Mat4)
		ENUM_TO_STRING_END(Enum::NotDefine),
		static uint32 ComponentCount(ShaderInternalType type);
	static uint32 ComponentElementSize(ShaderInternalType type);
	static uint32 ComponentSize(ShaderInternalType type);
	,
		Boolean, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Byte, Byte2, Byte3, Byte4, Mat2, Mat3, Mat4, NotDefine
		);

	ENUMERATION(BufferUsageType, uint32,
		ENUM_TO_STRING_BEGIN
		ENUM_TO_STRING_CONTENT(Enum::DynamicDraw)
		ENUM_TO_STRING_CONTENT(Enum::StreamDraw)
		ENUM_TO_STRING_END(Enum::StaticDraw),
		,
		StaticDraw, DynamicDraw, StreamDraw
	);

	ENUMERATION(BufferSpecificationType, uint32,
		ENUM_TO_STRING_BEGIN
		ENUM_TO_STRING_CONTENT(Enum::Index)
		ENUM_TO_STRING_CONTENT(Enum::Uniform)
		ENUM_TO_STRING_CONTENT(Enum::Storage)
		ENUM_TO_STRING_CONTENT(Enum::Texture)
		ENUM_TO_STRING_CONTENT(Enum::Constant)
		ENUM_TO_STRING_END(Enum::Vertex),
		,
		Vertex, Index, Uniform, Storage, Texture, Constant
	);

	ENUMERATION(IndexBufferType, uint32,
		ENUM_TO_STRING_BEGIN
		ENUM_TO_STRING_CONTENT(Enum::UInt8)
		ENUM_TO_STRING_CONTENT(Enum::UInt16)
		ENUM_TO_STRING_CONTENT(Enum::UInt32)
		ENUM_TO_STRING_CONTENT(Enum::UInt64)
		ENUM_TO_STRING_END(Enum::NotDefine),
		static usize SizeOf(IndexBufferType indexType);
	,
		NotDefine, UInt8, UInt16, UInt32, UInt64
		);

	ENUMERATION(SamplerType, uint32,
		ENUM_TO_STRING_BEGIN
		ENUM_TO_STRING_CONTENT(Enum::SamplerImage)
		ENUM_TO_STRING_CONTENT(Enum::StorageImage)
		ENUM_TO_STRING_END(Enum::CombineImage),
		,
		CombineImage, SamplerImage, StorageImage
	);

    ENUMERATION(TextCord, uint32,
        ENUM_TO_STRING_BEGIN
        ENUM_TO_STRING_CONTENT(Enum::Pixels)
        ENUM_TO_STRING_END(Enum::Normalized),
        ,
        Normalized, //!< Texture coordinates in range [0 .. 1]
        Pixels      //!< Texture coordinates in range [0 .. size]
    );

    ENUMERATION(TextureFormat, uint32,
        ENUM_TO_STRING_BEGIN
        ENUM_TO_STRING_CONTENT(Enum::RGBA8)
        ENUM_TO_STRING_CONTENT(Enum::RGB8)
        ENUM_TO_STRING_CONTENT(Enum::SRGB8_ALPHA8)
        ENUM_TO_STRING_CONTENT(Enum::RED_INTEGER)
        ENUM_TO_STRING_CONTENT(Enum::DEPTH_COMPONENT16)
        ENUM_TO_STRING_CONTENT(Enum::DEPTH_COMPONENT24)
        ENUM_TO_STRING_CONTENT(Enum::DEPTH_COMPONENT32F)
        ENUM_TO_STRING_CONTENT(Enum::STENCIL_INDEX8)
        ENUM_TO_STRING_CONTENT(Enum::DEPTH24_STENCIL8)
        ENUM_TO_STRING_CONTENT(Enum::DEPTH32F_STENCIL8)
        ENUM_TO_STRING_END(Enum::None),
        // Verifier si un format est un format de couleur
        static bool IsColor(TextureFormat format);

		// Verifier si un format est un format de profondeur
		static bool IsDepth(TextureFormat format);

		// Verifier si un format est un format de stencil
		static bool IsStencil(TextureFormat format);
		,
        None = 0,

        // Formats de couleur
        RGBA8,                 // Couleur RGBA 8 bits non normalisee
        RGB8,                  // Couleur RGB 8 bits non normalisee
        SRGB8_ALPHA8,          // Couleur sRGB avec canal alpha 8 bits non normalisee
        RED_INTEGER,

        // Formats de profondeur
        DEPTH_COMPONENT16,     // Profondeur 16 bits non normalisee
        DEPTH_COMPONENT24,     // Profondeur 24 bits non normalisee
        DEPTH_COMPONENT32F,    // Profondeur 32 bits en virgule flottante

        // Formats de stencil
        STENCIL_INDEX8,        // Index de stencil 8 bits
        DEPTH24_STENCIL8,      // Profondeur 24 bits avec index de stencil 8 bits
        DEPTH32F_STENCIL8      // Profondeur 32 bits en virgule flottante avec index de stencil 8 bits
    );

	ENUMERATION(RenderPrimitive, uint32,
		ENUM_TO_STRING_BEGIN
		ENUM_TO_STRING_CONTENT(Enum::Points)
		ENUM_TO_STRING_CONTENT(Enum::Lines)
		ENUM_TO_STRING_CONTENT(Enum::LineStrip)
		ENUM_TO_STRING_CONTENT(Enum::Triangles)
		ENUM_TO_STRING_CONTENT(Enum::TriangleStrip)
		ENUM_TO_STRING_CONTENT(Enum::TriangleFan)
		ENUM_TO_STRING_CONTENT(Enum::Paths)
		ENUM_TO_STRING_END(Enum::Points),
		,
		Points,
		Lines,
		LineStrip,
		Triangles,
		TriangleStrip,
		TriangleFan,
		Paths
	);

	ENUMERATION(CullModeType, uint32,
		ENUM_TO_STRING_BEGIN
		ENUM_TO_STRING_CONTENT(Enum::Front)
		ENUM_TO_STRING_CONTENT(Enum::Back)
		ENUM_TO_STRING_CONTENT(Enum::FrontBack)
		ENUM_TO_STRING_END(Enum::NoCull),
		,
		Front, Back, FrontBack, NoCull
	);

	ENUMERATION(FrontFaceType, uint32,
		ENUM_TO_STRING_BEGIN
		ENUM_TO_STRING_CONTENT(Enum::CounterClockwise)
		ENUM_TO_STRING_END(Enum::Clockwise),
		,
		CounterClockwise, Clockwise
	);

	ENUMERATION(PolygonModeType, uint32,
		ENUM_TO_STRING_BEGIN
		ENUM_TO_STRING_CONTENT(Enum::Line)
		ENUM_TO_STRING_CONTENT(Enum::Point)
		ENUM_TO_STRING_CONTENT(Enum::FillRectangle)
		ENUM_TO_STRING_END(Enum::Fill),
		,
		Line, Fill, Point, FillRectangle
	);

}  //  nkentseu

#endif  // __GRAPHICS_ENUM_H__!
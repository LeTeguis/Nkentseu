//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:55:46 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenGLUtils.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
#include <Logger/Formatter.h>

namespace nkentseu {

    std::string OpenGLStaticDebugInfo::file_call = "";
    uint32 OpenGLStaticDebugInfo::line_call = 0;
    std::string OpenGLStaticDebugInfo::methode_call = "";
    bool OpenGLStaticDebugInfo::success = true;

    OpenGLResult glCheckError_(const std::string& file, int32 line, const std::string& function)
    {
        OpenGLResult result = {true, GL_NO_ERROR};
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (errorCode)
            {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }
            NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, function.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Error("type = {0}", error);

            if (result.success) {
                result.result = errorCode;
                result.success = errorCode == GL_NO_ERROR;
            }
        }
        return result;
    }

    uint32 GLConvert::ShaderType(ShaderDataType::Code type)
    {
        // NotDefine, Boolean, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Byte4, Mat3, Mat4, Struct
        switch (type) {
        case ShaderDataType::Float:    return GL_FLOAT;
        case ShaderDataType::Float2:   return GL_FLOAT;
        case ShaderDataType::Float3:   return GL_FLOAT;
        case ShaderDataType::Float4:   return GL_FLOAT;
        case ShaderDataType::Mat3:     return GL_FLOAT;
        case ShaderDataType::Mat4:     return GL_FLOAT;
        case ShaderDataType::Int:      return GL_INT;
        case ShaderDataType::Int2:     return GL_INT;
        case ShaderDataType::Int3:     return GL_INT;
        case ShaderDataType::Int4:     return GL_INT;
        case ShaderDataType::Boolean:     return GL_BOOL;
        }

        return 0;
    }

    uint32 GLConvert::UsageType(BufferDataUsage::Code type)
    {
        if (type == BufferDataUsage::StaticDraw) return GL_STATIC_DRAW;
        if (type == BufferDataUsage::DynamicDraw) return GL_DYNAMIC_DRAW;
        if (type == BufferDataUsage::StreamDraw) return GL_STREAM_DRAW;
        return 0;
    }

    uint32 GLConvert::CullModeType(CullModeType::Code mode) {
        if (mode == CullModeType::Front) return GL_BACK;
        if (mode == CullModeType::Back) return GL_FRONT;
        if (mode == CullModeType::FrontBack) return GL_FRONT_AND_BACK;
        return 0;
    }

    uint32 GLConvert::PolygonModeType(PolygonModeType::Code contentMode) {
        if (contentMode == PolygonModeType::Line) return GL_LINE;
        if (contentMode == PolygonModeType::Fill) return GL_FILL;
        if (contentMode == PolygonModeType::FillRectangle) return GL_FILL_RECTANGLE_NV;
        if (contentMode == PolygonModeType::Point) return GL_POINT;
        return 0;
    }

    uint32 GLConvert::VerticesPerType(uint32 vertexType) {
        if (vertexType == GL_TRIANGLES) return 3;
        return 0;
    }

    uint32 GLConvert::VertexType(DrawVertexType::Code drawVertex) {
        if (drawVertex == DrawVertexType::Triangles) return GL_TRIANGLES;
        return 0;
    }
    uint32 GLConvert::IndexType(DrawIndexType::Code drawIndex)
    {
        if (drawIndex == DrawIndexType::UnsignedInt) return GL_UNSIGNED_INT;
        return 0;
    }
    uint32 GLConvert::GetModuleType(ShaderType::Code code)
    {
        if (code == ShaderType::Vertex) return GL_VERTEX_SHADER;
        if (code == ShaderType::Fragment) return GL_FRAGMENT_SHADER;
        if (code == ShaderType::Compute) return GL_COMPUTE_SHADER;
        if (code == ShaderType::Geometry) return GL_GEOMETRY_SHADER;
        //if (code == ShaderType::Pixel) return GL_PIXEL_SHADER;
        if (code == ShaderType::TesControl) return GL_TESS_CONTROL_SHADER;
        if (code == ShaderType::TesEvaluation) return GL_TESS_EVALUATION_SHADER;
        return 0;
    }
}  //  nkentseu

#endif
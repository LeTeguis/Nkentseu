//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:55:46 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenGLUtils.h"

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
        if (mode == CullModeType::Front) return GL_FRONT;
        if (mode == CullModeType::Back) return GL_BACK;
        if (mode == CullModeType::FrontBack) return GL_FRONT_AND_BACK;
        return GL_FRONT_AND_BACK;
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
        if (drawIndex == DrawIndexType::UInt8) return GL_UNSIGNED_INT;
        if (drawIndex == DrawIndexType::UInt16) return GL_UNSIGNED_INT;
        if (drawIndex == DrawIndexType::UInt32) return GL_UNSIGNED_INT;
        if (drawIndex == DrawIndexType::UInt64) return GL_UNSIGNED_INT;
        return 0;
    }

    uint32 GLConvert::GetModuleType(ShaderType::Code code)
    {
        if (code == ShaderType::Vertex) return GL_VERTEX_SHADER;
        if (code == ShaderType::Fragment || code == ShaderType::Pixel) return GL_FRAGMENT_SHADER;
        if (code == ShaderType::Compute) return GL_COMPUTE_SHADER;
        if (code == ShaderType::Geometry) return GL_GEOMETRY_SHADER;
        if (code == ShaderType::TesControl) return GL_TESS_CONTROL_SHADER;
        if (code == ShaderType::TesEvaluation) return GL_TESS_EVALUATION_SHADER;
        return 0;
    }

    uint32 GLConvert::GetModernModuleType(ShaderType::Code shaderType) {
        if (shaderType == ShaderType::Vertex) return GL_VERTEX_SHADER_BIT;
        if (shaderType == ShaderType::Fragment || shaderType == ShaderType::Pixel) return GL_FRAGMENT_SHADER_BIT;
        if (shaderType == ShaderType::Geometry) return GL_GEOMETRY_SHADER_BIT;
        if (shaderType == ShaderType::TesControl) return GL_TESS_CONTROL_SHADER_BIT;
        if (shaderType == ShaderType::TesEvaluation) return GL_TESS_EVALUATION_SHADER_BIT;
        if (shaderType == ShaderType::Compute) return GL_COMPUTE_SHADER_BIT;
        return 0;
    }

    uint32 GLConvert::GetPrimitiveType(RenderPrimitive::Enum primitive)
    {
        if (primitive == RenderPrimitive::Points) return GL_POINTS;
        if (primitive == RenderPrimitive::Lines) return GL_LINES;
        if (primitive == RenderPrimitive::LineStrip) return GL_LINE_STRIP;
        if (primitive == RenderPrimitive::Triangles) return GL_TRIANGLES;
        if (primitive == RenderPrimitive::TriangleStrip) return GL_TRIANGLE_STRIP;
        if (primitive == RenderPrimitive::TriangleFan) return GL_TRIANGLE_FAN;
        return GL_TRIANGLES;
    }

    /*bool OpenglBuffer::Create(const std::string& uniforName, usize size, BufferDataUsage::Code usage, usize binding, int64 offset)
    {
        OpenGLResult result;
        bool first = true;
        Log_nts.Debug("Bind = {0}", binding);

        this->usage = usage;

        glCheckError(first, result, glCreateBuffers(1, &this->buffer), "cannot gen buffer");
        glCheckError(first, result, glNamedBufferData(this->buffer, size, nullptr, GLConvert::UsageType(usage)), "cannot gen buffer"); // TODO: investigate usage hint
        glCheckError(first, result, glBindBufferBase(GL_UNIFORM_BUFFER, binding, this->buffer), "cannot gen buffer");

        /*glCheckError(first, result, glGenBuffers(1, &uniform), "cannot gen buffer");
        glCheckError(first, result, glBindBuffer(GL_UNIFORM_BUFFER, uniform), "cannot bind buffer");
        glCheckError(first, result, glBufferData(GL_UNIFORM_BUFFER, size, NULL, GLConvert::UsageType(usage)), "cannot set buffer data");
        glCheckError(first, result, glBindBuffer(GL_UNIFORM_BUFFER, 0), "cannot unbind buffer");

        glCheckError(first, result, glBindBufferRange(GL_UNIFORM_BUFFER, binding, uniform, offset, size), "cannot bind buffer range");//* /
        return result.success;
    }*/

    bool OpenglBuffer::Create(const std::string& uniforName, uint32 bufferType, uint32 usage, const void* data, usize size, usize binding, int64 offset, uint32 instance, bool usedsa)
    {
        OpenGLResult result;
        bool first = true;

        this->usage = usage;
        this->bufferType = bufferType;
        this->useDAS = usedsa;
        this->instance = instance;
        this->name = uniforName;
        this->binding = binding;

        uint32 sizes = size;

        if (this->useDAS) {
            if (this->instance > 1 && bufferType == GL_UNIFORM_BUFFER) {
                int32 minUboAlignment = 0;
                glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &minUboAlignment);

                if (minUboAlignment > 0) {
                    dynamicAlignment = (size + minUboAlignment - 1) & ~(minUboAlignment - 1);
                    sizes = dynamicAlignment * instance;
                }
            }

            glCheckError(first, result, glCreateBuffers(1, &buffer), "cannot create dsa buffer : {0}", name);
            glCheckError(first, result, glNamedBufferData(buffer, sizes, data, usage), "cannot set data buffer : {0}", name);
        }
        else {
            glCheckError(first, result, glGenBuffers(1, &buffer), "cannot gen buffer : {0}", name);
            glCheckError(first, result, glBindBuffer(bufferType, buffer), "cannot bind buffer : {0}", name);
            glCheckError(first, result, glBufferData(bufferType, sizes, data, usage), "cannot set data buffer : {0}", name);
            //Log_nts.Debug("type = {0}, name = {1}", bufferType, name);
        }   

        if (result.success){
            Unbind();
        }

        if (bufferType == GL_UNIFORM_BUFFER) {
            glCheckError(first, result, glBindBufferRange(bufferType, binding, buffer, offset, size), "cannot bind buffer range : {0}", name);
        }

        return result.success;
    }

    bool OpenglBuffer::Destroy()
    {
        if (buffer == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glDeleteBuffers(1, &buffer), "cannot delete buffer : {0}", name);
        buffer = 0;

        return result.success;
    }

    bool OpenglBuffer::WriteToBuffer(const void* data, usize size, usize offset)
    {
        if (buffer == 0) return false;

        OpenGLResult result;
        bool first = true;

        if (this->useDAS) {

            currentOffset = offset;

            if (uType == UniformBufferType::Dynamic && bufferType == GL_UNIFORM_BUFFER) {
                currentIndex++;

                if (currentIndex >= instance) {
                    currentIndex = 0;
                }
                currentOffset = currentIndex * dynamicAlignment;
            }

            glCheckError(first, result, glNamedBufferSubData(buffer, offset, size, data), "cannot bind buffer : {0}", name);
        }
        else {
            glCheckError(first, result, glBindBuffer(bufferType, buffer), "cannot bind buffer : {0}", name);
            glCheckError(first, result, glBufferSubData(bufferType, offset, size, data), "cannot bind buffer : {0}", name);
            if (bufferType == GL_UNIFORM_BUFFER) {
                glCheckError(first, result, glBindBufferRange(bufferType, binding, buffer, offset, size), "cannot bind buffer range : {0}", name);
            }
            glCheckError(first, result, glBindBuffer(bufferType, 0), "cannot bind buffer : {0}", name);
        }

        return result.success;
    }

    bool OpenglBuffer::Bind() const
    {
        if (buffer == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindBuffer(bufferType, buffer), "cannot bind buffer : {0}", name);

        return result.success;
    }

    bool OpenglBuffer::Unbind() const
    {
        if (buffer == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindBuffer(bufferType, 0), "cannot unbind buffer : {0}", name);

        return result.success;
    }

}  //  nkentseu
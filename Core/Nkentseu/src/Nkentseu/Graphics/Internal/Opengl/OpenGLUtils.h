//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-19 at 10:55:46 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_UTILS_H__
#define __OPENGL_UTILS_H__

#pragma once

#include <System/System.h>

#include <Nkentseu/Core/NkentseuLogger.h>
#include "OpenglContext.h"
#include "Nkentseu/Graphics/ShaderInfo.h"

#include <glad/gl.h>

namespace nkentseu {
    struct OpenGLResult {
        bool success = false;
        GLenum result;
    };

    struct OpenGLStaticDebugInfo {
        static std::string file_call;
        static uint32 line_call;
        static std::string methode_call;
        static bool success;

        static void SetInfo(const std::string& file, uint32 line, const std::string& method) {
            file_call = file;
            line_call = line;
            methode_call = method;
        }
    };

    template<typename... Args>
    OpenGLResult glCheckError_(const char* format, Args&&... args) {
        OpenGLResult result = { true, GL_NO_ERROR };
        GLenum errorCode;

        std::string file = OpenGLStaticDebugInfo::file_call;
        std::string function = OpenGLStaticDebugInfo::methode_call;
        uint32 line = OpenGLStaticDebugInfo::line_call;

        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            if (result.success) {
                result.result = errorCode;
                result.success = errorCode == GL_NO_ERROR;
            }

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
            std::string message = FORMATTER.Format(format, args...);
            message = FORMATTER.Format("code : {0}({1}); {2}", error, errorCode, message);
            NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, function.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Error("{0}", message);

        }
        return result;
    }

    #define glCheckError(first, presult, function, format, ... ) OpenGLStaticDebugInfo::SetInfo(__FILE__, __LINE__, __FUNCTION__); function; presult = (presult.success || first) ? glCheckError_(format, ##__VA_ARGS__) : presult; first = false

    class NKENTSEU_API GLConvert {
        public:
            static uint32 ShaderType(ShaderDataType::Code type);
            static uint32 UsageType(BufferDataUsage::Code type);
            static uint32 CullModeType(CullModeType::Code mode);
            static uint32 PolygonModeType(PolygonModeType::Code contentMode);
            static uint32 VerticesPerType(uint32 vertexType);
            static uint32 VertexType(DrawVertexType::Code drawVertex);
            static uint32 IndexType(DrawIndexType::Code drawIndex);
            static uint32 GetModuleType(ShaderType::Code code);
            static uint32 GetModernModuleType(ShaderType::Code shaderType);
    };

    struct NKENTSEU_API OpenglBuffer {
        bool Create(const std::string& uniforName, usize size, BufferDataUsage::Code usage, usize binding, int64 offset);
        bool Destroy();
        bool WriteToBuffer(void* data, usize size, usize offset = 0);

        uint32 uniform;
    };
}  //  nkentseu

#endif  // __OPEN_G_L_UTILS_H__!
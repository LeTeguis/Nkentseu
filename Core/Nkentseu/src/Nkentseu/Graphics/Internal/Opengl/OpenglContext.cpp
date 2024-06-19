//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:22 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//
#include "NkentseuPch/ntspch.h"
#include "OpenglContext.h"
#include "Nkentseu/Core/Window.h"
#include <glad/gl.h>

#ifdef NKENTSEU_PLATFORM_WINDOWS
#include <Nkentseu/Platform/Window/Windows/WGLContext.h>
#elif defined NKENTSEU_PLATFORM_LINUX
    #include <GL/glext.h>
    #if defined(NKENTSEU_PLATFORM_LINUX_XCB)
    #include <Nkentseu/Platform/Window/Linux/XCB/XGLContext.h>
    #elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
    #include <Nkentseu/Platform/Window/Linux/XLIB/XGLContext.h>
    #endif
#endif

#include <Nkentseu/Core/NkentseuLogger.h>
#include "OpenGLUtils.h"

namespace nkentseu {
    void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
        std::string source_str;
        std::string type_str;

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            source_str = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            source_str = "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            source_str = "Shader Compile"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            source_str = "Third party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:
            source_str = "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:
            source_str = "Other"; break;
        }

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            type_str = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            type_str = "Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            type_str = "Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:
            type_str = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            type_str = "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:
            type_str = "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            type_str = "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:
            type_str = "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:
            type_str = "Other"; break;
        }

        std::string file = OpenGLStaticDebugInfo::file_call;
        std::string methode = OpenGLStaticDebugInfo::methode_call;
        uint32 line = OpenGLStaticDebugInfo::line_call;

        OpenGLStaticDebugInfo::success = false;

        if (severity == GL_DEBUG_SEVERITY_HIGH) {
            std::string message = FORMATTER.Format("source : {0}({1}), type : {2}({3})", source_str, source, type_str, type);
            NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Fatal("{0}", message);
            return;
        }
        else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
            std::string message = FORMATTER.Format("source : {0}({1}), type : {2}({3})", source_str, source, type_str, type);
            NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Error("{0}", message);
            return;
        }
        else if (severity == GL_DEBUG_SEVERITY_LOW) {
            std::string message = FORMATTER.Format("source : {0}({1}), type : {2}({3})", source_str, source, type_str, type);
            NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Warning("{0}", message);
            return;
        }
        else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
            //NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Warning("{0}", message);
            //return;
        }

        OpenGLStaticDebugInfo::success = true;
    }

    OpenglContext::OpenglContext(Memory::Shared<Window> window, const ContextProperties& contextProperties) : m_Window(window), m_Properties(contextProperties){
        m_NativeContext = Memory::Alloc<NativeContext>();

        if (m_NativeContext == nullptr) return;

        if (!m_NativeContext->SetWindow(m_Window == nullptr ? nullptr : m_Window.get())) {
            return;
        }

        if (!m_NativeContext->SetProperties(m_Properties)) {
            return;
        }
    }

    OpenglContext::~OpenglContext(){
    }

    bool OpenglContext::Initialize()
    {
        if (m_NativeContext == nullptr) return false;

        if (m_NativeContext->Initialize()) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            return true;
        }
        return false;
    }

    bool OpenglContext::Deinitialize()
    {
        if (m_NativeContext == nullptr) return false;
        bool deinit = m_NativeContext->Deinitialize();
        if (deinit) {
            Memory::Reset(m_NativeContext);
        }
        return deinit;
    }

    bool OpenglContext::IsInitialize()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->IsInitialize();
    }

    bool OpenglContext::MakeCurrent()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->MakeCurrent();
    }

    bool OpenglContext::UnmakeCurrent()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->UnmakeCurrent();
    }

    bool OpenglContext::IsCurrent()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->IsCurrent();
    }

    bool OpenglContext::EnableVSync()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->EnableVSync();
    }

    bool OpenglContext::DisableVSync()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->DisableVSync();
    }

    bool OpenglContext::Swapchaine()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->Swapchaine();
    }

    const GraphicsInfos& OpenglContext::GetGraphicsInfo()
    {
        static const GraphicsInfos graphicsInfos = {}; 
        if (m_NativeContext == nullptr) return graphicsInfos;
        return m_NativeContext->GetGraphicsInfo();
    }

    Memory::Shared<NativeContext> OpenglContext::GetNative() {
        return m_NativeContext;
    }

    Memory::Shared<Window> OpenglContext::GetWindow() {
        return m_Window;
    }

    const ContextProperties& OpenglContext::GetProperties()
    {
        return m_Properties;
    }

    bool OpenglContext::IsValidContext()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->IsInitialize();
    }
}    // namespace nkentseu
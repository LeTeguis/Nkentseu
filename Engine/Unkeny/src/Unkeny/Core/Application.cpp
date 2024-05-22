//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 5:12:05 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "UnkenyPch/ntspch.h"
#include "Application.h"

#include <Nkentseu/Core/Window.h>

#include "Log.h"
#include <Ntsm/Ntsm.h>

#include <Nkentseu/Core/Events.h>
#include <Nkentseu/Graphics/Color.h>
#include <Nkentseu/Graphics/Context.h>
#include <Nkentseu/Graphics/Renderer.h>
#include <Nkentseu/Event/EventBroker.h>

#include <Nkentseu/Event/InputManager.h>

#include <Nkentseu/Graphics/Shader.h>


namespace nkentseu {

    std::vector<float32> vertices =
    {
        //0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
    };

    std::vector<float32> verticesTriangle =
    {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
    };

    std::vector<uint32> indices = {
        0, 1, 3, // premier triangle
        1, 2, 3 // second triangle
    };

    Memory::Shared<VertexBuffer> vertexBuffer = nullptr;
    Memory::Shared<IndexBuffer> indexBuffer = nullptr;
    Memory::Shared<VertexArray> vertexArray = nullptr;

    Application::Application() : m_Running(false) {
    }

    Application::~Application(){
    }

    bool Application::Initialize()
    {
        WindowProperties windowProperty;
        windowProperty.title = "Unkeny Engine";
        windowProperty.size.width = 1000;
        windowProperty.size.height = 600;

        m_Window = Memory::Alloc<Window>(windowProperty);
        NTSErrorCode error = ErrorMessaging.PopError();

        if (m_Window == nullptr || (error != NTSErrorCode::NoError && error != NTSErrorCode::Window_StayInWindowMode)) {
            Memory::Reset(m_Window);
            // Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", ErrorMessaging.GetErrorInfos(error));
            Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", (bool32)error);
            return false;
        }

        m_Context = Memory::Alloc<Context>();
        m_Renderer = Memory::Alloc<Renderer>();
        ContextProperties propertie;

        if (m_Context != nullptr) {
            if (m_Context->Initialize(m_Window.get(), propertie)) {
                Log.Debug("OpenGL version {0}.{1}", m_Context->GetProperties().version.major, m_Context->GetProperties().version.minor);
                if (m_Renderer != nullptr) {
                    m_Renderer->Initialize(m_Context.get());
                }
            }
        }
        
        EventTrack.AddObserver(EVENT_BIND_HANDLER(Application::OnEvent));
        
        Input.CreateAction("Saut", REGISTER_ACTION_SUBSCRIBER(Application::Saut));
        Input.AddCommand(ActionCommand("Saut", ActionCode(EventType::KeyboardInput, Keyboard::Space)));

        Input.CreateAxis("Course", REGISTER_AXIS_SUBSCRIBER(Application::Course));
        Input.AddCommand(AxisCommand("Course", AxisCode(EventType::KeyboardInput, Keyboard::Up)));

        m_Running = true;
        return m_Running;
    }

    void Application::Run()
    {
        if (m_Window == nullptr) {
            return;
        }

        if (m_Renderer == nullptr || m_Context == nullptr){
            if (m_Context != nullptr) m_Context->Deinitialize();
            if (m_Renderer != nullptr) m_Renderer->Deinitialize();
            m_Window->Close();
            return;
        }
        std::unordered_map<ShaderType::Code, std::string> shaderFiles;
        shaderFiles[ShaderType::Vertex] = "Resources/shaders/core.vs";
        shaderFiles[ShaderType::Fragment] = "Resources/shaders/core.frag";
        //shaderFiles[ShaderType::Vertex] = "Resources/shaders/shader.glsl.vert";
        //shaderFiles[ShaderType::Fragment] = "Resources/shaders/shader.glsl.frag";
        //shaderFiles[ShaderType::Vertex] = "Resources/shaders/triangleInternal.glsl.vert";
        //shaderFiles[ShaderType::Fragment] = "Resources/shaders/triangleInternal.glsl.frag";
        Memory::Shared<Shader> shader = Memory::Alloc<Shader>(shaderFiles);

        shader->Create();
        m_Renderer->SetActiveShader(shader);

        BufferLayout bufferLayout;
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float3, "position", false));
        //bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float3, "color", false));
        bufferLayout.CalculateOffsetsAndStride();

        vertexBuffer = Memory::Alloc<VertexBuffer>();
        if (vertexBuffer != nullptr) {
            if (!vertexBuffer->Create(BufferDataUsage::StaticDraw, vertices, bufferLayout.componentCount)) {
                Log.Error("Cannot create vertex buffer");
            }
        }
        else {
            Log.Error("Cannot allocate memory for vertex buffer");
        }

        indexBuffer = Memory::Alloc<IndexBuffer>();
        if (indexBuffer != nullptr) {
            if (!indexBuffer->Create(BufferDataUsage::StaticDraw, DrawIndexType::UnsignedInt, indices)) {
                Log.Error("Cannot create index buffer");
            }
        }
        else {
            Log.Error("Cannot allocate memory for index buffer");
        }

        vertexArray = Memory::Alloc<VertexArray>();
        if (vertexArray != nullptr) {
            vertexArray->SetVertexBuffer(vertexBuffer);
            vertexArray->SetIndexBuffer(indexBuffer);

            if (!vertexArray->Create(bufferLayout)) {
                Log.Error("Cannot create vertex array");
            }
        }
        else {
            Log.Error("Cannot allocate memory for vertex array");
        }

        while (m_Running) {
            EventTrack.Pick();

            if (m_Renderer == nullptr || m_Context == nullptr) { continue; }

            m_Renderer->Prepare();
            m_Renderer->Clear(Color::RandomRGB());
            //m_Renderer->Clear(Color::DefaultBackground());
            m_Renderer->Draw(vertexArray, DrawVertexType::Triangles);
            m_Renderer->Finalize();
        }

        if (vertexArray != nullptr) {
            vertexArray->Destroy();
        }

        if (indexBuffer != nullptr) {
            indexBuffer->Destroy();
        }

        if (vertexBuffer != nullptr) {
            vertexBuffer->Destroy();
        }

        m_Renderer->Deinitialize();
        m_Context->Deinitialize();
        m_Window->Close();
    }

    void Application::OnEvent(Event& event)
    {
        EventBroker broker(event);
        //Log.Debug("{0}", event);

        broker.Route<WindowStatusEvent>(REGISTER_CLIENT_EVENT(Application::OnWindowStatusEvent));
        broker.Route<KeyboardEvent>(REGISTER_CLIENT_EVENT(Application::OnKeyboardEvent));
        broker.Route<WindowResizedEvent>(REGISTER_CLIENT_EVENT(Application::OnWindowResizedEvent));
        broker.Route<WindowMovedEvent>(REGISTER_CLIENT_EVENT(Application::OnWindowMovedEvent));
        broker.Route<MouseInputEvent>(REGISTER_CLIENT_EVENT(Application::OnMouseInputEvent));
    }

    bool Application::OnWindowStatusEvent(WindowStatusEvent& event)
    {
        if (event.GetState() == WindowState::Closed) {
            m_Running = false;
            return m_Running;
        }
        return true;
    }

    bool Application::OnKeyboardEvent(KeyboardEvent& event)
    {
        if (event.GetKeycode() == Keyboard::Escape){
            m_Running = false;
        }

        if (event.GetKeycode() == Keyboard::Space && event.GetState() == ButtonState::Pressed) {
            if (m_DrawMode == DrawContentMode::Fill) {
                m_DrawMode = DrawContentMode::Line;
            }
            else if (m_DrawMode == DrawContentMode::Line) {
                m_DrawMode = DrawContentMode::Fill;
            }

            if (m_Renderer != nullptr) {
                m_Renderer->DrawMode(DrawMode::FrontBack, m_DrawMode);
            }
        }
        return false;
    }

    bool Application::OnWindowResizedEvent(WindowResizedEvent& event)
    {
        if (m_Renderer != nullptr) {
            m_Renderer->Resize(event.GetWindowRec().size);
            Log.Debug("{0}", event);
        }
        return false;
    }

    bool Application::OnWindowMovedEvent(WindowMovedEvent& event)
    {
        //Log.Debug("{0}", event);
        return false;
    }

    bool Application::OnMouseInputEvent(MouseInputEvent& event)
    {
        Log.Debug("{0}", event);
        return false;
    }

    void Application::Saut(const std::string& name, const ActionCode& actionCode, bool pressed, bool released)
    {
        Log.Debug("Saut");
    }

    void Application::Course(const std::string& name, const AxisCode& axisCode, float32 value)
    {
        if (value != 0)
            Log.Debug("Run {0}", value);
    }

}    // namespace nkentseu
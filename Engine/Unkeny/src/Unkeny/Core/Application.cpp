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
#include <Nkentseu/Core/FPSTimer.h>


namespace nkentseu {
    struct Vertex {
        Vector3f pos;
        Vector3f color;
    };

    float cubeVertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         // Bottom face          
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left              
    };

    const std::vector<Vertex> vertices_struct = {
        {{ 0.5f, 0.5f, 0.0f} , { 0.31f, 0.0f, 0.31f }}, // top right
        {{ 0.5f, -0.5f, 0.0f} , { 0.0f, 0.31f, 0.31f }}, // bottom right
        {{ -0.5f, -0.5f, 0.0f} , { 0.31f, 0.31f, 0.0f }}, // bottom left
        {{ -0.5f, 0.5f, 0.0f} , { 0.31f, 0.0f, 0.0f }} // top left
    };

    const std::vector<Vertex> vertices_triangles_multi = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };

    std::vector<float32> vertices =
    {
        //0.5f, 0.5f, 0.0f, 0.31f, 0.31f, 0.31f, // top right
        0.5f, -0.5f, 0.0f, 0.31f, 0.31f, 0.31f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.31f, 0.31f, 0.31f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.31f, 0.31f, 0.31f // top left
    };

    std::vector<float32> verticesTriangle =
    {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
    };

    std::vector<uint32> indices = {
        //0, 1, 2//, // premier triangle
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
                Log.Debug("Api version {0}.{1}", m_Context->GetProperties().version.major, m_Context->GetProperties().version.minor);
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

        Context* context = m_Context == nullptr ? nullptr : m_Context.get();

        if (m_Renderer == nullptr || m_Context == nullptr){
            if (m_Context != nullptr) m_Context->Deinitialize();
            if (m_Renderer != nullptr) m_Renderer->Deinitialize();
            m_Window->Close();
            return;
        }

        Timer timer;

        BufferLayout bufferLayout;
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float3, "position", 0));
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float3, "color", 1));
        bufferLayout.CalculateOffsetsAndStride();

        std::unordered_map<ShaderType::Code, std::string> shaderFiles;
        shaderFiles[ShaderType::Vertex] = "Resources/shaders/core.vert.glsl";
        shaderFiles[ShaderType::Fragment] = "Resources/shaders/core.frag.glsl";
        //shaderFiles[ShaderType::Vertex] = "Resources/shaders/shader.vert.glsl";
        //shaderFiles[ShaderType::Fragment] = "Resources/shaders/shader.frag.glsl";
        //shaderFiles[ShaderType::Vertex] = "Resources/shaders/triangleInternal.vert.glsl";
        //shaderFiles[ShaderType::Fragment] = "Resources/shaders/triangleInternal.frag.glsl";
        Memory::Shared<Shader> shader = Memory::Alloc<Shader>(context, shaderFiles, bufferLayout);

        shader->Create();

        vertexBuffer = Memory::Alloc<VertexBuffer>();
        if (vertexBuffer != nullptr) {
            if (!vertexBuffer->Create<Vertex>(context, BufferDataUsage::StaticDraw, vertices_struct, bufferLayout)) {
                Log.Error("Cannot create vertex buffer");
            }
        }
        else {
            Log.Error("Cannot allocate memory for vertex buffer");
        }

        indexBuffer = Memory::Alloc<IndexBuffer>();
        if (indexBuffer != nullptr) {
            if (!indexBuffer->Create(context, BufferDataUsage::StaticDraw, DrawIndexType::UInt32, indices)) {
                Log.Error("Cannot create index buffer");
            }
        }
        else {
            Log.Error("Cannot allocate memory for index buffer");
        }

        vertexArray = Memory::Alloc<VertexArray>(context);
        if (vertexArray != nullptr) {
            vertexArray->SetVertexBuffer(vertexBuffer);
            vertexArray->SetIndexBuffer(indexBuffer);

            if (!vertexArray->Create(bufferLayout)) {
            //if (!vertexArray->Create(context, 3)) {
                Log.Error("Cannot create vertex array");
            }
        }
        else {
            Log.Error("Cannot allocate memory for vertex array");
        }

        int32 numFrames = -1;
        float32 frameTime = 0;

        FPSTimer fps;

        while (m_Running) {
            fps.Update();
            /*float64 delta = timer.Elapsed();
            if (delta >= 1.0) {
                int32 currency = (int32)(numFrames / delta);
                int32 framerate = (1 > currency) ? 1 : currency;
                frameTime = 1000.0 / framerate;
                numFrames = -1;
                timer.Reset();

                std::string title = FORMATTER.Format("Running at {0} fps.", framerate);
                m_Window->SetTitle(title);
            }
            ++numFrames;*/
            std::string title = FORMATTER.Format("Running at {0} fps. {1}", fps.GetFps(), fps.GetFrameTime());
            m_Window->SetTitle(title);

            EventTrack.Pick();

            if (m_Renderer == nullptr || m_Context == nullptr) { continue; }

            //m_Renderer->Clear(Color::RandomRGB());
            
            timer.Reset();
            m_Renderer->Clear(Color::Black());
            //Log.Debug("Clear time {0} ms", timer.Reset().milliSeconds);

            m_Renderer->Prepare();
            //Log.Debug("Prepare time {0} ms", timer.Reset().milliSeconds);
            //m_Renderer->DrawMode(CullModeType::Back, m_PolygonMode);
            m_Renderer->DrawMode(CullModeType::NoCull, m_PolygonMode);
            //Log.Debug("DrawMode time {0} ms", timer.Reset().milliSeconds);
            m_Renderer->UseShader(shader);
            //Log.Debug("UseShader time {0} ms", timer.Reset().milliSeconds);
            m_Renderer->Draw(vertexArray, DrawVertexType::Triangles);
            //Log.Debug("Draw time {0} ms", timer.Reset().milliSeconds);
            m_Renderer->Finalize();
            //Log.Debug("Finalize time {0} ms", timer.Reset().milliSeconds);
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

        shader->Destroy();
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
            if (m_PolygonMode == PolygonModeType::Fill) {
                m_PolygonMode = PolygonModeType::Line;
            }
            else if (m_PolygonMode == PolygonModeType::Line) {
                m_PolygonMode = PolygonModeType::Point;
            }
            else if (m_PolygonMode == PolygonModeType::Point) {
                m_PolygonMode = PolygonModeType::Fill;
            }
        }
        return false;
    }

    bool Application::OnWindowResizedEvent(WindowResizedEvent& event)
    {
        if (m_Renderer != nullptr) {
            //m_Renderer->Resize(event.GetSize());
            //Log.Debug("{0}", event);
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
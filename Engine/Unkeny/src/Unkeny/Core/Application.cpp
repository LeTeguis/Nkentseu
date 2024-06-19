//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 5:12:05 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "UnkenyPch/ntspch.h"
#include "Application.h"

#include <Nkentseu/Core/Window.h>

#include "Log.h"
#include <Ntsm/Ntsm.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Nkentseu/Core/Events.h>
#include <Nkentseu/Graphics/Color.h>
#include <Nkentseu/Graphics/Context.h>
#include <Nkentseu/Graphics/Renderer.h>
#include <Nkentseu/Event/EventBroker.h>

#include <Nkentseu/Event/InputManager.h>

#include <Nkentseu/Graphics/Shader.h>
#include <Nkentseu/Core/FPSTimer.h>

#include "Unkeny/Graphics/CameraEditor.h"
#include <Unkeny/Graphics/Camera.h>
#include <Nkentseu/Graphics/Texture.h>


namespace nkentseu {
    using namespace maths;

    struct Vertex {
        Vector3f position;
        Vector3f color;
        Vector2f uv;
    };

    /*struct UniformBufferObject {
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
    };*/

    struct ObjectBuffer {
        matrix4f model = matrix4f::Identity();
    };

    struct CameraBuffer {
        matrix4f view = matrix4f::Identity();
        matrix4f proj = matrix4f::Identity();
    };

    float cubeVertices1[] = {
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
        {{ 0.5f, 0.5f, 0.0f} , { 0.31f, 0.0f, 0.31f }, { 0.0f, 0.0f }}, // top right
        {{ 0.5f, -0.5f, 0.0f} , { 0.0f, 0.31f, 0.31f }, {0.0f, 1.0f }}, // bottom right
        {{ -0.5f, -0.5f, 0.0f} , { 0.31f, 0.31f, 0.0f }, {1.0f, 1.0f }}, // bottom left
        {{ -0.5f, 0.5f, 0.0f} , { 0.31f, 0.0f, 0.0f }, {1.0f, 0.0f }} // top left
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

    const std::vector<Vertex> cubeVertices = {
        // Face avant
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f }}, // 0
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f }}, // 1
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f }}, // 2
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f }}, // 3
        // Face arrière
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f }}, // 4
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f }}, // 5
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f }}, // 6
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f }}  // 7
    };

    /*const std::vector<Vertex> cubeVertices = {
        // Face avant
        {{-0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}}, // 0 (Gray color: R = 0.5, G = 0.5, B = 0.5)
        {{ 0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}}, // 1
        {{ 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}}, // 2
        {{-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}}, // 3
        // Face arrière
        {{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}}, // 4
        {{ 0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}}, // 5
        {{ 0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}}, // 6
        {{-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}} // 7
    };*/

    std::vector<uint32_t> cubeIndices = {
        // Face avant
        0, 1, 2, 2, 3, 0,
        // Face arrière
        4, 5, 6, 6, 7, 4,
        // Face gauche
        0, 3, 7, 7, 4, 0,
        // Face droite
        1, 2, 6, 6, 5, 1,
        // Face supérieure
        3, 2, 6, 6, 7, 3,
        // Face inférieure
        0, 1, 5, 5, 4, 0
    };

    Vector3f cubePositions[] = {
           Vector3f(0.0f, 0.0f, 0.0f),
           Vector3f(2.0f, 5.0f, -15.0f),
           Vector3f(-1.5f, -2.2f, -2.5f),
           Vector3f(-3.8f, -2.0f, -12.3f),
           Vector3f(2.4f, -0.4f, -3.5f),
           Vector3f(-1.7f, 3.0f, -7.5f),
           Vector3f(1.3f, -2.0f, -2.5f),
           Vector3f(1.5f, 2.0f, -2.5f),
           Vector3f(1.5f, 0.2f, -1.5f),
           Vector3f(-1.3f, 1.0f, -1.5f)
    };

    Vector3f cameraSens = Vector3f(0.0f);

    Memory::Shared<VertexBuffer> vertexBuffer = nullptr;
    Memory::Shared<IndexBuffer> indexBuffer = nullptr;
    Memory::Shared<VertexArray> vertexArray = nullptr;

    Memory::Shared<VertexBuffer> vertexBuffer2 = nullptr;
    Memory::Shared<IndexBuffer> indexBuffer2 = nullptr;
    Memory::Shared<VertexArray> vertexArray2 = nullptr;
    Vector2f mouseDelta;
    Camera camera;
    CameraMovement movementCamera = CameraMovement::FORWARD;
    bool perspective = true;

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

        //m_Window = Memory::Alloc<Window>(windowProperty);
        m_Window = Window::Create(windowProperty);
        NTSErrorCode error = ErrorMessaging.PopError();

        if (m_Window == nullptr || (error != NTSErrorCode::NoError && error != NTSErrorCode::Window_StayInWindowMode)) {
            Memory::Reset(m_Window);
            // Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", ErrorMessaging.GetErrorInfos(error));
            Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", (bool32)error);
            return false;
        }

        //ContextProperties propertie(GraphicsApiType::VulkanApi);
        ContextProperties propertie(GraphicsApiType::OpenglApi);
        //ContextProperties propertie(GraphicsApiType::OpenglApi, Vector2i(4, 6));

        m_Context = Context::CreateInitialized(m_Window, propertie);
        m_Renderer = Renderer::CreateInitialized(m_Context);

        if (m_Renderer == nullptr) {
            Log.Fatal();
        }

        if (m_Context != nullptr) {
            Log.Debug("Api version {0}.{1}", m_Context->GetProperties().version.major, m_Context->GetProperties().version.minor);
        }

        //Events.AddObserver(EVENT_BIND_HANDLER(Application::OnEvent));
        EventTraker.AddObserver(EVENT_BIND_HANDLER(Application::OnEvent));

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

        if (m_Renderer == nullptr || m_Context == nullptr) {
            if (m_Context != nullptr) {
                m_Context->Deinitialize();
                Log.Debug();
            }
            if (m_Renderer != nullptr) {
                m_Renderer->Deinitialize();
                Log.Debug();
            }
            m_Window->Close();
            return;
        }

        Timer timer;

        BufferLayout bufferLayout;
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float3, "position", 0));
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float3, "color", 1));
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float2, "uv", 2));
        bufferLayout.CalculateOffsetsAndStride();

        UniformBufferLayout uniformLayout;
        uniformLayout.AddAttribut(UniformBufferAttribut(sizeof(ObjectBuffer), 0, "objectBuffer", ShaderType::Vertex, UniformBufferType::Dynamic, 10));
        uniformLayout.AddAttribut(UniformBufferAttribut(sizeof(CameraBuffer), 1, "cameraBuffer", ShaderType::Vertex, UniformBufferType::Static, 10));

        std::unordered_map<ShaderType::Code, std::string> shaderFiles;
        shaderFiles[ShaderType::Vertex] = "Resources/shaders/ubo.vert.glsl";
        shaderFiles[ShaderType::Fragment] = "Resources/shaders/ubo.frag.glsl";
        //shaderFiles[ShaderType::Vertex] = "Resources/shaders/core.vert.glsl";
        //shaderFiles[ShaderType::Fragment] = "Resources/shaders/core.frag.glsl";
        //shaderFiles[ShaderType::Vertex] = "Resources/shaders/shader.vert.glsl";
        //shaderFiles[ShaderType::Fragment] = "Resources/shaders/shader.frag.glsl";
        //shaderFiles[ShaderType::Vertex] = "Resources/shaders/triangleInternal.vert.glsl";
        //shaderFiles[ShaderType::Fragment] = "Resources/shaders/triangleInternal.frag.glsl";

        ShaderBufferLayout shaderLayout;
        shaderLayout.vertexInput = bufferLayout;
        shaderLayout.uniformBuffer = uniformLayout;

        Memory::Shared<Shader> shader = Shader::Create(m_Context, shaderFiles, shaderLayout);
        if (shader == nullptr) {
            Log.Error("Cannot create shader");
        }

        Memory::Shared<UniformBuffer> uniformBuffer = UniformBuffer::Create(m_Context, shader, uniformLayout);
        if (uniformBuffer == nullptr) {
            Log.Error("Cannot create uniform buffer");
        }

        /*std::vector<Memory::Shared<UniformBuffer>> ulist;

        for (uint32 i = 0; i < 10; i++) {
            Memory::Shared<UniformBuffer> ub = UniformBuffer::Create(m_Context, shader, uniformLayout);
            if (ub == nullptr) {
                Log.Error("Cannot create uniform buffer");
            }
            else {
                ulist.push_back(ub);
            }
        }*/

        vertexBuffer = VertexBuffer::Create<Vertex>(m_Context, BufferDataUsage::StaticDraw, cubeVertices, bufferLayout);
        if (vertexBuffer == nullptr) {
            Log.Error("Cannot create vertex buffer");
        }

        indexBuffer = IndexBuffer::Create(m_Context, BufferDataUsage::StaticDraw, cubeIndices);
        if (indexBuffer == nullptr) {
            Log.Error("Cannot create index buffer");
        }

        vertexArray = VertexArray::Create(m_Context);
        //vertexArray = VertexArray::Create(m_Context, 3);
        if (vertexArray == nullptr) {
            Log.Error("Cannot create vertex array");
        }
        else {
            vertexArray->SetVertexBuffer(vertexBuffer);
            vertexArray->SetIndexBuffer(indexBuffer);
        }

        vertexBuffer2 = VertexBuffer::Create<Vertex>(m_Context, BufferDataUsage::StaticDraw, vertices_struct, bufferLayout);
        if (vertexBuffer2 == nullptr) {
            Log.Error("Cannot create vertex buffer");
        }

        indexBuffer2 = IndexBuffer::Create(m_Context, BufferDataUsage::StaticDraw, indices);
        if (indexBuffer2 == nullptr) {
            Log.Error("Cannot create index buffer");
        }

        vertexArray2 = VertexArray::Create(m_Context);
        //vertexArray = VertexArray::Create(m_Context, 3);
        if (vertexArray2 == nullptr) {
            Log.Error("Cannot create vertex array");
        }
        else {
            vertexArray2->SetVertexBuffer(vertexBuffer2);
            vertexArray2->SetIndexBuffer(indexBuffer2);
        }

        Memory::Shared<Texture2D> tetxure = Texture2D::Create(m_Context, "Resources/textures/container2.png");
        if (tetxure == nullptr) {
            Log.Error("impossible de charger la texture  Resources/textures/container2.png");
        }
        else {
            Log.Info("Texture charger");
            tetxure->SetRepeated(true);
        }

        int32 numFrames = -1;
        float32 frameTime = 0;
        float32 time = 0.0f;

        FPSTimer fps;

        ObjectBuffer objectBuffer{};

        //camera.FPS_Camera = true;

        CameraBuffer cameraBuffer{};
        cameraBuffer.view = camera.GetView();
        cameraBuffer.proj = camera.GetProjection(CameraProjection::Perspective, camera.zoom(), m_Window->GetDpiAspect(), 0.1f, 100.0f);
        if (m_Context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
            cameraBuffer.proj[1][1] *= -1;
        }
        int32 slot = 0;
        while (m_Running) {
            //float64 delta = timer.Elapsed().seconds;
            float64 delta = timer.Reset().seconds;
            time += delta;
            fps.Update(delta);

            std::string title = FORMATTER.Format("Running at {0} fps. {1}, position {2}, oriantation {3}", fps.GetFps(), fps.GetFrameTime(), camera.Position, camera.Orientation);
            m_Window->SetTitle(title);

            EventTraker.Loop();

            float32 cameraSpeed = 2.5f * delta;
            float32 cameraRotationSpeed = 15.0f * delta;

            if (Input.IsKeyDown(Keyboard::Up)) {
                camera.ProcessKeyboard(CameraMovement::FORWARD, delta);
            } else if (Input.IsKeyDown(Keyboard::Down)) {
                camera.ProcessKeyboard(CameraMovement::BACKWARD, delta);
            }

            if (Input.IsKeyDown(Keyboard::Left)) {
                camera.ProcessKeyboard(CameraMovement::LEFT, delta);
            } else if (Input.IsKeyDown(Keyboard::Right)) {
                camera.ProcessKeyboard(CameraMovement::RIGHT, delta);
            } 

            if (Input.IsKeyDown(Keyboard::Z)) {
                camera.ProcessKeyboard(CameraMovement::UP, delta);
            } else if (Input.IsKeyDown(Keyboard::S)) {
                camera.ProcessKeyboard(CameraMovement::DOWN, delta);
            }

            if (Input.IsKeyDown(Keyboard::Y)) {
            } else if (Input.IsKeyDown(Keyboard::H)) {
            }

            if (Input.IsKeyDown(Keyboard::G)) {
            } else if (Input.IsKeyDown(Keyboard::J)) {
            }

            static CameraProjection projection = CameraProjection::Perspective;
            static float32 fov_or_othosize = camera.zoom();
            
            if (Input.IsKeyDown(Keyboard::P)) {
                projection = CameraProjection::Perspective;
                fov_or_othosize = camera.zoom();
            } else if (Input.IsKeyDown(Keyboard::O)) {
                projection = CameraProjection::Orthographic;
                fov_or_othosize = camera.Position.Len();
            }

            if (m_Renderer == nullptr || m_Context == nullptr) { continue; }

            //m_Renderer->Begin(Color::Black());
            m_Renderer->Begin(Color::Black());

            if (shader != nullptr) {
                shader->DrawMode(CullModeType::NoCull, m_PolygonMode);
                shader->Bind();
            }

            if (uniformBuffer != nullptr) {
                cameraBuffer.view = camera.GetView();
                cameraBuffer.proj = camera.GetProjection(projection, fov_or_othosize, m_Window->GetDpiAspect(), 0.1f, 100.0f);

                if (m_Context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
                    cameraBuffer.proj[1][1] *= -1;
                }
                uniformBuffer->Bind("cameraBuffer", &cameraBuffer, sizeof(CameraBuffer));
            }

            if (tetxure != nullptr) {
                slot = (slot+1) % 10;
                tetxure->Binds(2);
            }

            for (uint32 index = 0; index < 10; index++) {
                ObjectBuffer objb;
                if (uniformBuffer != nullptr) {
                    {
                        // update model
                        objb.model = matrix4f::Scaling(matrix4f::Identity(), 0.25f * Vector3f(1.0f, 1.0f, 1.0f));
                        objb.model = matrix4f::Rotation(objb.model, Vector3f(0.0f, 1.0f, 0.0f), (float32)time * Angle(90.0f));
                        objb.model = matrix4f::Translation(objb.model, cubePositions[index] * 0.25);
                        uniformBuffer->Bind("objectBuffer", &objb, sizeof(ObjectBuffer), index);
                    }
                    uniformBuffer->Flush();
                }
                if (index < 5 && vertexArray != nullptr) {
                    vertexArray->Draw(DrawVertexType::Triangles);
                }
                if (index >= 5 && vertexArray2 != nullptr) {
                    vertexArray2->Draw(DrawVertexType::Triangles);
                }
            }

            m_Renderer->End();
            mouseDelta = Vector2f();
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

        if (vertexArray2 != nullptr) {
            vertexArray2->Destroy();
        }

        if (indexBuffer2 != nullptr) {
            indexBuffer2->Destroy();
        }

        if (vertexBuffer2 != nullptr) {
            vertexBuffer2->Destroy();
        }

        if (uniformBuffer != nullptr) {
            uniformBuffer->Destroy();
        }

        /*for (auto& ubo : ulist) {
            ubo->Destroy();
        }*/

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
        broker.Route<MouseMovedEvent>(REGISTER_CLIENT_EVENT(Application::OnMouseMoved));
        broker.Route<MouseWheelEvent>(REGISTER_CLIENT_EVENT(Application::OnMouseScroll));
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

        if (event.GetState() == ButtonState::Pressed) {
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

    bool Application::OnMouseMoved(MouseMovedEvent& event)
    {
        mouseDelta = event.GetSpeed();
        mouseDelta.Normalize();

        camera.ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
        return false;
    }

    bool Application::OnMouseScroll(MouseWheelEvent& event)
    {
        camera.ProcessMouseScroll(event.GetDelta());
        return false;
    }

    void Application::Saut(const std::string& name, const ActionCode& actionCode, bool pressed, bool released)
    {
        if (pressed) {
            Log.Debug("Saut");
        }
    }

    void Application::Course(const std::string& name, const AxisCode& axisCode, float32 value)
    {
        if (value != 0) {
            Log.Debug("Run {0}", value);
        }
    }

}    // namespace nkentseu
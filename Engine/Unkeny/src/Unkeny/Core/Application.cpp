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
#include <Nkentseu/Graphics/Texture.h>

#include "Unkeny/Graphics/CameraEditor.h"
#include "Unkeny/Graphics/Camera.h"
#include <Nkentseu/Graphics/PrimitiveMesh.h>
#include <Nkentseu/Graphics/ShaderInputLayout.h>

namespace nkentseu {
    using namespace maths;
    //Vector4f color;
    struct VertexTest {
        Vector3f position;
        Vector4f color;
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

    float32 cubeVertices1[] = {
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

    const std::vector<VertexTest> vertices_struct = {
        {{ 0.5f, 0.5f, 0.0f} , { 0.31f, 0.0f, 0.31f, 1.0f }, { 0.0f, 0.0f }}, // top right
        {{ 0.5f, -0.5f, 0.0f} , { 0.0f, 0.31f, 0.31f, 1.0f }, {0.0f, 1.0f }}, // bottom right
        {{ -0.5f, -0.5f, 0.0f} , { 0.31f, 0.31f, 0.0f, 1.0f }, {1.0f, 1.0f }}, // bottom left
        {{ -0.5f, 0.5f, 0.0f} , { 0.31f, 0.0f, 0.0f, 1.0f }, {1.0f, 0.0f }} // top left
    };

    const std::vector<VertexTest> vertices_triangles_multi = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f }},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f }},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, { 1.0f, 1.0f }}
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

    const std::vector<VertexTest> cubeVertices = {
        // Face avant
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f }}, // 0
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f }}, // 1
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f }}, // 2
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f }}, // 3
        // Face arrière
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f }}, // 4
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f }}, // 5
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f }}, // 6
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f }}  // 7
    };

    /*const std::vector<Vertex> cubeVertices = {
        // Face avant
        {{-0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}}, // 0 (Gray color: R = 0.5, G = 0.5, B = 0.5)
        {{ 0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}}, // 1
        {{ 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}}, // 2
        {{-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}}, // 3
        // Face arrière
        {{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}}, // 4
        {{ 0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5, 1.0ff}}, // 5
        {{ 0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}}, // 6
        {{-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}} // 7
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

    std::vector<VertexTest> verticesCube = {
        // Front face
        {{-1, -1,  1}, {1, 0, 0, 1}, {0, 0}},  // 0
        {{ 1, -1,  1}, {0, 1, 0, 1}, {1, 0}},  // 1
        {{ 1,  1,  1}, {0, 0, 1, 1}, {1, 1}},  // 2
        {{-1,  1,  1}, {1, 1, 0, 1}, {0, 1}},  // 3
        // Back face
        {{-1, -1, -1}, {1, 0, 1, 1}, {1, 0}},  // 4
        {{ 1, -1, -1}, {0, 1, 1, 1}, {0, 0}},  // 5
        {{ 1,  1, -1}, {1, 1, 1, 1}, {0, 1}},  // 6
        {{-1,  1, -1}, {0, 0, 0, 1}, {1, 1}},  // 7
    };

    std::vector<uint32> indicesCube = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        4, 0, 3, 3, 7, 4,
        // Right face
        1, 5, 6, 6, 2, 1,
        // Top face
        3, 2, 6, 6, 7, 3,
        // Bottom face
        4, 5, 1, 1, 0, 4,
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

    struct Shape3D {
        std::vector<VertexTest> vertices;
        std::vector<uint32> indices;
    };

    class ShapeCube : public Shape3D {
    public:
        ShapeCube() {
            vertices = {
                // Front face
                {{-1, -1,  1}, {1, 0, 0, 1}, {0, 0}},
                {{ 1, -1,  1}, {0, 1, 0, 1}, {1, 0}},
                {{ 1,  1,  1}, {0, 0, 1, 1}, {1, 1}},
                {{-1,  1,  1}, {1, 1, 0, 1}, {0, 1}},
                // Back face
                {{-1, -1, -1}, {1, 0, 1, 1}, {1, 0}},
                {{ 1, -1, -1}, {0, 1, 1, 1}, {0, 0}},
                {{ 1,  1, -1}, {1, 1, 1, 1}, {0, 1}},
                {{-1,  1, -1}, {0, 0, 0, 1}, {1, 1}},
            };

            indices = {
                /*/ Front face
                0, 1, 2, 2, 3, 0,
                // Back face
                4, 5, 6, 6, 7, 4,
                // Left face
                4, 0, 3, 3, 7, 4,
                // Right face
                1, 5, 6, 6, 2, 1,
                // Top face
                3, 2, 6, 6, 7, 3,
                // Bottom face
                4, 5, 1, 1, 0, 4,*/

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
        }
    };

    class ShapeSphere : public Shape3D {
    public:
        ShapeSphere(float32 radius = 1.0f, int32 sectorCount = 36, int32 stackCount = 18) {
            float32 x, y, z, xy;
            float32 s, t;
            float32 sectorStep = 2 * maths::Pi / sectorCount;
            float32 stackStep = maths::Pi / stackCount;
            float32 sectorAngle, stackAngle;

            for (int32 i = 0; i <= stackCount; ++i) {
                stackAngle = maths::Pi / 2 - i * stackStep;
                xy = radius * cosf(stackAngle);
                z = radius * sinf(stackAngle);

                for (int32 j = 0; j <= sectorCount; ++j) {
                    sectorAngle = j * sectorStep;
                    x = xy * cosf(sectorAngle);
                    y = xy * sinf(sectorAngle);
                    s = (float32)j / sectorCount;
                    t = (float32)i / stackCount;
                    vertices.push_back({ {x, y, z}, {1, 0, 0, 1}, {s, t} });
                }
            }

            int32 k1, k2;
            for (int32 i = 0; i < stackCount; ++i) {
                k1 = i * (sectorCount + 1);
                k2 = k1 + sectorCount + 1;
                for (int32 j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                    if (i != 0) {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);
                    }
                    if (i != (stackCount - 1)) {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }
            }
        }
    };

    class ShapeTube : public Shape3D {
    public:
        ShapeTube(float32 radius = 1.0f, float32 height = 2.0f, int32 sectorCount = 36) {
            float32 sectorStep = 2 * maths::Pi / sectorCount;
            float32 sectorAngle;

            for (int32 i = 0; i <= 1; ++i) {
                float32 h = (i == 0) ? -height / 2 : height / 2;
                for (int32 j = 0; j <= sectorCount; ++j) {
                    sectorAngle = j * sectorStep;
                    float32 x = radius * cosf(sectorAngle);
                    float32 y = radius * sinf(sectorAngle);
                    VertexTest vertex;
                    vertex.position = Vector3f( x, y, h );
                    vertex.color = Vector4f(x, y, h );
                    vertex.uv = Vector2f(j / (float32)sectorCount, i);
                    vertices.push_back(vertex);
                }
            }

            int32 k1, k2;
            for (int32 i = 0; i < sectorCount; ++i) {
                k1 = i;
                k2 = k1 + sectorCount + 1;
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    };

    class ShapeCapsule : public Shape3D {
    public:
        ShapeCapsule(float32 radius = 1.0f, float32 height = 2.0f, int32 sectorCount = 36, int32 stackCount = 18) {
            float32 sectorStep = 2 * maths::Pi / sectorCount;
            float32 stackStep = maths::Pi / stackCount;
            float32 sectorAngle, stackAngle;

            // Upper hemisphere
            for (int32 i = 0; i <= stackCount / 2; ++i) {
                stackAngle = maths::Pi / 2 - i * stackStep;
                float32 xy = radius * cosf(stackAngle);
                float32 z = radius * sinf(stackAngle) + height / 2;
                for (int32 j = 0; j <= sectorCount; ++j) {
                    sectorAngle = j * sectorStep;
                    float32 x = xy * cosf(sectorAngle);
                    float32 y = xy * sinf(sectorAngle);
                    vertices.push_back({ {x, y, z}, {1, 0, 0}, {j / (float32)sectorCount, i / (float32)stackCount} });
                }
            }

            // Cylinder
            for (int32 i = 0; i <= 1; ++i) {
                float32 h = (i == 0) ? height / 2 : -height / 2;
                for (int32 j = 0; j <= sectorCount; ++j) {
                    float32 sectorAngle = j * sectorStep;
                    float32 x = radius * cosf(sectorAngle);
                    float32 y = radius * sinf(sectorAngle);
                    vertices.push_back({ {x, y, h}, {0, 1, 0}, {j / (float32)sectorCount, (stackCount / 2 + i) / (float32)stackCount} });
                }
            }

            // Lower hemisphere
            for (int32 i = stackCount / 2; i <= stackCount; ++i) {
                stackAngle = maths::Pi / 2 - i * stackStep;
                float32 xy = radius * cosf(stackAngle);
                float32 z = radius * sinf(stackAngle) - height / 2;
                for (int32 j = 0; j <= sectorCount; ++j) {
                    sectorAngle = j * sectorStep;
                    float32 x = xy * cosf(sectorAngle);
                    float32 y = xy * sinf(sectorAngle);
                    vertices.push_back({ {x, y, z}, {0, 0, 1}, {j / (float32)sectorCount, i / (float32)stackCount} });
                }
            }

            int32 k1, k2;

            // Upper hemisphere indices
            for (int32 i = 0; i < stackCount / 2; ++i) {
                k1 = i * (sectorCount + 1);
                k2 = k1 + sectorCount + 1;
                for (int32 j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                    if (i != 0) {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);
                    }
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }

            // Cylinder indices
            for (int32 i = 0; i < sectorCount; ++i) {
                k1 = (stackCount / 2) * (sectorCount + 1) + i;
                k2 = k1 + sectorCount + 1;
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // Lower hemisphere indices
            for (int32 i = stackCount / 2; i <= stackCount + 2; ++i) {
                k1 = (i + 1) * (sectorCount + 1);
                k2 = k1 + sectorCount + 1;
                for (int32 j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                    //if (i != stackCount + 2) {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);
                    //}
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }


            /*int bottomPoleIndex = vertices.size() - 1;
            int lowerHemisphereStart = (stackCount / 2 + 2) * (sectorCount + 1);
            int offset = lowerHemisphereStart + (stackCount - stackCount / 2 - 1) * (sectorCount + 1);
            for (int j = 0; j < sectorCount; ++j) {
                indices.push_back(bottomPoleIndex);
                indices.push_back(offset + j);
                indices.push_back(offset + (j + 1) % sectorCount);
            }*/
        }
    };

    Vector3f cameraSens = Vector3f(0.0f);

    Memory::Shared<VertexBuffer> vertexBuffer = nullptr;
    Memory::Shared<IndexBuffer> indexBuffer = nullptr;
    Memory::Shared<VertexArray> vertexArray = nullptr;

    Memory::Shared<VertexBuffer> vertexBuffer2 = nullptr;
    Memory::Shared<IndexBuffer> indexBuffer2 = nullptr;
    Memory::Shared<VertexArray> vertexArray2 = nullptr;

    ShapeCube shapeCube;
    ShapeSphere shapeSphere;
    ShapeCapsule shapeCapsule;
    ShapeTube shapeTube;

    Vector2f mouse_position(100, 100);
    Vector2f sub_window_position(100, 100);
    Vector2f sub_window_size(400, 300);
    bool mouseIsPressed = false;
    bool mouseIsHover = false;

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

        Memory::Shared<ShaderInputLayout> shaderInputLayout = ShaderInputLayout::Create(m_Context);

        if (shaderInputLayout != nullptr) {
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("position", ShaderInternalType::Float3, 0));
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("color", ShaderInternalType::Float4, 1));
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("uv", ShaderInternalType::Float2, 2));

            //shaderInputLayout->uniformInput.AddAttribute(UniformInputAttribute("ObjectBuffer", ShaderStage::Vertex, BufferUsageType::DynamicDraw, sizeof(ObjectBuffer), 0, 10));
            shaderInputLayout->uniformInput.AddAttribute(UniformInputAttribute("CameraBuffer", ShaderStage::Enum::Vertex, BufferUsageType::StaticDraw, sizeof(CameraBuffer), 0, 1));

            shaderInputLayout->samplerInput.AddAttribute(SamplerInputAttribute("textureSampler", 1, ShaderStage::Enum::Fragment, SamplerType::CombineImage));

            shaderInputLayout->pushConstantInput.AddAttribute(PushConstantInputAttribute("ObjectBuffer", ShaderStage::Enum::Vertex, sizeof(ObjectBuffer)));

            if (!shaderInputLayout->Initialize()) {
                Log.Error("linitialisation des input shader ont echouer");
            }
        }

        ShaderFilePathLayout shaderFilesLayout({
            {"Resources/shaders/ubo.vert.glsl", ShaderStage::Enum::Vertex},
            {"Resources/shaders/ubo.frag.glsl", ShaderStage::Enum::Fragment},
            //{"Resources/shaders/core.vert.glsl", ShaderStage::Enum::Vertex},
            //{"Resources/shaders/core.frag.glsl", ShaderStage::Enum::Fragment},
            //{"Resources/shaders/shader.vert.glsl", ShaderStage::Enum::Vertex},
            //{"Resources/shaders/shader.frag.glsl", ShaderStage::Enum::Fragment},
            //{"Resources/shaders/triangleInternal.vert.glsl", ShaderStage::Enum::Vertex},
            //{"Resources/shaders/triangleInternal.frag.glsl", ShaderStage::Enum::Fragment},
        });

        Memory::Shared<Shader> shader = Shader::Create(m_Context, shaderFilesLayout, shaderInputLayout);
        
        if (shader == nullptr) {
            Log.Error("Cannot create shader");
        }

        Memory::Shared<UniformBuffer> uniformBuffer = UniformBuffer::Create(m_Context, shaderInputLayout, shader, {"CameraBuffer"});
        if (uniformBuffer == nullptr) {
            Log.Error("Cannot create uniform buffer");
        }

        Cube cube;

        vertexBuffer = VertexBuffer::Create<VertexTest>(m_Context, shaderInputLayout, BufferDataUsage::StaticDraw, shapeCube.vertices);
        if (vertexBuffer == nullptr) {
            Log.Error("Cannot create vertex buffer");
        }

        indexBuffer = IndexBuffer::Create(m_Context, BufferDataUsage::StaticDraw, shapeCube.indices);
        if (indexBuffer == nullptr) {
            Log.Error("Cannot create index buffer");
        }

        vertexArray = VertexArray::Create(m_Context, shaderInputLayout);
        //vertexArray = VertexArray::Create(m_Context, 3);
        if (vertexArray == nullptr) {
            Log.Error("Cannot create vertex array");
        }
        else {
            vertexArray->SetVertexBuffer(vertexBuffer);
            vertexArray->SetIndexBuffer(indexBuffer);
        }

        vertexBuffer2 = VertexBuffer::Create<VertexTest>(m_Context, shaderInputLayout, BufferDataUsage::StaticDraw, shapeCapsule.vertices);
        if (vertexBuffer2 == nullptr) {
            Log.Error("Cannot create vertex buffer");
        }

        indexBuffer2 = IndexBuffer::Create(m_Context, BufferDataUsage::StaticDraw, shapeCapsule.indices);
        if (indexBuffer2 == nullptr) {
            Log.Error("Cannot create index buffer");
        }

        vertexArray2 = VertexArray::Create(m_Context, shaderInputLayout);
        //vertexArray = VertexArray::Create(m_Context, 3);
        if (vertexArray2 == nullptr) {
            Log.Error("Cannot create vertex array");
        }
        else {
            vertexArray2->SetVertexBuffer(vertexBuffer2);
            vertexArray2->SetIndexBuffer(indexBuffer2);
        }

        Memory::Shared<Texture2D> tetxure = Texture2D::Create(m_Context, shaderInputLayout, "Resources/textures/container2.png");
        if (tetxure == nullptr) {
            Log.Error("impossible de charger la texture  Resources/textures/container2.png");
        }
        else {
            Log.Info("Texture charger");
            tetxure->SetRepeated(true);
        }
        //*/

        int32 numFrames = -1;
        float32 frameTime = 0;
        float32 time = 0.0f;

        FPSTimer fps;

        CameraBuffer cameraBuffer{};
        cameraBuffer.view = camera.GetView();
        cameraBuffer.proj = camera.GetProjection(CameraProjection::Perspective, camera.zoom(), m_Window->GetDpiAspect(), 0.1f, 100.0f);
        if (m_Context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
            cameraBuffer.proj[1][1] *= -1;
        }
        int32 slot = 0;

        CircleShape circle(100, 100, 36, Color::CadetBlue());
        circle.GenerateIndices();
        circle.GenerateVertices();

        RectangleShape rectangle(200, 100, 10, 0, Color::Cyan());
        rectangle.GenerateIndices();
        rectangle.GenerateVertices();

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

            if (m_Window != nullptr) {
                if (Input.IsKeyDown(Keyboard::C)) {
                    m_Window->ShowMouse(true);
                }
                else if (Input.IsKeyDown(Keyboard::V)) {
                    m_Window->ShowMouse(false);
                }
            }

            static bool pause = false;

            if (Input.IsKeyDown(Keyboard::L)) {
                pause = true;
            }
            else if (Input.IsKeyDown(Keyboard::K)) {
                pause = false;
            }

            static CameraProjection projection = CameraProjection::Perspective;
            static float32 fov_or_othosize = camera.zoom();
            
            if (Input.IsKeyDown(Keyboard::P)) {
                projection = CameraProjection::Perspective;
                fov_or_othosize = camera.zoom().Deg();
            } else if (Input.IsKeyDown(Keyboard::O)) {
                projection = CameraProjection::Orthographic;
                fov_or_othosize = camera.Position.Len();
            }

            if (m_Renderer == nullptr || m_Context == nullptr) { continue; }

            m_Renderer->Begin(Color::Black());
            //m_Renderer->Begin(Color::DuckBlue());
            //m_Renderer->SetViewport();
            //m_Renderer->Begin(Color::BlueTransparent);

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
                uniformBuffer->SetData("CameraBuffer", &cameraBuffer, sizeof(CameraBuffer));
                uniformBuffer->Bind();
            }

            if (tetxure != nullptr) {
                slot = (slot+1) % 10;
                tetxure->Binds(1);
            }//*/

            static float32 tmpTime = time;
            if (!pause) {
                tmpTime = time;
            }

            for (uint32 index = 0; index < 10; index++) {
                ObjectBuffer objb;
                //if (uniformBuffer != nullptr) {
                if (shaderInputLayout != nullptr) {
                    {
                        // update model
                        matrix4f scale = matrix4f::Scaling(matrix4f::Identity(), 0.5f * Vector3f(1.0f, 1.0f, 1.0f));
                        //matrix4f rotation = matrix4f::Rotation(matrix4f::Identity(), Vector3f(0.0f, 1.0f, 0.0f), (float32)tmpTime * Angle(90.0f));
                        matrix4f rotation = matrix4f::Rotation(matrix4f::Identity(), Vector3f(0.0f, 1.0f, 0.0f), (float32)tmpTime * Angle(90.0f), Vector3f());
                        matrix4f translation = matrix4f::Translation(matrix4f::Identity(), cubePositions[index]);

                        objb.model = translation * rotation * scale;
                        //objb.model = scale * rotation * translation;

                        if (index >= 3 && index <= 6) {
                            if (index == 3 || index == 5) {
                                objb.model = matrix4f::Rotation(objb.model, Vector3f(0.0f, 1.0f, 0.0f), (float32)tmpTime * Angle(-90.0f) * ((float32)index));
                            } else {
                                objb.model = matrix4f::Rotation(objb.model, Vector3f(0.0f, 1.0f, 0.0f), (float32)tmpTime * Angle(90.0f) * ((float32)index));
                            }
                        }

                        //objb.model = matrix4f::Identity();
                        //Log.Debug("model - {0}", objb.model);
                        shaderInputLayout->UpdatePushConstant("ObjectBuffer", &objb, sizeof(ObjectBuffer), shader);
                        //uniformBuffer->SetData("ObjectBuffer", &objb, sizeof(ObjectBuffer), index);
                    }
                    //uniformBuffer->Bind();
                }
                if (index < 5 && vertexArray != nullptr) {
                    //vertexArray->DrawVertex(RenderPrimitive::Triangles);
                    vertexArray->BindIndex();
                    vertexArray->DrawIndex(RenderPrimitive::Triangles);
                    vertexArray->UnbindIndex();
                }
                if (index >= 5 && vertexArray2 != nullptr) {
                    vertexArray2->BindIndex();
                    vertexArray2->DrawIndex(RenderPrimitive::Triangles);
                    vertexArray2->UnbindIndex();
                    //vertexArray2->DrawVertex(RenderPrimitive::Triangles);
                }
            }

            //*
            Memory::Shared<Canvas> canvas = nullptr;
            canvas = m_Renderer->GetCanvas();

            if (canvas != nullptr) {
                //canvas->DrawRect(maths::Vector2f(0, 0), Vector2f(200, 200), Color::Blue());
                //canvas->DrawRect(maths::Vector2f(100, 100), Vector2f(200, 200), Color::Yellow());
                //canvas->DrawRect(maths::Vector2f(-80, 120), Vector2f(250, 200), Color::Green());
                //canvas->DrawFilledCircle(maths::Vector2f(-200, 200), 50, Color::Green());
                //canvas->DrawHollowCircle(maths::Vector2f(-200, 100), 50, Color::Green());
                //canvas->DrawThickOutlineCircle(maths::Vector2f(-200, -100), 50, 20, Color::Green());

                // Exemple d'utilisation
                //canvas->DrawFilledRect({ 100.0f, 100.0f }, { 200.0f, 150.0f }, Color(1.0f, 0.0f, 0.0f));
                //canvas->DrawHollowRect({ 300.0f, 100.0f }, { 150.0f, 100.0f }, Color(0.0f, 1.0f, 0.0f));
                //canvas->DrawThickOutlineRect({ 100.0f, 100.0f }, { 150.0f, 200.0f }, 10.0f, Color(0.0f, 0.0f, 1.0f));
                //canvas->DrawRoundedRect({ 100.0f, 100.0f }, { 150.0f, 200.0f }, 10, Color(0.0f, 0.0f, 1.0f));
                //canvas->DrawFilledRoundRect({ 100.0f, 100.0f }, { 150.0f, 200.0f }, { 10.0f, 0.0f, 50.0f, 0.0f }, Color(0.0f, 0.0f, 1.0f));
                //canvas->DrawHollowRoundRect({ 100.0f, 100.0f }, { 150.0f, 200.0f }, { 10.0f, 10.0f, 0.0f, 0.0f }, 0, Color(0.0f, 0.0f, 1.0f));
                //canvas->DrawThickOutlineRoundRect({ 100.0f, 100.0f }, { 150.0f, 200.0f }, { 10.0f, 10.0f, 0.0f, 0.0f }, 0, Color(0.0f, 0.0f, 1.0f));
                //canvas->DrawHollowRoundRect({ 100.0f, 100.0f }, { 150.0f, 200.0f }, 20.0f, 0, Color(0.0f, 0.0f, 1.0f));
                //canvas->DrawThickOutlineRoundRect({ 100.0f, 100.0f }, { 150.0f, 200.0f }, 20.0f, 10.f, Color(0.0f, 0.0f, 1.0f));
                //canvas->DrawFilledRoundRect({ 100.0f, 100.0f }, { 150.0f, 200.0f }, 20.0f, Color(0.0f, 0.0f, 1.0f));

                Color color = Color(64, 64, 64);
                if (mouseIsHover) color = Color(0, 162, 232);
                // draw border
                canvas->DrawFilledRoundRect(Vector2f(-1, -1) + sub_window_position, Vector2f(2.0f, 26.0f) + sub_window_size, { 5.0f, 5.0f, 0.0f, 0.0f }, color);
                // draw header
                canvas->DrawFilledRoundRect(Vector2f(0, 0) + sub_window_position, Vector2f(sub_window_size.width, 24.0f), { 5.0f, 5.0f, 0.0f, 0.0f }, Color(20, 20, 20));
                // draw content
                canvas->DrawFilledRoundRect(Vector2f(0, 24) + sub_window_position, sub_window_size, { 0.0f, 0.0f, 0.0f, 0.0f }, Color(33, 33, 33));

                //canvas->SetScissor(sub_window_position.x, sub_window_position.y + 24, sub_window_size.width, sub_window_size.height);


                Vector2f button_size(100, 36);
                Vector2f button_position;
                button_position.x = sub_window_position.x + ((sub_window_size.width - button_size.width) / 2.0f);
                button_position.y = sub_window_position.y + sub_window_size.height - button_size.height;

                canvas->DrawFilledRoundRect(Vector2f(-1, -1) + button_position, Vector2f(2.0f, 2.0f) + button_size, { 2.50f, 2.50f, 2.50f, 2.50f }, Color(64, 64, 64));
                canvas->DrawFilledRoundRect(button_position, button_size, { 2.50f, 2.50f, 2.50f, 2.50f }, Color(20, 20, 20));

                //canvas->Draw(RenderPrimitive::LineStrip, circle);
                //canvas->Draw(RenderPrimitive::Triangles, rectangle, nullptr, maths::matrix4f::Translation(matrix4f::Identity(), Vector3f(-300, 0, 0)));
            }//*/

            if (shader != nullptr) {
                shader->Unbind();
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

        if (event.GetState() == ButtonState::Pressed) {
            mouseIsPressed = true;
            mouse_position = event.GetPosition();
        }
        else if (event.GetState() == ButtonState::Released) {
            mouseIsPressed = false;
        }

        return false;
    }

    bool Application::OnMouseMoved(MouseMovedEvent& event)
    {
        mouseDelta = event.GetSpeed();
        mouseIsHover = false;

        if (event.GetPosition().x >= sub_window_position.x && event.GetPosition().x <= sub_window_position.x + sub_window_size.width &&
            event.GetPosition().y >= sub_window_position.y && event.GetPosition().y <= sub_window_position.y + sub_window_size.height) {
            //sub_window_position += mouseDelta;
            mouseIsHover = true;
        }

        if (mouseDelta != Vector2f() && mouseIsPressed) {
            if (event.GetPosition().x >= sub_window_position.x && event.GetPosition().x <= sub_window_position.x + sub_window_size.width &&
                event.GetPosition().y >= sub_window_position.y && event.GetPosition().y <= sub_window_position.y + sub_window_size.height) {
                //sub_window_position += mouseDelta;
                mouseIsHover = true;

                Vector2f speed = (Vector2f)event.GetPosition() - mouse_position;
                sub_window_position += speed;
                mouse_position = event.GetPosition();
            }
            //sub_window_position += mouseDelta;
        }

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
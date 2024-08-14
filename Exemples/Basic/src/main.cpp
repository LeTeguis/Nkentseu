#include <iostream>

#include <NTSWindow/Nkentseu.h>

#include <NTSLogger/Log.h>
#include <NTSLogger/Assert.h>

#include <NTSWindow/Core/Events.h>
#include <NTSWindow/Core/Window.h>
#include <NTSWindow/Event/InputManager.h>
#include <NTSWindow/Event/EventBroker.h>

#include <NTSGraphics/Platform/WindowInfo.h>
#include <NTSGraphics/Core/Context.h>
#include <NTSGraphics/Core/RenderWindow.h>
#include <NTSGraphics/Core/IndexBuffer.h>
#include <NTSGraphics/Core/VertexBuffer.h>
#include <NTSGraphics/Core/VertexArray.h>

#include <NTSMaths/Matrix/Matrix4.h>
#include <NTSWindow/Core/FPSTimer.h>
#include <NTSGraphics/Core/Texture.h>

#include "Convert.h"
#include <NTSGraphics/G2D/RenderCache2D.h>

#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H

using namespace nkentseu;
using namespace nkentseu::maths;

#define Log        nkentseu::LogBase("App")
#define Assert     nkentseu::AssertBase("App")

void OnEvent(Event& event);
bool OnWindowResizedEvent(WindowResizedEvent& event);
bool OnWindowFocusedEvent(WindowFocusedEvent& event);
bool OnKeyboardEvent(KeyboardEvent& event); 
bool OnMouseScroll(MouseWheelEvent& event);
bool OnMouseMoved(MouseMovedEvent& event);

struct Vertex3D {
    Vector3f position;
    Vector4f color;
    Vector2f uv;
};

struct ObjectBuffer {
    mat4f model = maths::mat4f(1.0f);
};

struct CameraBuffer {
    mat4f view = maths::mat4f(1.0f);
    mat4f proj = maths::mat4f(1.0f);
};

enum class CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum class CameraProjection {
    Orthographic,
    Perspective
};

class CameraView {
private:
    maths::Vector3f m_forward;
    maths::Vector3f m_right;
    maths::Vector3f m_up;
    maths::Vector3f m_position;
    float32 m_aspectRatio;
    CameraProjection m_projectionType;
    maths::Angle m_fov; // Field of view for perspective projection
    float32 m_orthoSize; // Size for orthographic projection
    float32 m_nearClip;
    float32 m_farClip;
    float32 m_movementSpeed;
    float32 m_rotationSpeed;
    maths::EulerAngle m_euler;   // Yaw angle

public:
    CameraView()
        : m_position(maths::Vector3f(0, 0, -10)),
        m_aspectRatio(16.0f / 9.0f), // Default aspect ratio
        m_projectionType(CameraProjection::Perspective),
        m_fov(45.0f), // Default field of view
        m_orthoSize(10.0f), // Default orthographic size
        m_nearClip(0.1f),
        m_farClip(1000.0f),
        m_movementSpeed(5.0f), // Default movement speed
        m_rotationSpeed(1.0f), // Default rotation speed
        m_euler(0, 90, 0),
        m_forward(maths::Vector3f::Forward()),
        m_up(maths::Vector3f::Up()),
        m_right(maths::Vector3f::Right())
    {
    }

    // Getters and setters
    const maths::Vector3f& getPosition() const { return m_position; }
    void setPosition(const maths::Vector3f& position) { m_position = position; }

    const maths::EulerAngle& getOrientation() const { return m_euler; }
    void setOrientation(const maths::EulerAngle& rotation) { m_euler = rotation; }
    void MoveOrientation(const maths::EulerAngle& rotation) { m_euler += rotation; }
    void setPitch(const maths::Angle& pitch) { m_euler.pitch = pitch; }
    void setRoll(const maths::Angle& roll) { m_euler.roll = roll; }
    void setYaw(const maths::Angle& yaw) { m_euler.yaw = yaw; }
    void MovePitch(const maths::Angle& pitch) { m_euler.pitch += pitch; }
    void MoveRoll(const maths::Angle& roll) { m_euler.roll += roll; }
    void MoveYaw(const maths::Angle& yaw) { m_euler.yaw += yaw; }

    float32 getAspectRatio() const { return m_aspectRatio; }
    void setAspectRatio(float32 aspectRatio) { m_aspectRatio = aspectRatio; }

    CameraProjection getProjectionType() const { return m_projectionType; }
    void setProjectionType(CameraProjection projectionType) { m_projectionType = projectionType; }

    maths::Angle getFieldOfView() const { return m_fov; }
    void setFieldOfView(maths::Angle fov) { m_fov = fov; }

    float32 getOrthoSize() const { return m_orthoSize; }
    void setOrthoSize(float32 orthoSize) { m_orthoSize = orthoSize; }

    float32 getNearClip() const { return m_nearClip; }
    void setNearClip(float32 nearClip) { m_nearClip = nearClip; }

    float32 getFarClip() const { return m_farClip; }
    void setFarClip(float32 farClip) { m_farClip = farClip; }

    float32 getMovementSpeed() const { return m_movementSpeed; }
    void setMovementSpeed(float32 movementSpeed) { m_movementSpeed = movementSpeed; }

    float32 getRotationSpeed() const { return m_rotationSpeed; }
    void setRotationSpeed(float32 rotationSpeed) { m_rotationSpeed = rotationSpeed; }

    maths::Angle getPitch() const { return m_euler.pitch; }
    void setPitch(maths::Angle pitch) { m_euler.pitch = pitch; }

    maths::Angle getYaw() const { return m_euler.yaw; }
    void setYaw(maths::Angle yaw) { m_euler.yaw = yaw; }

    // Camera movement
    void translate(const maths::Vector3f& translation) {
        m_position += translation;
    }

    void rotate(const maths::EulerAngle& rotation) {
        m_euler += rotation;
    }

    void moveForward(float32 distance) {
        m_position -= m_forward * distance * m_movementSpeed;
    }

    void moveBackward(float32 distance) {
        m_position += m_forward * distance * m_movementSpeed;
    }

    void moveRight(float32 distance) {
        m_position += m_right * distance * m_movementSpeed;
    }

    void moveLeft(float32 distance) {
        m_position -= m_right * distance * m_movementSpeed;
    }

    void moveUp(float32 distance) {
        m_position += m_up * distance * m_movementSpeed;
    }

    void moveDown(float32 distance) {
        m_position -= m_up * distance * m_movementSpeed;
    }

    // Projection matrix
    maths::mat4f getProjection() const {
        if (m_projectionType == CameraProjection::Orthographic) {
            return maths::mat4f::Orthogonal(m_orthoSize * m_aspectRatio, m_orthoSize, m_nearClip, m_farClip);
        }
        else {
            return maths::mat4f::Perspective(m_fov, m_aspectRatio, m_nearClip, m_farClip);
        }
    }

    // Get the view matrix
    maths::mat4f getView() {
        updateDirection3();
        return maths::mat4f::LookAt(m_position, m_position + m_forward, m_up);
    }

    void updateDirection() {
        maths::Vector3f front;

        front.x = maths::Cos(m_euler.yaw) * maths::Cos(m_euler.pitch);
        front.y = maths::Sin(m_euler.pitch);
        front.z = maths::Sin(m_euler.yaw) * maths::Cos(m_euler.pitch);

        m_forward = front.Normalized();
        m_right = m_forward.Cross(maths::Vector3f::Up()).Normalized();
        m_up = m_right.Cross(m_forward).Normalized();
    }

    void updateDirection2() {
    }

    void updateDirection3() {
        maths::Vector3f front;

        // Calcul du vecteur de direction sans roulis
        front.x = maths::Cos(m_euler.yaw) * maths::Cos(m_euler.pitch);
        front.y = maths::Sin(m_euler.pitch);
        front.z = maths::Sin(m_euler.yaw) * maths::Cos(m_euler.pitch);

        m_forward = front.Normalized();

        // Calcul du vecteur de droite sans roulis
        maths::Vector3f right = m_forward.Cross(maths::Vector3f::Up()).Normalized();

        // Calcul du vecteur vers le haut sans roulis
        maths::Vector3f up = right.Cross(m_forward).Normalized();

        // Appliquer le roulis aux vecteurs de droite et vers le haut
        float32 cosRoll = maths::Cos(m_euler.roll);
        float32 sinRoll = maths::Sin(m_euler.roll);

        // Calculer les nouveaux vecteurs de droite et vers le haut après l'application du roulis
        m_right = (right * cosRoll + up * sinRoll).Normalized();
        m_up = (up * cosRoll - right * sinRoll).Normalized();
    }


    // Zoom method
    void zoom(float32 amount) {
        if (m_projectionType == CameraProjection::Orthographic) {
            m_orthoSize += amount;
            if (m_orthoSize < 0.1f) m_orthoSize = 0.1f; // Ensure ortho size doesn't go negative
        }
        else {
            m_fov += amount;
            if (m_fov < 1.0f) m_fov = 1.0f; // Ensure fov doesn't go negative or too small
        }
    }

    float32 getZoom() {
        if (m_projectionType == CameraProjection::Orthographic) {
            return m_orthoSize;
        }
        else {
            return m_fov;
        }
    }

    // Process keyboard input for camera movement
    void processKeyboard(CameraMovement direction, float32 deltaTime) {
        float32 velocity = m_movementSpeed * deltaTime;
        switch (direction) {
        case CameraMovement::FORWARD:
            moveBackward(velocity);
            break;
        case CameraMovement::BACKWARD:
            moveForward(velocity);
            break;
        case CameraMovement::RIGHT:
            moveRight(velocity);
            break;
        case CameraMovement::LEFT:
            moveLeft(velocity);
            break;
        case CameraMovement::UP:
            moveUp(velocity);
            break;
        case CameraMovement::DOWN:
            moveDown(velocity);
            break;
        }
    }

    // Process mouse movement to rotate the camera
    void processMouseMovement(float32 xoffset, float32 yoffset, bool constrainPitch = false) {
        xoffset *= m_rotationSpeed;
        yoffset *= m_rotationSpeed;

        m_euler += maths::EulerAngle(maths::Angle(yoffset), maths::Angle(xoffset), 0.0f);

        if (constrainPitch) {
            if (m_euler.pitch > maths::Angle(45.0f)) {
                m_euler.pitch = maths::Angle(45.0f);
            }
            else if (m_euler.pitch < maths::Angle(-45.0f)) {
                m_euler.pitch = maths::Angle(-45.0f);
            }
        }
    }

    // Process mouse scroll for zooming
    void processMouseScroll(float32 yoffset) {
        zoom(yoffset);
    }
};

struct Shape3D {
    std::vector<Vertex3D> vertices;
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

struct Character {
    uint32 TextureID; // ID handle of the glyph texture
    maths::Vector2f Size; // Size of glyph
    maths::Vector2f Bearing; // Offset from baseline to left/top of glyph
    uint32 Advance; // Offset to advance to next glyph
};
std::map<char, Character> Characters;

Memory::Shared<nkentseu::Window> window = nullptr;
bool running = true;

Memory::Shared<Context> context = nullptr;
Memory::Shared<RenderWindow> renderWindow = nullptr;
CameraView cameraView;
Vector2f mouse_position(100, 100);
Vector2f sub_window_position(100, 100);
Vector2f sub_window_size(400, 300);
bool mouseIsPressed = false;
bool mouseIsHover = false;
Vector2f mouseDelta;

nkentseu::int32 Main(const nkentseu::ARGV& argv) {
    Log.Trace();

    WindowProperties windowProperty;
    windowProperty.title = "Window Test";
    windowProperty.size = Vector2u(1000, 600);

    window = Window::Create(windowProperty);
    Memory::Shared<nkentseu::WindowInfo> windowInfos = Convert::ConvertWindowInfoFrom(window);

    if (window == nullptr || windowInfos == nullptr) {
        Memory::Reset(window);
        Memory::Reset(windowInfos);
        Assert.ATrue(true, "Erreur lord de la creation de la fenetre");
        return false;
    }
    else {
        Log.Trace("creation reussi");
    }

    EventTraker.AddObserver(OnEvent);

    window->ShowMouse(false);

    ContextProperties propertie(GraphicsApiType::Enum::VulkanApi);
    //ContextProperties propertie(GraphicsApiType::Enum::OpenglApi);

    context = Context::CreateInitialized(windowInfos.get(), propertie, window->GetSize(), window->ConvertPixelToDpi(window->GetSize()));
    renderWindow = RenderWindow::CreateInitialized(context, window->GetSize(), window->ConvertPixelToDpi(window->GetSize()));

    Timer timer;

    Memory::Shared<ShaderInputLayout> shaderInputLayout = ShaderInputLayout::Create(context);

    if (shaderInputLayout != nullptr) {
        shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("position", ShaderInternalType::Enum::Float3, 0));
        shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("color", ShaderInternalType::Enum::Float4, 1));
        shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("uv", ShaderInternalType::Enum::Float2, 2));

        //shaderInputLayout->uniformInput.AddAttribute(UniformInputAttribute("ObjectBuffer", ShaderStage::Vertex, BufferUsageType::DynamicDraw, sizeof(ObjectBuffer), 0, 10));
        shaderInputLayout->uniformInput.AddAttribute(UniformInputAttribute("CameraBuffer", ShaderStage::Enum::Vertex, BufferUsageType::Enum::StaticDraw, sizeof(CameraBuffer), 0, 0, 1));

        shaderInputLayout->samplerInput.AddAttribute(SamplerInputAttribute("textureSampler", 1, 1, ShaderStage::Enum::Fragment, SamplerType::Enum::CombineImage));

        shaderInputLayout->pushConstantInput.AddAttribute(PushConstantInputAttribute("ObjectBuffer", ShaderStage::Enum::Vertex, sizeof(ObjectBuffer)));

        if (!shaderInputLayout->Initialize()) {
            Log.Error("linitialisation des input shader ont echouer");
        }
    }

    ShaderFilePathLayout shaderFilesLayout({
        //{"Resources/shaders/ubo.vert.glsl", ShaderStage::Enum::Vertex},
        //{"Resources/shaders/ubo.frag.glsl", ShaderStage::Enum::Fragment},
        {"Resources/shaders/core.vert.glsl", ShaderStage::Enum::Vertex},
        {"Resources/shaders/core.frag.glsl", ShaderStage::Enum::Fragment},
        //{"Resources/shaders/shader.vert.glsl", ShaderStage::Enum::Vertex},
        //{"Resources/shaders/shader.frag.glsl", ShaderStage::Enum::Fragment},
        //{"Resources/shaders/triangleInternal.vert.glsl", ShaderStage::Enum::Vertex},
        //{"Resources/shaders/triangleInternal.frag.glsl", ShaderStage::Enum::Fragment},
        });

    Memory::Shared<Shader> shader = nullptr;
    shader = Shader::Create(context, shaderFilesLayout, shaderInputLayout);

    if (shader == nullptr) {
        Log.Error("Cannot create shader");
    }

    Memory::Shared<UniformBuffer> uniformBuffer = nullptr;
    uniformBuffer = UniformBuffer::Create(context, shaderInputLayout, shader);
    if (uniformBuffer == nullptr) {
        Log.Error("Cannot create uniform buffer");
    }

    Memory::Shared<VertexBuffer> vertexBuffer = nullptr;
    Memory::Shared<IndexBuffer> indexBuffer = nullptr;
    Memory::Shared<VertexArray> vertexArray = nullptr;
    ShapeCube shapeCube;

    vertexBuffer = VertexBuffer::Create<Vertex3D>(context, shaderInputLayout, BufferUsageType::Enum::StaticDraw, shapeCube.vertices);
    if (vertexBuffer == nullptr) {
        Log.Error("Cannot create vertex buffer");
    }

    indexBuffer = IndexBuffer::Create(context, BufferUsageType::Enum::StaticDraw, shapeCube.indices);
    if (indexBuffer == nullptr) {
        Log.Error("Cannot create index buffer");
    }

    vertexArray = VertexArray::Create(context, shaderInputLayout);
    //vertexArray = VertexArray::Create(m_Context, shaderInputLayout, 3);
    if (vertexArray == nullptr) {
        Log.Error("Cannot create vertex array");
    }
    else {
        vertexArray->SetVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);
    }

    Memory::Shared<Texture2DBinding> tetxureBinding = nullptr;
    Memory::Shared<Texture2D> tetxure = nullptr;
    //tetxure = Texture2D::Create(context, shaderInputLayout, "Resources/textures/container2.png");
    tetxure = Texture2D::Create(context, TextureFormat::Enum::RGBA8, "Resources/textures/container2.png");
    if (tetxure == nullptr) {
        Log.Error("impossible de charger la texture  Resources/textures/container2.png");
    }
    else {
        Log.Info("Texture charger");
        tetxure->SetRepeated(true);
        tetxureBinding = Texture2DBinding::Create(context, shaderInputLayout);

        if (tetxureBinding == nullptr) {
            Log.Error("impossible creer le binding pour la texture Resources/textures/container2.png");
        }
        else {
            if (!tetxureBinding->Initialize(tetxure)) {
                Log.Error("impossible de initializer le binding pour la texture Resources/textures/container2.png");
                Memory::Reset(tetxureBinding);
                tetxureBinding = nullptr;
            }
        }
    }

    Memory::Shared<Font> font = Memory::AllocateShared<Font>(context);
    if (font == nullptr || !font->LoadFromFile("Resources/Fonts/arial.ttf")) {
        Log.Error("cannot load font");
        Memory::Reset(font);
        font = nullptr;
    }

    Memory::Shared<Renderer2D> canvas = nullptr;
    canvas = Memory::AllocateShared<Renderer2D>(context);

    if (canvas != nullptr) {
        if (!canvas->Initialize()) {
            Log.Error("Cannot initialize canvas");
            canvas.reset();
            canvas = nullptr;
        }
        else {
            Log.Info("Canvas is load");
        }
    }

    Vector2f moving(400, 200);

    float32 time = 0.0f;
    FPSTimer fps;

    while (running) {
        for (auto ev : EventTraker.ProcessQueue()) {
            if (ev == nullptr) continue;
            Event& event = *ev;

            if (event.IsA<WindowStatusEvent>()) {
                WindowStatusEvent& status = event.Cast<WindowStatusEvent>();

                if (status.GetState() == WindowState::Closed) {
                    running = false;
                }
            }
        }

        float64 delta = timer.Reset().seconds;
        time += delta;
        static float32 tmpTime = time;
        static bool pause = false;

        fps.Update(delta);

        if (Input.IsKeyDown(Keyboard::Up)) {
            cameraView.processKeyboard(CameraMovement::FORWARD, delta);
        }
        else if (Input.IsKeyDown(Keyboard::Down)) {
            cameraView.processKeyboard(CameraMovement::BACKWARD, delta);
        }

        if (Input.IsKeyDown(Keyboard::Left)) {
            cameraView.processKeyboard(CameraMovement::LEFT, delta);
        }
        else if (Input.IsKeyDown(Keyboard::Right)) {
            cameraView.processKeyboard(CameraMovement::RIGHT, delta);
        }

        if (Input.IsKeyDown(Keyboard::Z)) {
            cameraView.processKeyboard(CameraMovement::UP, delta);
        }
        else if (Input.IsKeyDown(Keyboard::S)) {
            cameraView.processKeyboard(CameraMovement::DOWN, delta);
        }

        if (Input.IsKeyDown(Keyboard::Y)) {
            moving.y -= (float32)delta * 10.0f;
        }
        else if (Input.IsKeyDown(Keyboard::H)) {
            moving.y += (float32)delta * 10.0f;
        }

        if (Input.IsKeyDown(Keyboard::G)) {
            moving.x -= (float32)delta * 10.0f;
        }
        else if (Input.IsKeyDown(Keyboard::J)) {
            moving.x += (float32)delta * 10.0f;
        }

        if (Input.IsKeyDown(Keyboard::L)) {
            pause = true;
        }
        else if (Input.IsKeyDown(Keyboard::K)) {
            pause = false;
        }

        if (!pause) {
            tmpTime = time;
        }

        if (context == nullptr || !context->Prepare()) continue;

        if (renderWindow == nullptr || renderWindow->Begin(Color(31, 31, 31))) {
            if (shader != nullptr && shader->Bind()) {

                // update camera buffer
                if (uniformBuffer != nullptr) {
                    CameraBuffer cameraBuffer{};
                    cameraBuffer.view = cameraView.getView();
                    cameraBuffer.proj = cameraView.getProjection();

                    if (context->GetProperties().graphicsApi == GraphicsApiType::Enum::VulkanApi) {
                        cameraBuffer.proj[1][1] *= -1;
                    }
                    uniformBuffer->SetData("CameraBuffer", &cameraBuffer, sizeof(CameraBuffer));
                    uniformBuffer->Bind("CameraBuffer");
                }
                // use texture
                if (tetxureBinding != nullptr) {
                    tetxureBinding->Bind(1);
                }

                // draw 10 objets
                for (int32 objet = 0; objet < 10; objet++) {
                    // update push constant
                    if (shaderInputLayout != nullptr) {
                        ObjectBuffer objb;

                        mat4f translation = mat4f::Translation(cubePositions[objet] * 2);
                        mat4f rotation = mat4f::RotationY((float32)tmpTime * Angle(90.0f));

                        objb.model = translation * rotation;// * translation;

                        shaderInputLayout->UpdatePushConstant("ObjectBuffer", &objb, sizeof(ObjectBuffer), shader);
                    }

                    // draw a object store in vertex and index buffer
                    Memory::Shared<VertexArray> mesh = nullptr;

                    mesh = vertexArray;

                    if (mesh != nullptr) {
                        if (mesh->BindIndex()) {
                            if (!mesh->DrawIndex(RenderPrimitive::Enum::Triangles)) {
                                Log.Warning();
                            }
                            if (!mesh->UnbindIndex()) {
                                Log.Warning();
                            }
                        }
                        //Log.Error();
                    }
                }

                if (!shader->Unbind()) {
                    Log.Error("cannot unbind shader");
                }
            }
            else { Log.Error("cannot bind shader"); }

            if (canvas != nullptr) {
                canvas->Prepare(renderWindow);

                //canvas->AddFilledRectangle(Vector2f(0, 0), Vector2f(1000, 600), Color(31, 0, 31, 100));

                canvas->AddFilledRectangle(Vector2f(100, 100), Vector2f(100, 100), Color(31, 31, 31, 128));
                canvas->AddOutlineRectangle(moving, Vector2f(100, 100), Color::White());
                canvas->AddFilledRoundedRectangle(moving + Vector2f(200, 200), Vector2f(100, 100), 5.0f, Color::White(), 16, tetxure);
                canvas->AddOutlineRoundedRectangle(Vector2f(400, 0), Vector2f(100, 100), 5.0f, Color::White());
                //canvas->AddFilledRoundedRectangle(Vector2f(600, 100), Vector2f(300, 200), {10, 10, 0, 10}, Color::White());
                //canvas->AddOutlineRoundedRectangle(Vector2f(600, 100), Vector2f(300, 200), { 10, 10, 0, 10 }, Color::White());
                Vector2f corner[4] = { {10, 10}, {0, 0}, {10, 5}, {0, 0} };
                canvas->AddOutlineRoundedRectangle(Vector2f(600, 100), Vector2f(300, 200), corner, Color::White());
                canvas->AddFilledRoundedRectangle(Vector2f(200, 420), Vector2f(300, 200), corner, Color::White(), 16, tetxure);
                //canvas->AddOutlineEllipse(Vector2f(550, 450), { 100, 50 }, Color::YellowGreen(), 5);
                canvas->AddFilledTriangle({ 100, 100 }, { 50, 200 }, { 150, 200 }, Color::YellowGreen());
                canvas->AddOutlineTriangle({ 100, 100 }, { 50, 200 }, { 150, 200 }, Color::Red(), 5);

                //canvas->AddLine({ 100, 100 }, { 400, 400 }, Color::Green(), 10);
                //canvas->AddPath({ { 100, 100 }, { 400, 400 }, { 200, 400 }, { 100, 600 } }, Color::Green(), true, 1.0f);
                canvas->AddFillPath({ { 100, 100 }, { 400, 400 }, { 200, 400 }, { 100, 600 } }, Color::Green());
                canvas->AddText(font, "Bonjour", { 100, 100 }, 100, false, false, false, false, Color::Blue());
                canvas->Present(renderWindow);

            }

            renderWindow->End();
        }

        if (context != nullptr) {
            context->Present();
        }
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

    if (uniformBuffer != nullptr) {
        uniformBuffer->Destroy();
    }

    if (font != nullptr) font->Destroy();
    if (canvas != nullptr) canvas->Destroy();
    if (tetxure != nullptr) tetxure->Destroy();
    if (shader != nullptr) shader->Destroy();
    if (shaderInputLayout != nullptr) shaderInputLayout->Release();
    if (renderWindow != nullptr) renderWindow->Deinitialize();
    if (context != nullptr) context->Deinitialize();

    Memory::Reset(windowInfos);
    if (window != nullptr) {
        window->Close();
    }
    Log.Debug("finish");
    Memory::Reset(window);
    return 0;
}

void OnEvent(Event& event)
{
    EventBroker broker(event);

    broker.Route<WindowResizedEvent>(OnWindowResizedEvent);
    //broker.Route<WindowFocusedEvent>(OnWindowFocusedEvent);
    broker.Route<KeyboardEvent>(OnKeyboardEvent);
    broker.Route<MouseMovedEvent>(OnMouseMoved);
    broker.Route<MouseWheelEvent>(OnMouseScroll);
}

bool OnWindowResizedEvent(WindowResizedEvent& event)
{
    if (context != nullptr || window != nullptr) {
        context->OnWindowResized(event.GetSize(), window->ConvertPixelToDpi(event.GetSize()));
    }
    return false;
}

bool OnWindowFocusedEvent(WindowFocusedEvent& event)
{
    if (context != nullptr || window != nullptr) {
        if (event.GetFocusState() == FocusState::Focused) {
            context->OnWindowResized(window->GetSize(), window->ConvertPixelToDpi(window->GetSize()));
        }
        else if (event.GetFocusState() == FocusState::Unfocused) {
            context->OnWindowResized({}, {});
        }
    }
    return false;
}

bool OnKeyboardEvent(KeyboardEvent& event)
{
    if (event.GetKeycode() == Keyboard::Escape) {
        running = false;
    }

    return false;
}

bool OnMouseMoved(MouseMovedEvent& event)
{
    mouseDelta = event.GetSpeed();
    mouseIsHover = false;

    if (event.GetPosition().x >= sub_window_position.x && event.GetPosition().x <= sub_window_position.x + sub_window_size.width &&
        event.GetPosition().y >= sub_window_position.y && event.GetPosition().y <= sub_window_position.y + sub_window_size.height) {
        mouseIsHover = true;
    }

    if (mouseDelta != Vector2f() && mouseIsPressed) {
        if (event.GetPosition().x >= sub_window_position.x && event.GetPosition().x <= sub_window_position.x + sub_window_size.width &&
            event.GetPosition().y >= sub_window_position.y && event.GetPosition().y <= sub_window_position.y + sub_window_size.height) {
            mouseIsHover = true;

            Vector2f speed = (Vector2f)event.GetPosition() - mouse_position;
            sub_window_position += speed;
            mouse_position = event.GetPosition();
        }
    }

    mouseDelta.Normalize();

    cameraView.processMouseMovement(mouseDelta.x, -mouseDelta.y);

    return false;
}

bool OnMouseScroll(MouseWheelEvent& event)
{
    cameraView.processMouseScroll(event.GetDelta());
    return false;
}
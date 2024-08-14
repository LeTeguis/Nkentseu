//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-16 at 03:06:30 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "RenderCache2D.h"
#include <Logger/Formatter.h>

#include "Nkentseu/Core/Window.h"
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanTexture.h>

namespace nkentseu {
    using namespace maths;

#define UV_NORMALISED(point) ((point - position) / size)
    
    struct MVP2D {
        maths::matrix4f view = maths::matrix4f::Identity();
        maths::matrix4f proj = maths::matrix4f::Identity();
    };

    struct Material2D {
        int32 useTexture = false;
        int32 useColor = true;
    };

    RenderCache2D::RenderCache2D(const Memory::Shared<Context>& context) : beginCommand(false), currentVertice(0), currentIndice(0), m_Context(context) {}

    RenderCache2D::~RenderCache2D() {
        Clear();
    }

    bool RenderCache2D::Destroy()
    {
        if (m_Context == nullptr) return false;
        bool success = true;

        for (auto& binding : textureBindings) {
            binding->Destroy();
        }
        textureBindings.clear();

        if (vertexArray != nullptr) {
            success = vertexArray->Destroy();
        }

        if (indexBuffer != nullptr) {
            success = indexBuffer->Destroy() && success;
        }

        if (vertexBuffer != nullptr) {
            success = vertexBuffer->Destroy() && success;
        }

        if (uniformBuffer != nullptr) {
            success = uniformBuffer->Destroy() && success;
        }

        if (defaultTetxure != nullptr) {
            success = defaultTetxure->Destroy() && success;
        }

        if (m_Shader != nullptr) {
            success = m_Shader->Destroy() && success;
        }

        if (shaderInputLayout != nullptr) {
            success = shaderInputLayout->Release() && success;
        }

        return success;
    }

    bool RenderCache2D::Initialize()
    {
        shaderInputLayout = Memory::SharedCast<ShaderInputLayout>(ShaderInputLayout::Create(m_Context));

        if (shaderInputLayout != nullptr) {
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("aPos", ShaderInternalType::Enum::Float2, 0));
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("aColor", ShaderInternalType::Enum::Float4, 1));
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("aTexCoord", ShaderInternalType::Enum::Float2, 2));
            shaderInputLayout->uniformInput.AddAttribute(UniformInputAttribute("MVP2D", ShaderStage::Enum::Vertex, BufferUsageType::Enum::StaticDraw, sizeof(MVP2D), 0, 0, 1));
            shaderInputLayout->pushConstantInput.AddAttribute(PushConstantInputAttribute("Material2D", ShaderStage::Enum::Fragment, sizeof(Material2D)));
            shaderInputLayout->samplerInput.AddAttribute(SamplerInputAttribute("TextureSampler", 1, 1, ShaderStage::Enum::Fragment, SamplerType::Enum::CombineImage));

            if (!shaderInputLayout->Initialize()) {
                Log_nts.Error("linitialisation des input shader ont echouer");
                return false;
            }
        }

        ShaderFilePathLayout shaderFilesLayout({
            {"Resources/shaders/render2d.vert.glsl", ShaderStage::Enum::Vertex},
            {"Resources/shaders/render2d.frag.glsl", ShaderStage::Enum::Fragment},
            });

        m_Shader = Shader::Create(m_Context, shaderFilesLayout, shaderInputLayout);
        if (m_Shader == nullptr) {
            Log_nts.Error("Failed to create render cache shader");
            Destroy();
            return false;
        }

        uint32 size = RENDER_CACH_MAX_VERTICES;
        vertexBuffer = VertexBuffer::Create(m_Context, shaderInputLayout, BufferUsageType::Enum::DynamicDraw, nullptr, size);
        if (!vertexBuffer) {
            Log_nts.Error("Failed to create render cache vertex buffer");
            Destroy();
            return false;
        }

        size = RENDER_CACH_MAX_INDICES;
        indexBuffer = IndexBuffer::Create(m_Context, BufferUsageType::Enum::DynamicDraw, DrawIndexType::UInt32, nullptr, size);
        if (!indexBuffer) {
            Log_nts.Error("Failed to create render cache index buffer");
            Destroy();
            return false;
        }

        vertexArray = VertexArray::Create(m_Context, shaderInputLayout);
        if (!vertexArray) {
            Log_nts.Error("Failed to create render cache vertex array");
            Destroy();
            return false;
        }
        else {
            vertexArray->SetVertexBuffer(vertexBuffer);
            vertexArray->SetIndexBuffer(indexBuffer);
        }

        uniformBuffer = UniformBuffer::Create(m_Context, shaderInputLayout, m_Shader, { "MVP2D" });
        if (uniformBuffer == nullptr) {
            Log_nts.Error("Cannot create uniform buffer");
            Destroy();
            return false;
        }

        Image image;
        image.Create({ 100, 100 }, Color::Black());

        defaultTetxure = Texture2D::Create(m_Context, shaderInputLayout);

        if (defaultTetxure == nullptr) {
            Log_nts.Error("Cannot create texture");
            Destroy();
            return false;
        }

        if (!defaultTetxure->Create(image)) {
            Log_nts.Error("Cannot load black texture");
            Destroy();
            return false;
        }

        return true;
    }

    void RenderCache2D::Prepare(Memory::Shared<Renderer> renderer)
    {
        for (auto& binding : textureBindings) {
            binding->Destroy();
        }
        textureBindings.clear();
    }

    void RenderCache2D::Present(Memory::Shared<Renderer> renderer)
    {
        if (renderer == nullptr || vertices.empty() || indices.empty() || commands.empty()) {
            return;
        }

        renderer->EnableDepthTest(false);
        renderer->EnableScissorTest(true);
        renderer->ResetViewport();
        renderer->ResetScissor();

        if (m_Shader == nullptr || !m_Shader->Bind()) {
            return;
        }

        if (vertexArray == nullptr) {
            m_Shader->Unbind();
            return;
        }

        if (vertexArray->GetVertexBuffer() == nullptr || vertexArray->GetIndexBuffer() == nullptr) {
            m_Shader->Unbind();
            return;
        }

        if (!vertexArray->GetVertexBuffer()->SetData(vertices.data(), vertices.size() * sizeof(Vertex2D))) {
            Log_nts.Error("Cannot update vertex buffer");
        }

        if (!vertexArray->GetIndexBuffer()->SetData(indices.data(), indices.size() * sizeof(uint32))) {
            Log_nts.Error("Cannot update index buffer");
        }

        if (uniformBuffer != nullptr) {
            Vector2f size = m_Context->GetWindow()->GetSize();

            MVP2D cameraBuffer{};
            cameraBuffer.view = matrix4f::Identity();

            if (m_Context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
                cameraBuffer.proj = matrix4f::Orthogonal(Vector2f(0, 0), Vector2f(size.width, size.height), -1.f, 1.0f);
            }
            else {
                cameraBuffer.proj = matrix4f::Orthogonal(Vector2f(0, size.height), Vector2f(size.width, 0), -1.f, 1.0f);
            }

            uniformBuffer->SetData("MVP2D", &cameraBuffer, sizeof(MVP2D));
            uniformBuffer->Bind();
        }

        if (vertexArray->BindIndex()) {
            if (defaultTetxure != nullptr) {
                defaultTetxure->Bind(1);
            }

            uint32 offset = 0;

            int32 begin = 0;
            int32 end = commands.size();
            int32 pas = 1;

            if (m_Context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
                begin = commands.size() - 1;
                end = -1;
                pas = -1;
            }

            for (int32 index = begin; index != end; index += pas) {
                const auto command = commands[index];

                bool useTexture = false;

                if (command.textureId >= 0 && command.textureId < textureBindings.size() && textureBindings[command.textureId] != nullptr)
                {
                    textureBindings[command.textureId]->Bind(1);
                    useTexture = true;
                }

                if (command.clipRegion.dimensions != Vector2f()) {
                    renderer->SetScissor(command.clipRegion.controlPoints);
                }

                if (shaderInputLayout != nullptr) {
                    Material2D material;
                    material.useColor = 1 << 1;
                    material.useTexture |= useTexture ? 1 << 2 : 0;
                    shaderInputLayout->UpdatePushConstant("Material2D", &material, sizeof(Material2D), m_Shader);
                }
                vertexArray->DrawIndex(command.primitive, command.indexOffset, command.indexCount);
            }
            vertexArray->UnbindIndex();
        }

        if (!m_Shader->Unbind()) {
        }

        renderer->EnableScissorTest(false);
        renderer->EnableDepthTest(true);
        Clear();
    }

    void RenderCache2D::BeginCommand(RenderPrimitive primitive) {
        if (beginCommand) return;
        currentCommand.primitive = primitive;
        currentVertices.clear();
        currentIndices.clear();
        beginCommand = true;
    }

    void RenderCache2D::AddCommand(const RenderCommand2D& command) {
        if (!beginCommand) return;
        commands.push_back(command);
    }

    void RenderCache2D::SetCommandClipRegion(const ClipRegion& clipRegion) {
        if (!beginCommand) return;
        currentCommand.clipRegion = clipRegion;
    }

    void RenderCache2D::SetCommandTextureId(Memory::Shared<Texture2D> texture) {
        if (!beginCommand || texture == nullptr) return;

        for (auto& binding : textureBindings) {
            binding->Destroy();
        }
        textureBindings.clear();
        int32 id = 0;
        for (auto binding : textureBindings) {
            if (binding->IsDefined(texture)) {
                currentCommand.textureId = id;
                return;
            }
            id++;
        }

        Memory::Shared<Texture2DBinding> textureBinding = Texture2DBinding::CreateInitialize(m_Context, shaderInputLayout, texture);

        if (textureBinding != nullptr) {
            textureBindings.push_back(textureBinding);
            currentCommand.textureId = textureBindings.size() - 1;
        }
        else {
            Log_nts.Error("Cannot add texture");
        }
    }

    RenderCommand2D* RenderCache2D::CreateCommand(RenderPrimitive primitive, const ClipRegion& clipRegion, Memory::Shared<Texture2D> texture) {
        if (!beginCommand) return nullptr;
        currentCommand.primitive = primitive;
        currentCommand.clipRegion = clipRegion;
        SetCommandTextureId(texture);
        return &currentCommand;
    }

    RenderCommand2D* RenderCache2D::GetCurrentCommand() {
        if (!beginCommand) return nullptr;
        return &currentCommand;
    }

    RenderCommand2D* RenderCache2D::GetCommand(uint32 index) {
        if (index < commands.size()) {
            return &commands[index];
        }
        return nullptr;
    }

    RenderCommand2D* RenderCache2D::GetLastCommand() {
        if (commands.size() == 0) {
            return nullptr; // index out of bounds
        }
        return &commands[commands.size() - 1]; // return the last command at the given index
    }

    RenderCommand2D* RenderCache2D::GetBeginCommand() {
        if (commands.size() == 0) {
            return nullptr; // index out of bounds
        }
        return &commands[0]; // return the first command at the given index
    }

    void RenderCache2D::AddVertex(const maths::Vector2f& position, const maths::Vector4f& color, const maths::Vector2f& uv) {
        if (!beginCommand) return;
        Vertex2D vertex;
        vertex.position = position;
        vertex.color = color;
        vertex.texCord = uv;
        currentVertices.push_back(vertex);
    }

    void RenderCache2D::AddVertex(const Vertex2D& vertex)
    {
        if (!beginCommand) return;
        currentVertices.push_back(vertex);
    }

    void RenderCache2D::AddIndex(uint32 index) {
        if (!beginCommand) return;
        currentIndices.push_back(index + currentVertice);
    }

    RenderCommand2D* RenderCache2D::AddShape(Shape2D* shape)
    {
        if (shape == nullptr) return nullptr;

        BeginCommand(shape->GetPrimitive());

        SetCommandTextureId(shape->GetTexture());
        SetCommandClipRegion(shape->GetClipRegion());

        for (auto vertice : shape->GetVertices()) {
            AddVertex(vertice);
        }

        for (auto indice : shape->GetIndices()) {
            AddIndex(indice);
        }

        EndCommand(true);
        return GetLastCommand();
    }

    void RenderCache2D::EndCommand(bool send) {
        if (!beginCommand) return;
        if (send) {
            currentCommand.indexCount = currentIndices.size();
            currentCommand.indexOffset = currentIndice;
            currentCommand.vertexCount = currentVertices.size();
            currentCommand.vertexOffset = currentVertice;

            currentVertice += currentCommand.vertexCount;
            currentIndice += currentCommand.indexCount;

            vertices.insert(vertices.end(), currentVertices.begin(), currentVertices.end());
            indices.insert(indices.end(), currentIndices.begin(), currentIndices.end());

            commands.push_back(currentCommand);
        }
        currentCommand = RenderCommand2D();
        currentVertices.clear();
        currentIndices.clear();
        beginCommand = false;
    }

    void RenderCache2D::Clear() {
        commands.clear();
        currentCommand = RenderCommand2D();
        currentVertice = 0;
        currentIndice = 0;
        beginCommand = false;
        vertices.clear();
        indices.clear();
        currentVertices.clear();
        currentIndices.clear();
    }

    RenderCommand2D* RenderCache2D::AddFilledRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        Vector2f uv1 = uvs.size() > 0 ? uvs[0] : Vector2f(0, 0);
        Vector2f uv2 = uvs.size() > 1 ? uvs[1] : Vector2f(0, 1);
        Vector2f uv3 = uvs.size() > 2 ? uvs[2] : Vector2f(1, 1);
        Vector2f uv4 = uvs.size() > 3 ? uvs[3] : Vector2f(1, 0);

        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        AddVertex(position, color, uv1);
        AddVertex(position + Vector2f(0, size.y), color, uv2);
        AddVertex(position + size, color, uv3);
        AddVertex(position + Vector2f(size.x, 0), color, uv4);

        AddIndex(0);
        AddIndex(1);
        AddIndex(2);
        AddIndex(2);
        AddIndex(3);
        AddIndex(0);

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddOutlineRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, float32 lineWidth, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        float32 line_width = maths::Min<float32>({lineWidth, size.width, size.height});

        Vector2f uv1 = uvs.size() > 0 ? uvs[0] : Vector2f(0, 0);
        Vector2f uv2 = uvs.size() > 1 ? uvs[1] : Vector2f(0, 1);
        Vector2f uv3 = uvs.size() > 2 ? uvs[2] : Vector2f(1, 1);
        Vector2f uv4 = uvs.size() > 3 ? uvs[3] : Vector2f(1, 0);

        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        AddVertex(position, color, uv1);
        AddVertex(position + Vector2f(0, size.y), color, uv2);
        AddVertex(position + size, color, uv3);
        AddVertex(position + Vector2f(size.x, 0), color, uv4);

        AddVertex(position + line_width, color, uv1);
        AddVertex(position + Vector2f(line_width, size.y - line_width), color, uv2);
        AddVertex(position + size - line_width, color, uv3);
        AddVertex(position + Vector2f(size.x - line_width, line_width), color, uv4);

        for (int32 i = 0; i < 4; i++) {
            int32 next = (i + 1) % 4;

            AddIndex(i);
            AddIndex(i + 4);
            AddIndex(next + 4);

            AddIndex(i);
            AddIndex(next + 4);
            AddIndex(next);
        }

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddFilledRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, float32 radius, const Color& color, uint32 segments, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        Vector2f textureSize(1);

        if (texture != nullptr) {
            textureSize = texture->GetSize();
        }

        float32 radius_ajust = maths::Min<float32>({ radius, size.width * 0.5f, size.height * 0.5f });
        const float32 segments_haps = 1.0f / (float32)segments;
        const uint32 indice_count = segments + 1;
        const uint32 indice_corner_count = indice_count + 1;

        std::vector<Vector2f> centers;
        centers.push_back(position + Vector2f(radius_ajust, radius_ajust));
        centers.push_back(position + Vector2f(radius_ajust, size.height - radius_ajust));
        centers.push_back(position + Vector2f(size.width - radius_ajust, size.height - radius_ajust));
        centers.push_back(position + Vector2f(size.width - radius_ajust, radius_ajust));

        std::vector<float32> angles;
        angles.push_back(maths::Pi * 0.5f);
        angles.push_back(-maths::Pi);
        angles.push_back(-maths::Pi * 0.5f);
        angles.push_back(0.0f);

        uint32 vertices_count = 0;
        for (int32 corner = 0; corner < 4; corner++) {
            Vector2f uv = uvs.size() > corner * indice_count ? uvs[corner * indice_count] : UV_NORMALISED(centers[corner]);
            AddVertex(centers[corner], color, uv);
            vertices_count++;
            vertices_count = AddInternalArc(vertices_count, centers[corner], position, size, Vector2f(radius_ajust), Vector2f(size.x * 0.1f, size.y * 0.1f), color, angles[corner], angles[corner] + maths::Pi * 0.5f, segments, uvs);
        }

        float32 center_indices[4] = {0, indice_corner_count, indice_corner_count * 2, indice_corner_count * 3};

        for (int32 corner = 0; corner < 4; corner++) {
            // Ajout des indices pour dessiner les triangles
            for (int32 index = 1; index < indice_count; ++index) {
                AddIndex(center_indices[corner]);
                AddIndex(center_indices[corner] + index);
                AddIndex(center_indices[corner] + index + 1);
            }

            AddIndex(center_indices[corner]);
            AddIndex(center_indices[corner] + indice_count);
            AddIndex(center_indices[(corner + 1) % 4] + 1);

            AddIndex(center_indices[(corner + 1) % 4] + 1);
            AddIndex(center_indices[(corner + 1) % 4]);
            AddIndex(center_indices[corner]);
        }

        AddIndex(center_indices[0]);
        AddIndex(center_indices[1]);
        AddIndex(center_indices[2]);

        AddIndex(center_indices[2]);
        AddIndex(center_indices[3]);
        AddIndex(center_indices[0]);

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddOutlineRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, float32 radius, const Color& color, float32 lineWidth, uint32 segments, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        std::vector<float32> angles;
        angles.push_back(maths::Pi * 0.5f);
        angles.push_back(-maths::Pi);
        angles.push_back(-maths::Pi * 0.5f);
        angles.push_back(0.0f);

        uint32 vertices_count = 0;

        const float32 segments_haps = 1.0f / (float32)segments;
        const uint32 indice_count = segments + 1;
        const uint32 indice_rect_count = indice_count * 4;
        const float32 line_width = maths::Min<float32>({ lineWidth, size.width, size.height });

        for (int32 box = 0; box < 2; box++) {

            const float32 radius_ajust = maths::Min<float32>({ radius, (size.width - line_width * box) * 0.5f, (size.height - line_width * box) * 0.5f });

            std::vector<Vector2f> centers;
            centers.push_back(position + Vector2f(radius_ajust, radius_ajust) + line_width * box);
            centers.push_back(position + Vector2f(radius_ajust, size.height - radius_ajust) + box * Vector2f(line_width,- line_width));
            centers.push_back(position + Vector2f(size.width - radius_ajust, size.height - radius_ajust) - line_width * box);
            centers.push_back(position + Vector2f(size.width - radius_ajust, radius_ajust) + box * Vector2f(-line_width, line_width));


            for (int32 corner = 0; corner < 4; corner++) {
                vertices_count = AddInternalArc(vertices_count, centers[corner], position, size, Vector2f(radius_ajust), Vector2f(size.x * 0.1f, size.y * 0.1f), color, angles[corner], angles[corner] + maths::Pi * 0.5f, segments, uvs);
            }
        }

        for (int32 index = 0; index < indice_rect_count; ++index) {
            uint32 next = (index + 1) % indice_rect_count;

            AddIndex(index);
            AddIndex(next);
            AddIndex(indice_rect_count + index);

            AddIndex(indice_rect_count + index);
            AddIndex(next);
            AddIndex(indice_rect_count + next);
        }

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddFilledRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& radius, const Color& color, uint32 segments, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        std::vector<float32> angles;
        angles.push_back(maths::Pi);
        angles.push_back(maths::Pi * 1.5f);
        angles.push_back(0.0f);
        angles.push_back(maths::Pi * 0.5f);

        std::vector<float32> radius_ajust = {
            maths::Min<float32>({radius[0], size.width * 0.5f, size.height * 0.5f}),
            maths::Min<float32>({radius[1], size.width * 0.5f, size.height * 0.5f}),
            maths::Min<float32>({radius[2], size.width * 0.5f, size.height * 0.5f}),
            maths::Min<float32>({radius[3], size.width * 0.5f, size.height * 0.5f})
        };

        const float32 rad = maths::Max<float32>(radius_ajust);
        const float32 segments_haps = 1.0f / (float32)segments;
        const uint32 indice_count = segments + 1;
        const uint32 indice_corner_count = indice_count + 1;

        std::vector<Vector2f> centers;
        centers.push_back(position + (radius_ajust[0] == 0 ? rad : radius_ajust[0]));
        centers.push_back(position + Vector2f(0, size.height) + (radius_ajust[1] == 0 ? Vector2f(rad, -rad) : Vector2f(radius_ajust[1], -radius_ajust[1])));
        centers.push_back(position + size + (radius_ajust[2] == 0 ? Vector2f(-rad, -rad) : Vector2f(-radius_ajust[2], -radius_ajust[2])));
        centers.push_back(position + Vector2f(size.width, 0) + (radius_ajust[3] == 0 ? Vector2f(-rad, rad) : Vector2f(-radius_ajust[3], radius_ajust[3])));

        for (int32 corner = 0; corner < 4; corner++) {
            Vector2f uv = uvs.size() > corner * indice_count ? uvs[corner * indice_count] : UV_NORMALISED(centers[corner]);
            AddVertex(centers[corner], color, uv);

            for (int32 i = segments; i >= 0; --i) {
                float32 theta = (static_cast<float32>(i) / static_cast<float32>(segments) * 0.5 * maths::Pi) - angles[corner];

                Vector2f trigo(maths::Cos(Angle::FromRadian(theta)), maths::Sin(Angle::FromRadian(theta)));

                float32 radius_calculus = radius_ajust[corner];

                if (radius_ajust[corner] == 0) {
                    trigo.x = (trigo.x < 0) ? -1 : 1;
                    trigo.y = (trigo.y < 0) ? -1 : 1;
                    radius_calculus = rad;
                }

                Vector2f vertex = centers[corner] + trigo * radius_calculus;

                uv = uvs.size() > corner * indice_count ? uvs[corner * indice_count + i + 1] : trigo;

                AddVertex(vertex, color, uv);
            }
        }

        float32 center_indices[4] = { 0, indice_corner_count, indice_corner_count * 2, indice_corner_count * 3 };

        for (int32 corner = 0; corner < 4; corner++) {
            // Ajout des indices pour dessiner les triangles
            for (int32 index = 1; index < indice_count; ++index) {
                AddIndex(center_indices[corner]);
                AddIndex(center_indices[corner] + index);
                AddIndex(center_indices[corner] + index + 1);
            }

            AddIndex(center_indices[corner]);
            AddIndex(center_indices[corner] + indice_count);
            AddIndex(center_indices[(corner + 1) % 4] + 1);

            AddIndex(center_indices[(corner + 1) % 4] + 1);
            AddIndex(center_indices[(corner + 1) % 4]);
            AddIndex(center_indices[corner]);
        }

        AddIndex(center_indices[0]);
        AddIndex(center_indices[1]);
        AddIndex(center_indices[2]);

        AddIndex(center_indices[2]);
        AddIndex(center_indices[3]);
        AddIndex(center_indices[0]);

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddOutlineRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& radius, const Color& color, float32 lineWidth, uint32 segments, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        std::vector<float32> angles;
        angles.push_back(maths::Pi);
        angles.push_back(maths::Pi * 1.5f);
        angles.push_back(0.0f);
        angles.push_back(maths::Pi * 0.5f);

        const float32 segments_haps = 1.0f / (float32)segments;
        const uint32 indice_count = segments + 1;
        uint32 indice_rect_count = 0;
        const float32 line_width = maths::Min<float32>({ lineWidth, size.width, size.height });

        for (int32 box = 0; box < 2; box++) {
            indice_rect_count = 0;

            std::vector<float32> radius_ajust = {
                maths::Min<float32>({radius[0], (size.width - line_width * box) * 0.5f, (size.height - line_width * box) * 0.5f}),
                maths::Min<float32>({radius[1], (size.width - line_width * box) * 0.5f, (size.height - line_width * box) * 0.5f}),
                maths::Min<float32>({radius[2], (size.width - line_width * box) * 0.5f, (size.height - line_width * box) * 0.5f}),
                maths::Min<float32>({radius[3], (size.width - line_width * box) * 0.5f, (size.height - line_width * box) * 0.5f})
            };

            const float32 rad = maths::Max<float32>(radius_ajust);

            std::vector<Vector2f> centers;
            centers.push_back(position + (radius_ajust[0] == 0 ? rad : radius_ajust[0]) + line_width * box);
            centers.push_back(position + Vector2f(0, size.height) + (radius_ajust[1] == 0 ? Vector2f(rad, -rad) : Vector2f(radius_ajust[1], -radius_ajust[1])) + box * Vector2f(line_width, -line_width));
            centers.push_back(position + size + (radius_ajust[2] == 0 ? Vector2f(-rad, -rad) : Vector2f(-radius_ajust[2], -radius_ajust[2])) - line_width * box);
            centers.push_back(position + Vector2f(size.width, 0) + (radius_ajust[3] == 0 ? Vector2f(-rad, rad) : Vector2f(-radius_ajust[3], radius_ajust[3])) + box * Vector2f(-line_width, line_width));

            for (int32 corner = 0; corner < 4; corner++) {
                for (int32 i = segments; i >= 0; --i) {
                    float32 theta = (static_cast<float32>(i) / static_cast<float32>(segments) * 0.5 * maths::Pi) - angles[corner];

                    Vector2f trigo(maths::Cos(Angle::FromRadian(theta)), maths::Sin(Angle::FromRadian(theta)));

                    float32 radius_calculus = radius_ajust[corner];

                    if (radius_ajust[corner] == 0) {
                        trigo.x = (trigo.x < 0) ? -1 : 1;
                        trigo.y = (trigo.y < 0) ? -1 : 1;
                        radius_calculus = rad;
                    }

                    Vector2f vertex = centers[corner] + trigo * radius_calculus;

                    Vector2f uv = uvs.size() > corner * indice_count ? uvs[corner * indice_count + i + 1] : trigo;

                    AddVertex(vertex, color, uv);
                    indice_rect_count++;
                }
            }
        }

        for (int32 index = 0; index < indice_rect_count; ++index) {
            uint32 next = (index + 1) % indice_rect_count;

            AddIndex(index);
            AddIndex(next);
            AddIndex(indice_rect_count + index);

            AddIndex(indice_rect_count + index);
            AddIndex(next);
            AddIndex(indice_rect_count + next);
        }

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddFilledRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector2f radius[4], const Color& color, uint32 segments, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        std::vector<float32> angles;
        angles.push_back(maths::Pis2);
        angles.push_back(-maths::Pi);
        angles.push_back(-maths::Pis2);
        angles.push_back(0.0f);

        std::vector<float32> radius_ajustX = {
            maths::Min<float32>({radius[0].x, size.width * 0.5f}), maths::Min<float32>({radius[1].x, size.width * 0.5f}),
            maths::Min<float32>({radius[2].x, size.width * 0.5f}), maths::Min<float32>({radius[3].x, size.width * 0.5f})
        };
        std::vector<float32> radius_ajustY = {
            maths::Min<float32>({radius[0].y, size.height * 0.5f}), maths::Min<float32>({radius[1].y, size.height * 0.5f}),
            maths::Min<float32>({radius[2].y, size.height * 0.5f}), maths::Min<float32>({radius[3].y, size.height * 0.5f}),
        };

        const float32 radX = maths::Max<float32>(radius_ajustX);
        const float32 radY = maths::Max<float32>(radius_ajustY);
        const float32 segments_haps = 1.0f / (float32)segments;
        const uint32 indice_count = segments + 1;
        uint32 indice_rect_count = 0;
        const uint32 indice_corner_count = indice_count + 1;

        #define Centering0(vx, vy) Vector2f((vx == 0) ? radX : vx, (vy == 0) ? radY : vy)
        #define Centering1(vx, vy) Vector2f((vx == 0) ? radX : vx, (vy == 0) ? -radY : -vy)
        #define Centering2(vx, vy) Vector2f((vx == 0) ? -radX : -vx, (vy == 0) ? -radY : -vy)
        #define Centering3(vx, vy) Vector2f((vx == 0) ? -radX : -vx, (vy == 0) ? radY : vy)

        std::vector<Vector2f> centers;
        centers.push_back(position + Centering0(radius_ajustX[0], radius_ajustY[0]));
        centers.push_back(position + Vector2f(0, size.height) + Centering1(radius_ajustX[1], radius_ajustY[1]));
        centers.push_back(position + size + Centering2(radius_ajustX[2], radius_ajustY[2]));
        centers.push_back(position + Vector2f(size.width, 0) + Centering3(radius_ajustX[3], radius_ajustY[3]));

        for (int32 corner = 0; corner < 4; corner++) {
            Vector2f uv = uvs.size() > corner * indice_count ? uvs[corner * indice_count] : UV_NORMALISED(centers[corner]);
            AddVertex(centers[corner], color, uv);
            indice_rect_count = AddInternalArc(indice_rect_count, centers[corner], position, size, Vector2f(radius_ajustX[corner], radius_ajustY[corner]), Vector2f(radX, radY), color, angles[corner], angles[corner] + maths::Pi * 0.5f, segments, uvs);
        }

        float32 center_indices[4] = { 0, indice_corner_count, indice_corner_count * 2, indice_corner_count * 3 };

        for (int32 corner = 0; corner < 4; corner++) {
            // Ajout des indices pour dessiner les triangles
            for (int32 index = 1; index < indice_count; ++index) {
                AddIndex(center_indices[corner]);
                AddIndex(center_indices[corner] + index);
                AddIndex(center_indices[corner] + index + 1);
            }

            AddIndex(center_indices[corner]);
            AddIndex(center_indices[corner] + indice_count);
            AddIndex(center_indices[(corner + 1) % 4] + 1);

            AddIndex(center_indices[(corner + 1) % 4] + 1);
            AddIndex(center_indices[(corner + 1) % 4]);
            AddIndex(center_indices[corner]);
        }

        AddIndex(center_indices[0]);
        AddIndex(center_indices[1]);
        AddIndex(center_indices[2]);

        AddIndex(center_indices[2]);
        AddIndex(center_indices[3]);
        AddIndex(center_indices[0]);

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddOutlineRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector2f radius[4], const Color& color, float32 lineWidth, uint32 segments, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        std::vector<float32> angles;
        angles.push_back(maths::Pis2);
        angles.push_back(-maths::Pi);
        angles.push_back(-maths::Pis2);
        angles.push_back(0.0f);

        const float32 segments_haps = 1.0f / (float32)segments;
        const uint32 indice_count = segments + 1;
        uint32 indice_rect_count = 0;
        const float32 line_width = maths::Min<float32>({ lineWidth, size.width, size.height });

        for (int32 box = 0; box < 2; box++) {
            indice_rect_count = 0;

            std::vector<float32> radius_ajustX = {
                maths::Min<float32>({radius[0].x, (size.width - line_width * box) * 0.5f}), 
                maths::Min<float32>({radius[1].x, (size.width - line_width * box) * 0.5f}),
                maths::Min<float32>({radius[2].x, (size.width - line_width * box) * 0.5f}), 
                maths::Min<float32>({radius[3].x, (size.width - line_width * box) * 0.5f})
            };
            std::vector<float32> radius_ajustY = {
                maths::Min<float32>({radius[0].y, (size.height - line_width * box) * 0.5f}), 
                maths::Min<float32>({radius[1].y, (size.height - line_width * box) * 0.5f}),
                maths::Min<float32>({radius[2].y, (size.height - line_width * box) * 0.5f}), 
                maths::Min<float32>({radius[3].y, (size.height - line_width * box) * 0.5f}),
            };

            const float32 radX = maths::Max<float32>(radius_ajustX);
            const float32 radY = maths::Max<float32>(radius_ajustY);

            #define Centering10(vx, vy) (Vector2f((vx == 0) ? radX : vx, (vy == 0) ? radY : vy) + line_width * box)
            #define Centering11(vx, vy) (Vector2f((vx == 0) ? radX : vx, (vy == 0) ? -radY : -vy) + box * Vector2f(line_width, -line_width))
            #define Centering12(vx, vy) (Vector2f((vx == 0) ? -radX : -vx, (vy == 0) ? -radY : -vy) - line_width * box)
            #define Centering13(vx, vy) (Vector2f((vx == 0) ? -radX : -vx, (vy == 0) ? radY : vy) + box * Vector2f(-line_width, line_width))

            std::vector<Vector2f> centers;
            centers.push_back(position + Centering10(radius_ajustX[0], radius_ajustY[0]));
            centers.push_back(position + Vector2f(0, size.height) + Centering11(radius_ajustX[1], radius_ajustY[1]));
            centers.push_back(position + size + Centering12(radius_ajustX[2], radius_ajustY[2]));
            centers.push_back(position + Vector2f(size.width, 0) + Centering13(radius_ajustX[3], radius_ajustY[3]));

            for (int32 corner = 0; corner < 4; corner++) {
                indice_rect_count = AddInternalArc(indice_rect_count, centers[corner], position, size, Vector2f(radius_ajustX[corner], radius_ajustY[corner]), Vector2f(radX, radY), color, angles[corner], angles[corner] + maths::Pi * 0.5f, segments, uvs);
            }
        }

        for (int32 index = 0; index < indice_rect_count; ++index) {
            uint32 next = (index + 1) % indice_rect_count;

            AddIndex(index);
            AddIndex(next);
            AddIndex(indice_rect_count + index);

            AddIndex(indice_rect_count + index);
            AddIndex(next);
            AddIndex(indice_rect_count + next);
        }

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddFilledCircle(const maths::Vector2f& position, float32 radius, const Color& color, uint32 segments, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs) {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        Vector2f size(radius * 2.0f);

        float32 angleIncrement = 2 * maths::Pi / segments;
        AddVertex(position, color, uvs.size() <= 0 ? UV_NORMALISED(position + (size * 0.5f)) : uvs[0]);

        for (uint32 i = 0; i <= segments; i++) {
            float32 angle = i * angleIncrement;
            Vector2f trigo(cos(angle), sin(angle));
            maths::Vector2f p = position + trigo * radius;

            AddVertex(p, color, uvs.size() <= i + 1 ? UV_NORMALISED(p) : uvs[i + 1]);
        }

        for (uint32 i = 1; i <= segments; i++) {
            AddIndex(0);
            AddIndex(i);
            AddIndex(i + 1);
        }

        AddIndex(0);
        AddIndex(segments);
        AddIndex(1);

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddOutlineCircle(const maths::Vector2f& position, float32 radius, const Color& color, float32 lineWidth, uint32 segments, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs) {

        if (radius == lineWidth) {
            return AddFilledCircle(position, radius, color, segments, texture, uvs);
        }

        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        Vector2f size(radius * 2.0f);

        float32 angleIncrement = 2 * maths::Pi / segments;
        uint32 circle_vertex_count = 0;

        for (uint32 circle = 0; circle < 2; circle++) {
            circle_vertex_count = 0;
            for (uint32 i = 0; i <= segments; i++) {
                float32 angle = i * angleIncrement;
                Vector2f trigo(cos(angle), sin(angle));
                maths::Vector2f p = position + trigo * (radius - lineWidth * circle);

                AddVertex(p, color, uvs.size() <= i ? UV_NORMALISED(p) : uvs[i]);
                circle_vertex_count++;
            }
        }

        for (uint32 index = 0; index < circle_vertex_count; ++index) {
            uint32 next = (index + 1) % circle_vertex_count;

            AddIndex(index);
            AddIndex(next);
            AddIndex(circle_vertex_count + index);

            AddIndex(circle_vertex_count + index);
            AddIndex(next);
            AddIndex(circle_vertex_count + next);
        }

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddFilledEllipse(const maths::Vector2f& position, const maths::Vector2f& radius, const Color& color, uint32 segments, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        Vector2f size(radius * 2.0f);

        float32 angleIncrement = 2 * maths::Pi / segments;
        AddVertex(position, color, uvs.size() <= 0 ? UV_NORMALISED(position + (size * 0.5f)) : uvs[0]);

        for (uint32 i = 0; i <= segments; i++) {
            float32 angle = i * angleIncrement;
            Vector2f trigo(cos(angle), sin(angle));
            maths::Vector2f p = position + trigo * radius;

            AddVertex(p, color, uvs.size() <= i + 1 ? UV_NORMALISED(p) : uvs[i + 1]);
        }

        for (uint32 i = 1; i <= segments; i++) {
            AddIndex(0);
            AddIndex(i);
            AddIndex(i + 1);
        }

        AddIndex(0);
        AddIndex(segments);
        AddIndex(1);

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddOutlineEllipse(const maths::Vector2f& position, const maths::Vector2f& radius, const Color& color, float32 lineWidth, uint32 segments, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        Vector2f size(radius * 2.0f);

        float32 angleIncrement = 2 * maths::Pi / segments;
        uint32 circle_vertex_count = 0;

        for (uint32 circle = 0; circle < 2; circle++) {
            circle_vertex_count = 0;
            for (uint32 i = 0; i <= segments; i++) {
                float32 angle = i * angleIncrement;
                Vector2f trigo(cos(angle), sin(angle));
                maths::Vector2f p = position + trigo * (radius - lineWidth * circle);

                AddVertex(p, color, uvs.size() <= i ? UV_NORMALISED(p) : uvs[i]);
                circle_vertex_count++;
            }
        }

        for (uint32 index = 0; index < circle_vertex_count; ++index) {
            uint32 next = (index + 1) % circle_vertex_count;

            AddIndex(index);
            AddIndex(next);
            AddIndex(circle_vertex_count + index);

            AddIndex(circle_vertex_count + index);
            AddIndex(next);
            AddIndex(circle_vertex_count + next);
        }

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddFilledTriangle(const maths::Vector2f& p1, const maths::Vector2f& p2, const maths::Vector2f& p3, const Color& color, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs) {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        AddVertex(p1, color, uvs.size() > 0 ? uvs[0] : Vector2f(0.5f, 1.0f));
        AddVertex(p2, color, uvs.size() > 1 ? uvs[1] : Vector2f(0.0f, 0.0f));
        AddVertex(p3, color, uvs.size() > 2 ? uvs[2] : Vector2f(1.0f, 0.0f));

        AddIndex(0);
        AddIndex(1);
        AddIndex(2);

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddOutlineTriangle(const maths::Vector2f& p1, const maths::Vector2f& p2, const maths::Vector2f& p3, const Color& color, float32 lineWidth, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs) {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        AddVertex(p1, color, uvs.size() > 0 ? uvs[0] : Vector2f(0.5f, 1.0f));
        AddVertex(p2, color, uvs.size() > 1 ? uvs[1] : Vector2f(0.0f, 0.0f));
        AddVertex(p3, color, uvs.size() > 2 ? uvs[2] : Vector2f(1.0f, 0.0f));

        Vector2f p1_p = (((p2 + p3) / 2.0f) - p1).Normalized() * lineWidth + p1;
        Vector2f p2_p = (((p1 + p3) / 2.0f) - p2).Normalized() * lineWidth + p2;
        Vector2f p3_p = (((p2 + p1) / 2.0f) - p3).Normalized() * lineWidth + p3;

        AddVertex(p1_p, color, uvs.size() > 3 ? uvs[3] : Vector2f(0.5f, 1.0f));
        AddVertex(p2_p, color, uvs.size() > 4 ? uvs[4] : Vector2f(0.0f, 0.0f));
        AddVertex(p3_p, color, uvs.size() > 5 ? uvs[5] : Vector2f(1.0f, 0.0f));

        int32 triangle_vertex_count = 3;
        for (uint32 index = 0; index < triangle_vertex_count; ++index) {
            uint32 next = (index + 1) % triangle_vertex_count;

            AddIndex(index);
            AddIndex(next);
            AddIndex(triangle_vertex_count + index);

            AddIndex(triangle_vertex_count + index);
            AddIndex(next);
            AddIndex(triangle_vertex_count + next);
        }

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddLine(const maths::Vector2f& p1, const maths::Vector2f& p2, const Color& color, float32 lineWidth, Memory::Shared<Texture2D> texture, maths::Vector2f uv0, maths::Vector2f uv1) {
        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        Vector2f normal = (p1 - p2).Normal().Normalized() * lineWidth * 0.5f;

        Vector2f p1_1 = p1 + normal;
        Vector2f p1_2 = p1 - normal;

        Vector2f p2_1 = p2 + normal;
        Vector2f p2_2 = p2 - normal;

        AddVertex(p1_1, color, uv0);
        AddVertex(p1_2, color, uv0);
        AddVertex(p2_2, color, uv1);
        AddVertex(p2_1, color, uv1);

        AddIndex(0);
        AddIndex(1);
        AddIndex(2);

        AddIndex(2);
        AddIndex(3);
        AddIndex(0);

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddFillPath(const std::vector<maths::Vector2f>& points, const Color& color, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        if (points.size() == 0) return nullptr;
        if (points.size() == 1) {
            if (uvs.size() >= 1)
                return AddPoint(points[0], color, 1.0f, texture, uvs[0]);
            return AddPoint(points[0], color, 1.0f, texture);
        }
        if (points.size() == 2) {
            if (uvs.size() >= 2)
                return AddLine(points[0], points[1], color, 1.0f, texture, uvs[0], uvs[1]);
            return AddLine(points[0], points[1], color, 1.0f, texture);
        }

        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        float32 inv = 1.0f / points.size();

        Vector2f prevNormal;
        int32 last_index = 0;

        for (int32 p = 0; p < points.size(); p++) {
            AddVertex(points[p], color, uvs.size() > p ? uvs[p] : Vector2f(1).Normalized() * p * inv);
        }

        for (int32 p = 0; p < points.size() - 2; p++) {
            AddIndex(0);
            AddIndex(p + 1);
            AddIndex(p + 2);
        }

        if (points.size() > 3) {
            AddIndex(0);
            AddIndex(points.size() - 1);
            AddIndex(points.size() - 2);
        }

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddOutlinePath(const std::vector<maths::Vector2f>& points, const Color& color, bool closed, float32 lineWidth, Memory::Shared<Texture2D> texture, const std::vector<maths::Vector2f>& uvs)
    {
        if (points.size() == 0) return nullptr;
        if (points.size() == 1) {
            if (uvs.size() >= 1)
                return AddPoint(points[0], color, lineWidth, texture, uvs[0]);
            return AddPoint(points[0], color, lineWidth, texture);
        }
        if (points.size() == 2) {
            if (uvs.size() >= 2)
                return AddLine(points[0], points[1], color, lineWidth, texture, uvs[0], uvs[1]);
            return AddLine(points[0], points[1], color, lineWidth, texture);
        }

        BeginCommand(RenderPrimitive::Enum::Triangles);
        SetCommandTextureId(texture);

        float32 inv = 1.0f / points.size();

        Vector2f prevNormal;
        int32 last_index = 0;

        for (int32 p = 0; p < points.size(); p++) {
            Vector2f p1 = points[p];
            Vector2f p2 = points[(p + 1) % points.size()];

            Vector2f normal = (p1 - p2).Normal().Normalized();
            Vector2f apply;

            if (prevNormal == Vector2f()) apply = normal;
            else if (p == points.size() - 1 && !closed) apply = prevNormal;
            else apply = ((normal + prevNormal) / 2.0f).Normalized();

            apply = apply * lineWidth * 0.5f;
            prevNormal = normal;

            Vector2f p1_1 = p1 + apply;
            Vector2f p1_2 = p1 - apply;

            AddVertex(p1_1, color, uvs.size() > p ? uvs[p] : Vector2f(1).Normalized() * p * inv);
            last_index++;
            AddVertex(p1_2, color, uvs.size() > p ? uvs[p] : Vector2f(1).Normalized() * p * inv);
            last_index++;
        }

        for (int32 p = 0; p < points.size(); p++) {
            AddIndex(p);
            AddIndex(p + 1);
            AddIndex(p + 2);

            AddIndex(p + 1);
            AddIndex(p + 2);
            AddIndex(p + 3);
        }

        if (closed) {
            AddIndex(0);
            AddIndex(1);
            AddIndex(last_index - 1);

            AddIndex(last_index - 1);
            AddIndex(last_index - 2);
            AddIndex(0);
        }

        EndCommand(true);
        return GetLastCommand();
    }

    RenderCommand2D* RenderCache2D::AddPoint(const maths::Vector2f& p, const Color& color, float32 lineWidth, Memory::Shared<Texture2D> texture, maths::Vector2f uv) {
        BeginCommand(RenderPrimitive::Enum::Points);
        SetCommandTextureId(texture);
        AddVertex(p, color, uv);
        AddIndex(0);
        EndCommand(true);
        return GetLastCommand();
    }

    int32 RenderCache2D::AddInternalArc(const Vector2f& center, const Vector2f& radius, const Vector2f& rad, const Color& color, float32 departAngle, float32 endAngle, float32 segments, int32 decal, const std::vector<maths::Vector2f>& uvs)
    {
        int32 pas = 0;
        int32 depart = 0;
        int32 fin = 0;
        int32 vertex_count = 0;
        int32 indice_count = segments + 1;
        float32 inverseg = 1.0f / static_cast<float32>(segments);

        if (departAngle > endAngle) {
            pas = -1;
            depart = segments;
            fin = -1;
        }
        else if (departAngle < endAngle) {
            pas = 1;
            depart = 0;
            fin = segments + 1;
        }
        else {
            return 0;
        }

        int32 i = depart;

        while (i != fin) {
            float32 theta = (static_cast<float32>(i) * inverseg * maths::Abs(fin - depart)) + depart;

            Vector2f trigo(maths::Cos(Angle::FromRadian(theta)), maths::Sin(Angle::FromRadian(theta)));

            Vector2f radius_calculus(radius.x, radius.y);

            if (radius.x == 0) {
                trigo.x = (trigo.x < 0) ? -1 : 1;
                radius_calculus.x = rad.x;
            }

            if (radius.y == 0) {
                trigo.y = (trigo.y < 0) ? -1 : 1;
                radius_calculus.y = rad.y;
            }

            Vector2f vertex = center + trigo * radius_calculus;

            Vector2f uv = uvs.size() > i ? uvs[decal + i] : trigo;

            AddVertex(vertex, color, uv);
            vertex_count++;

            i += pas;
        }

        return vertex_count;
    }

    int32 RenderCache2D::AddInternalArc(uint32 depart, const maths::Vector2f& center, const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector2f& radius, const maths::Vector2f& rad, const Color& color, float32 startAngle, float32 endAngle, uint32 segments, const std::vector<maths::Vector2f>& uvs)
    {
        int32 vertexLeng = 0;
        float32 angleIncrement = maths::Abs(endAngle - startAngle) / segments;

        for (int32 vertice = segments; vertice >= 0; --vertice) {
            float32 angle = vertice * angleIncrement - endAngle;
            Vector2f trigo = Vector2f(cos(angle), sin(angle));

            Vector2f radius_calculus(radius.x, radius.y);

            if (radius.x == 0) {
                trigo.x = (trigo.x < 0) ? -1 : 1;
                radius_calculus.x = rad.x;
            }

            if (radius.y == 0) {
                trigo.y = (trigo.y < 0) ? -1 : 1;
                radius_calculus.y = rad.y;
            }

            Vertex2D vertex;
            vertex.position = center + radius_calculus * trigo;
            vertex.texCord = uvs.size() > depart + vertice ? uvs[depart + vertice] : UV_NORMALISED(vertex.position);
            vertex.color = color;

            AddVertex(vertex);
            vertexLeng++;
        }

        return vertexLeng + depart;
    }

}  //  nkentseu
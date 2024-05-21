//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 08:17:20 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalRenderer.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>

#include "Nkentseu/Graphics/Context.h"
#include "Nkentseu/Graphics/Shader.h"
#include "Nkentseu/Graphics/VertexArray.h"
#include "Nkentseu/Graphics/VertexBuffer.h"
#include "Nkentseu/Graphics/IndexBuffer.h"
#include "InternalContext.h"
#include "InternalShader.h"
#include "InternalIndexBuffer.h"
#include "InternalVertexArray.h"
#include "InternalVertexBuffer.h"

namespace nkentseu {
    
    // Constructor
    InternalRenderer::InternalRenderer() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalRenderer::~InternalRenderer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalRenderer::Initialize(Context* context) {
        return false;
    }

    bool InternalRenderer::Deinitialize() {
        return false;
    }

    bool InternalRenderer::Clear(const Color& color) {
        return false;
    }

    bool InternalRenderer::Clear(uint8 r, uint8 g, uint8 b, uint8 a) {
        return false;
    }

    bool InternalRenderer::SetActiveShader(Memory::Shared<Shader> shader) {
        return false;
    }

    bool InternalRenderer::UnsetActiveShader() {
        return false;
    }

    bool InternalRenderer::DrawMode(DrawMode::Code mode, DrawContentMode::Code contentMode) {
        return false;
    }

    bool InternalRenderer::Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex) {
        return false;
    }

    bool InternalRenderer::Present() {
        return false;
    }

    bool InternalRenderer::Swapbuffer() {
        return false;
    }

    bool InternalRenderer::Resize(const Vector2u& size) {
        return false;
    }

}  //  nkentseu

#endif
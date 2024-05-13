//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 10:02:13 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "GraphicsProperties.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    std::string GraphicsInfos::ToString() const
    {
        return FORMATTER.Format("*temporary*\nRenderer: {0}\nVendor: {1}\nVersion: {2}\nSharing Langage Version: {3}\nExtension: {4}\nProfile Mask: {5}\n\n", renderer, vendor, version, langageVersion, extension, profilMask);
    }

    ContextProperties& ContextProperties::operator=(const ContextProperties& other) {
        this->version = other.version;
        this->offScreenSize = other.offScreenSize;
        this->pixelFormat = other.pixelFormat;
        return *this;
    }

    ContextProperties::ContextProperties() {
        version = InitVersion();
        offScreenSize = Vector2u();
        pixelFormat = GraphicsPixelFormat();
        // Log_nts.Debug("version = {0}", version);
    }

    ContextProperties::ContextProperties(Vector2u size, GraphicsPixelFormat format)
        : offScreenSize(size), pixelFormat(format) {
        version = InitVersion();
        // Log_nts.Debug("version = {0}", version);
    }

    ContextProperties::ContextProperties(GraphicsPixelFormat format)
        : offScreenSize(), pixelFormat(format) {
        version = InitVersion();
        // Log_nts.Debug("version = {0}", version);
    }

    ContextProperties::ContextProperties(const ContextProperties& properties) : version(properties.version), offScreenSize(properties.offScreenSize), pixelFormat(properties.pixelFormat) {
    }

    Vector2i ContextProperties::InitVersion()
    {
        Vector2i version;

#if NKENTSEU_GRAPHICS_API_OPENGL
        version.major = 4;
        version.minor = 6;
#elif NKENTSEU_GRAPHICS_API_VULKAN
        version.major = 1;
        version.minor = 3;
#elif NKENTSEU_GRAPHICS_API_DIRECTX11
        version.major = 11;
        version.minor = 0;
#elif NKENTSEU_GRAPHICS_API_DIRECTX12
        version.major = 12;
        version.minor = 0;
#elif NKENTSEU_GRAPHICS_API_SOFTWARE
        version.major = 1;
        version.minor = 0;
#elif NKENTSUU_GRAPHICS_API_METAL
        version.major = 1;
        version.minor = 0;
#endif
        return version;
    }
}    // namespace nkentseu
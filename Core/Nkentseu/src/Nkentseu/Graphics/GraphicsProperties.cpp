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

    ContextProperties::ContextProperties(const GraphicsApiType::ApiType api)
    {
        graphicsApi = api;
        version = InitVersion(graphicsApi);
    }

    ContextProperties::ContextProperties(const GraphicsApiType::ApiType api, const Vector2i& version)
    {
        this->graphicsApi = api;
        this->version = version;
    }

    ContextProperties& ContextProperties::operator=(const ContextProperties& other) {
        this->version = other.version;
        this->offScreenSize = other.offScreenSize;
        this->pixelFormat = other.pixelFormat;
        this->graphicsApi = other.graphicsApi;
        return *this;
    }

    ContextProperties::ContextProperties() {
        graphicsApi = GraphicsApiType::VulkanApi;
        version = InitVersion(graphicsApi);
        offScreenSize = Vector2u();
        pixelFormat = GraphicsPixelFormat();
        // Log_nts.Debug("version = {0}", version);
    }

    ContextProperties::ContextProperties(Vector2u size, GraphicsPixelFormat format)
        : offScreenSize(size), pixelFormat(format) {
        version = InitVersion(graphicsApi);
        // Log_nts.Debug("version = {0}", version);
    }

    ContextProperties::ContextProperties(GraphicsPixelFormat format)
        : offScreenSize(), pixelFormat(format) {
        version = InitVersion(graphicsApi);
        // Log_nts.Debug("version = {0}", version);
    }

    ContextProperties::ContextProperties(const ContextProperties& properties) : version(properties.version), offScreenSize(properties.offScreenSize), pixelFormat(properties.pixelFormat), graphicsApi(properties.graphicsApi) {
    }

    Vector2i ContextProperties::InitVersion(const GraphicsApiType::ApiType api)
    {
        Vector2i version;

        if (api == GraphicsApiType::OpenglApi) {
            version.major = 4;
            #ifdef NKENTSEU_PLATFORM_LINUX_USE_SUBSYSTEM
            version.minor = 2; // 2 sur les platforme  wsl
            #else
            version.minor = 6; // 6 sur les platforme autre que wsl
            #endif
        } else if (api == GraphicsApiType::VulkanApi) {
            version.major = 1;
            version.minor = 3;
        } else if (api == GraphicsApiType::DirectX11Api) {
            version.major = 11;
            version.minor = 0;
        } else if (api == GraphicsApiType::DirectX12Api) {
            version.major = 12;
            version.minor = 0;
        } else if (api == GraphicsApiType::SoftwareApi) {
            version.major = 1;
            version.minor = 0;
        } else if (api == GraphicsApiType::MetalApi) {
            version.major = 1;
            version.minor = 0;
        }
        return version;
    }
}    // namespace nkentseu
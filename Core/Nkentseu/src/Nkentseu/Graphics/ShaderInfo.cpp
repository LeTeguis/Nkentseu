//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 06:01:02 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "ShaderInfo.h"
#include <Logger/Formatter.h>

#include <fstream>
#include <sstream>
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {

    std::string ShaderType::ToString(ShaderType::Code shaderType) {

        if (shaderType == ShaderType::Vertex) return "Vertex";
        if (shaderType == ShaderType::Compute) return "Compute";
        if (shaderType == ShaderType::Fragment) return "Fragment";
        if (shaderType == ShaderType::Geometry) return "Geometry";
        if (shaderType == ShaderType::Pixel) return "Pixel";
        if (shaderType == ShaderType::TesControl) return "TesControl";
        if (shaderType == ShaderType::TesEvaluation) return "TesEvaluation";

        return "NotDefine";
    }

    ShaderType::Code ShaderType::FromString(const std::string& shaderTypeStr) {
        if (shaderTypeStr == "Vertex") return ShaderType::Vertex;
        if (shaderTypeStr == "Compute") return ShaderType::Compute;
        if (shaderTypeStr == "Fragment") return ShaderType::Fragment;
        if (shaderTypeStr == "Geometry") return ShaderType::Geometry;
        if (shaderTypeStr == "Pixel") return ShaderType::Pixel;
        if (shaderTypeStr == "TesControl") return ShaderType::TesControl;
        if (shaderTypeStr == "TesEvaluation") return ShaderType::TesEvaluation;

        return ShaderType::NotDefine;
    }

    uint32 ShaderDataType::ComponentCount(ShaderDataType::Code shaderDataType)
    {
        if (shaderDataType == ShaderDataType::Boolean) return 1;
        if (shaderDataType == ShaderDataType::Float) return 1;
        if (shaderDataType == ShaderDataType::Float2) return 2;
        if (shaderDataType == ShaderDataType::Float3) return 3;
        if (shaderDataType == ShaderDataType::Float4) return 4;
        if (shaderDataType == ShaderDataType::Int) return 1;
        if (shaderDataType == ShaderDataType::Int2) return 2;
        if (shaderDataType == ShaderDataType::Int3) return 3;
        if (shaderDataType == ShaderDataType::Int4) return 4;
        if (shaderDataType == ShaderDataType::Byte4) return 4;
        if (shaderDataType == ShaderDataType::Mat3) return 9;
        if (shaderDataType == ShaderDataType::Mat4) return 16;
        if (shaderDataType == ShaderDataType::Struct) return -1;

        return 0;
    }

    uint32 ShaderDataType::ComponentElementSize(ShaderDataType::Code shaderDataType)
    {
        if (shaderDataType == ShaderDataType::Boolean) return sizeof(bool);
        if (shaderDataType == ShaderDataType::Float) return sizeof(float32);
        if (shaderDataType == ShaderDataType::Float2) return sizeof(float32);
        if (shaderDataType == ShaderDataType::Float3) return sizeof(float32);
        if (shaderDataType == ShaderDataType::Float4) return sizeof(float32);
        if (shaderDataType == ShaderDataType::Int) return sizeof(int32);
        if (shaderDataType == ShaderDataType::Int2) return sizeof(int32);
        if (shaderDataType == ShaderDataType::Int3) return sizeof(int32);
        if (shaderDataType == ShaderDataType::Int4) return sizeof(int32);
        if (shaderDataType == ShaderDataType::Byte4) return 1;
        if (shaderDataType == ShaderDataType::Mat3) return sizeof(float32);
        if (shaderDataType == ShaderDataType::Mat4) return sizeof(float32);
        if (shaderDataType == ShaderDataType::Struct) return 0;

        return 0;
    }

    uint32 ShaderDataType::ComponentSize(ShaderDataType::Code shaderDataType)
    {
        return ComponentElementSize(shaderDataType) * ComponentCount(shaderDataType);
    }

    std::string ShaderDataType::ToString(ShaderDataType::Code shaderDataType) {

        if (shaderDataType == ShaderDataType::Boolean) return "Boolean";
        if (shaderDataType == ShaderDataType::Float) return "Float";
        if (shaderDataType == ShaderDataType::Float2) return "Float2";
        if (shaderDataType == ShaderDataType::Float3) return "Float3";
        if (shaderDataType == ShaderDataType::Float4) return "Float4";
        if (shaderDataType == ShaderDataType::Int) return "Int";
        if (shaderDataType == ShaderDataType::Int2) return "Int2";
        if (shaderDataType == ShaderDataType::Int3) return "Int3";
        if (shaderDataType == ShaderDataType::Int4) return "Int4";
        if (shaderDataType == ShaderDataType::Byte4) return "Byte4";
        if (shaderDataType == ShaderDataType::Mat3) return "Mat3";
        if (shaderDataType == ShaderDataType::Mat4) return "Mat4";
        if (shaderDataType == ShaderDataType::Struct) return "Struct";

        return "NotDefine";
    }

    ShaderDataType::Code ShaderDataType::FromString(const std::string& shaderDataTypeStr) {
        if (shaderDataTypeStr == "Boolean") return ShaderDataType::Boolean;
        if (shaderDataTypeStr == "Float") return ShaderDataType::Float;
        if (shaderDataTypeStr == "Float2") return ShaderDataType::Float2;
        if (shaderDataTypeStr == "Float3") return ShaderDataType::Float3;
        if (shaderDataTypeStr == "Float4") return ShaderDataType::Float4;
        if (shaderDataTypeStr == "Int") return ShaderDataType::Int;
        if (shaderDataTypeStr == "Int2") return ShaderDataType::Int2;
        if (shaderDataTypeStr == "Int3") return ShaderDataType::Int3;
        if (shaderDataTypeStr == "Int4") return ShaderDataType::Int4;
        if (shaderDataTypeStr == "Byte4") return ShaderDataType::Byte4;
        if (shaderDataTypeStr == "Mat3") return ShaderDataType::Mat3;
        if (shaderDataTypeStr == "Mat4") return ShaderDataType::Mat4;
        if (shaderDataTypeStr == "Struct") return ShaderDataType::Struct;

        return ShaderDataType::NotDefine;
    }

    // Convertit un code de type de buffer en chaîne de caractères
    std::string BufferType::ToString(BufferType::Code bufferType) {
        switch (bufferType) {
        case NotDefine: return "NotDefine";
        case Vertex: return "Vertex";
        case Index: return "Index";
        case Uniform: return "Uniform";
        case Storage: return "Storage";
        case Texture: return "Texture";
        case Constant: return "Constant";
        default: return "Unknown";
        }
    }

    // Convertit une chaîne de caractères en code de type de buffer
    BufferType::Code BufferType::FromString(const std::string& bufferTypeStr) {
        if (bufferTypeStr == "NotDefine") return NotDefine;
        if (bufferTypeStr == "Vertex") return Vertex;
        if (bufferTypeStr == "Index") return Index;
        if (bufferTypeStr == "Uniform") return Uniform;
        if (bufferTypeStr == "Storage") return Storage;
        if (bufferTypeStr == "Texture") return Texture;
        if (bufferTypeStr == "Constant") return Constant;
        return NotDefine; // Valeur par défaut
    }
    
    std::string ShaderLoader::LoadShaderToMemoryStr(const std::string& shaderFile)
    {
        std::ifstream file;
        std::stringstream bufferedLines;
        std::string line;

        file.open(shaderFile);

        while (std::getline(file, line)) {
            bufferedLines << line << "\n";
        }

        std::string shaderSource = bufferedLines.str();
        bufferedLines.str("");

        file.close();

        return shaderSource;
    }

    ShaderData ShaderLoader::LoadShaderToMemoryData(const std::string& shaderFile)
    {
        return ShaderData();
    }

    std::vector<ShaderData> ShaderLoader::LoadShaderToMemoryDatas(const std::string& shaderFile)
    {
        return std::vector<ShaderData>();
    }
    
    const char* ShaderLoader::LoadShaderToMemoryChar(const std::string& shaderFile)
    {
        std::ifstream file;
        std::stringstream bufferedLines;
        std::string line;

        file.open(shaderFile);

        while (std::getline(file, line)) {
            bufferedLines << line << "\n";
        }

        static std::string shaderSource;
        shaderSource = bufferedLines.str();
        bufferedLines.str("");

        file.close();

        return shaderSource.c_str();
    }

    BufferAttribute::BufferAttribute(ShaderDataType::Code type, const std::string& name, uint32 location, bool normalized)
        : name(name), type(type), size(ShaderDataType::ComponentSize(type)), offset(0), normalized(normalized), location(location) {
    }

    uint32 BufferAttribute::GetComponentCount() const {
        return ShaderDataType::ComponentCount(type);  // Get number of components from data type
    }

    uint32 BufferAttribute::GetComponentSize() const
    {
        return ShaderDataType::ComponentSize(type);
    }

    BufferLayout::BufferLayout(const std::initializer_list<BufferAttribute>& attributes)
        : attributes(attributes) {
        CalculateOffsetsAndStride();
    }

    void BufferLayout::CalculateOffsetsAndStride() {
        usize offset = 0;
        stride = 0;
        componentCount = 0;
        for (auto& attribute : attributes) {
            attribute.offset = offset;  // Set offset for each attribute
            offset += attribute.size;    // Update offset for the next attribute
            stride += attribute.size;    // Accumulate size for total stride
            componentCount += attribute.GetComponentCount();
        }
    }

    const std::vector<BufferAttribute>& BufferLayout::GetAttributes() const {
        return attributes;
    }

    uint32 BufferLayout::GetStride() const {
        return stride;
    }

    std::vector<BufferAttribute>::iterator BufferLayout::begin() { return attributes.begin(); }
    std::vector<BufferAttribute>::iterator BufferLayout::end() { return attributes.end(); }
    std::vector<BufferAttribute>::const_iterator BufferLayout::begin() const { return attributes.begin(); }
    std::vector<BufferAttribute>::const_iterator BufferLayout::end() const { return attributes.end(); }

    VertexInputElement::VertexInputElement(const std::string& name, uint32 binding, uint32 location, ShaderDataType::Code type, uint32 size, uint32 offset, bool normalized)
        : binding(binding), location(location), type(type), size(size), offset(offset), normalized(normalized), name(name) {}
    
    std::string BufferDataUsage::ToString(BufferDataUsage::Code bufferUsage)
    {
        switch (bufferUsage) {
        case StaticDraw: return "StaticDraw";
        case DynamicDraw: return "DynamicDraw";
        case StreamDraw: return "StreamDraw";
        default: return "NotDefine";
        }
    }

    BufferDataUsage::Code BufferDataUsage::FromString(const std::string& bufferUsageStr)
    {
        if (bufferUsageStr == "StaticDraw") return StaticDraw;
        if (bufferUsageStr == "DynamicDraw") return DynamicDraw;
        if (bufferUsageStr == "StreamDraw") return StreamDraw;
        return NotDefine; // Valeur par défaut
    }

    std::string DrawVertexType::ToString(DrawVertexType::Code shaderDataDrawType)
    {
        switch (shaderDataDrawType) {
        case Triangles: return "Triangles";
        default: return "NotDefine";
        }
    }

    DrawVertexType::Code DrawVertexType::FromString(const std::string& shaderDataDrawTypeStr)
    {
        if (shaderDataDrawTypeStr == "Triangles") return Triangles;
        return NotDefine; // Valeur par défaut
    }

    std::string CullModeType::ToString(CullModeType::Code drawMode)
    {
        switch (drawMode) {
        case Front: return "Front";
        case Back: return "Back";
        case FrontBack: return "FrontBack";
        case NoCull: return "NoCull";
        default: return "NotDefine";
        }
    }

    CullModeType::Code CullModeType::FromString(const std::string& drawModeStr)
    {
        if (drawModeStr == "Front") return Front;
        if (drawModeStr == "Back") return Back;
        if (drawModeStr == "FrontBack") return FrontBack;
        if (drawModeStr == "NoCull") return NoCull;
        return NotDefine; // Valeur par défaut
    }

    std::string PolygonModeType::ToString(PolygonModeType::Code drawContentMode)
    {
        switch (drawContentMode) {
        case Line: return "Line";
        case Fill: return "Fill";
        case Point: return "Point";
        case FillRectangle: return "FillRectangle";
        default: return "NotDefine";
        }
    }

    PolygonModeType::Code PolygonModeType::FromString(const std::string& drawContentModeStr)
    {
        if (drawContentModeStr == "Line") return Line;
        if (drawContentModeStr == "Fill") return Fill;
        if (drawContentModeStr == "FillRectangle") return FillRectangle;
        if (drawContentModeStr == "Point") return Point;
        return NotDefine; // Valeur par défaut
    }

    std::string DrawIndexType::ToString(DrawIndexType::Code indexType)
    {
        switch (indexType) {
        case UInt8: return "UInt8";
        case UInt16: return "UInt16";
        case UInt32: return "UInt32";
        case UInt64: return "UInt64";
        default: return "NotDefine";
        }
    }

    DrawIndexType::Code DrawIndexType::FromString(const std::string& indexTypeStr)
    {
        if (indexTypeStr == "UInt8") return UInt8;
        if (indexTypeStr == "UInt16") return UInt16;
        if (indexTypeStr == "UInt32") return UInt32;
        if (indexTypeStr == "UInt64") return UInt64;
        return NotDefine; // Valeur par défaut
    }

    usize DrawIndexType::SizeOf(DrawIndexType::Code indexType)
    {
        switch (indexType) {
        case UInt8: return sizeof(uint8);
        case UInt16: return sizeof(uint16);
        case UInt32: return sizeof(uint32);
        case UInt64: return sizeof(uint64);
        default: return sizeof(uint32);
        }
    }

    std::string FrontFaceType::ToString(FrontFaceType::Code mode)
    {
        switch (mode) {
        case CounterClockwise: return "CounterClockwise";
        case Clockwise: return "Clockwise";
        default: return "NotDefine";
        }
    }

    FrontFaceType::Code FrontFaceType::FromString(const std::string& mode)
    {
        if (mode == "Clockwise") return Clockwise;
        if (mode == "CounterClockwise") return CounterClockwise;
        return NotDefine; // Valeur par défaut
    }

    std::string PrimitiveTopologyType::ToString(CullModeType::Code mode)
    {
        switch (mode) {
        case PointList: return "PointList";
        case LineList: return "LineList";
        case LineStrip: return "LineStrip";
        case TriangleList: return "TriangleList";
        case TriangleStrip: return "TriangleStrip";
        case TriangleFan: return "TriangleFan";
        case LineListWithAdjacency: return "LineListWithAdjacency";
        case LineStripWithAdjacency: return "LineStripWithAdjacency";
        case TriangleListWithAdjacency: return "TriangleListWithAdjacency";
        case TriangleStripWithAdjacency: return "TriangleStripWithAdjacency";
        case PathList: return "PathList";
        default: return "NotDefine";
        }
    }

    CullModeType::Code PrimitiveTopologyType::FromString(const std::string& mode)
    {
        if (mode == "PointList") return PointList;
        if (mode == "LineList") return LineList;
        if (mode == "LineStrip") return LineStrip;
        if (mode == "TriangleList") return TriangleList;
        if (mode == "TriangleStrip") return TriangleStrip;
        if (mode == "TriangleFan") return TriangleFan;
        if (mode == "LineListWithAdjacency") return LineListWithAdjacency;
        if (mode == "LineStripWithAdjacency") return LineStripWithAdjacency;
        if (mode == "TriangleListWithAdjacency") return TriangleListWithAdjacency;
        if (mode == "TriangleStripWithAdjacency") return TriangleStripWithAdjacency;
        if (mode == "PathList") return PathList;
        return NotDefine; // Valeur par défaut
    }

    UniformBufferAttribut::UniformBufferAttribut(usize size, usize instance, uint32 binding, const std::string& name, ShaderType::Code type, UniformBufferType::Code utype)
        : size(size), instance(instance), binding(binding), name(name), type(type), uType(utype) {}

    UniformBufferAttribut::UniformBufferAttribut(usize size, usize instance, uint32 binding, const std::string& name, ShaderType::Code type, UniformBufferType::Code utype, BufferDataUsage::Code usage)
        : size(size), instance(instance), binding(binding), name(name), type(type), usage(usage), uType(utype) {}

    const UniformBufferAttribut& UniformBufferLayout::GetAttribut(uint32 index) {
        static const UniformBufferAttribut uba;

        if (index <= attributes.size()) return uba;
        return attributes[index];
    }

    const UniformBufferAttribut& UniformBufferLayout::GetAttribut(const std::string& name) {
        static const UniformBufferAttribut uba;

        for (auto& attribut : attributes) {
            if (attribut.name == name) {
                return attribut;
            }
        }

        return uba;
    }
}  //  nkentseu
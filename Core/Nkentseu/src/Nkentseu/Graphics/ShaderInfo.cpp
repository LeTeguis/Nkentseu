//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 06:01:02 PM.
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

    int32 ShaderDataType::ComponentCount(ShaderDataType::Code shaderDataType)
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
        if (shaderDataType == ShaderDataType::Mat3) return 6;
        if (shaderDataType == ShaderDataType::Mat4) return 16;
        if (shaderDataType == ShaderDataType::Struct) return -1;

        return 0;
    }

    int32 ShaderDataType::ComponentSize(ShaderDataType::Code shaderDataType)
    {
        if (shaderDataType == ShaderDataType::Boolean) return 1;
        if (shaderDataType == ShaderDataType::Float) return 4 * 1;
        if (shaderDataType == ShaderDataType::Float2) return 4 * 2;
        if (shaderDataType == ShaderDataType::Float3) return 4 * 3;
        if (shaderDataType == ShaderDataType::Float4) return 4 * 4;
        if (shaderDataType == ShaderDataType::Int) return 4 * 1;
        if (shaderDataType == ShaderDataType::Int2) return 4 * 2;
        if (shaderDataType == ShaderDataType::Int3) return 4 * 3;
        if (shaderDataType == ShaderDataType::Int4) return 4 * 4;
        if (shaderDataType == ShaderDataType::Byte4) return -1; // a definir
        if (shaderDataType == ShaderDataType::Mat3) return 4 * 3 * 3;
        if (shaderDataType == ShaderDataType::Mat4) return 4 * 4 * 4;
        if (shaderDataType == ShaderDataType::Struct) return -2;

        return 0;
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

    // Convertit un code de type de buffer en cha�ne de caract�res
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

    // Convertit une cha�ne de caract�res en code de type de buffer
    BufferType::Code BufferType::FromString(const std::string& bufferTypeStr) {
        if (bufferTypeStr == "NotDefine") return NotDefine;
        if (bufferTypeStr == "Vertex") return Vertex;
        if (bufferTypeStr == "Index") return Index;
        if (bufferTypeStr == "Uniform") return Uniform;
        if (bufferTypeStr == "Storage") return Storage;
        if (bufferTypeStr == "Texture") return Texture;
        if (bufferTypeStr == "Constant") return Constant;
        return NotDefine; // Valeur par d�faut
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

    BufferAttribute::BufferAttribute(BufferType::Code bufferType, ShaderDataType::Code type, const std::string& name, bool normalized)
        : name(name), type(type), bufferType(bufferType), size(ShaderDataType::ComponentSize(type)), offset(0), normalized(normalized) {}

    uint32 BufferAttribute::GetComponentCount() const {
        return ShaderDataType::ComponentCount(type);  // Get number of components from data type
    }

    BufferLayout::BufferLayout(const std::initializer_list<BufferAttribute>& attributes)
        : attributes(attributes) {
        CalculateOffsetsAndStride();
    }

    void BufferLayout::CalculateOffsetsAndStride() {
        usize offset = 0;
        stride = 0;
        for (auto& attribute : attributes) {
            attribute.offset = offset;  // Set offset for each attribute
            offset += attribute.size;    // Update offset for the next attribute
            stride += attribute.size;    // Accumulate size for total stride
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
    
}  //  nkentseu
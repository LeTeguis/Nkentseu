//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 06:01:02 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __SHADER_INFO_H__
#define __SHADER_INFO_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>

#include <string>
#include <unordered_map>

namespace nkentseu {
	struct NKENTSEU_API ShaderType {
		using Code = uint64;
		enum : Code {
			NotDefine, Vertex, Fragment, Pixel, Geometry, TesControl, TesEvaluation, Compute
		};

		static std::string ToString(ShaderType::Code shaderType);
		static ShaderType::Code FromString(const std::string& shaderTypeStr);
	};

	struct NKENTSEU_API ShaderDataType {
		using Code = uint64;
		enum : Code {
			NotDefine, Boolean, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Byte4, Mat3, Mat4, Struct
		};

		static uint32 ComponentCount(ShaderDataType::Code shaderDataType);
		static uint32 ComponentElementSize(ShaderDataType::Code shaderDataType);
		static uint32 ComponentSize(ShaderDataType::Code shaderDataType);
		static std::string ToString(ShaderDataType::Code shaderDataType);
		static ShaderType::Code FromString(const std::string& shaderDataTypeStr);
	};

	struct NKENTSEU_API BufferDataUsage {
		using Code = uint64;

		enum : Code {
			NotDefine,
			StaticDraw,
			DynamicDraw,
			StreamDraw
		};

		static std::string ToString(BufferDataUsage::Code bufferUsage);
		static BufferDataUsage::Code FromString(const std::string& bufferUsageStr);
	};

	struct NKENTSEU_API DrawVertexType {
		using Code = uint64;

		enum : Code {
			NotDefine,
			Triangles
		};

		static std::string ToString(DrawVertexType::Code shaderDataDrawType);
		static DrawVertexType::Code FromString(const std::string& shaderDataDrawTypeStr);
	};

	// Repr�sente diff�rents types de buffers
	struct NKENTSEU_API BufferType {
		using Code = uint64;
		// Enum�ration des types de buffers
		enum : Code {
			NotDefine, Vertex, Index, Uniform, Storage, Texture, Constant
		};

		// Convertit un code de type de buffer en cha�ne de caract�res
		static std::string ToString(BufferType::Code bufferType);

		// Convertit une cha�ne de caract�res en code de type de buffer
		static BufferType::Code FromString(const std::string& bufferTypeStr);
	};

	// Repr�sente diff�rents types de buffers
	struct NKENTSEU_API DrawIndexType {
		using Code = uint64;
		// Enum�ration des types de buffers
		enum : Code {
			NotDefine, UInt8, UInt16, UInt32, UInt64
		};

		// Convertit un code de type de buffer en cha�ne de caract�res
		static std::string ToString(DrawIndexType::Code indexType);

		// Convertit une cha�ne de caract�res en code de type de buffer
		static DrawIndexType::Code FromString(const std::string& indexTypeStr);

		static usize SizeOf(DrawIndexType::Code indexType);
	};

	struct NKENTSEU_API ShaderData {
		ShaderType::Code type;
		std::string data;
	};

	struct NKENTSEU_API ShaderLoader {
		static std::string LoadShaderToMemoryStr(const std::string& shaderFile);
		static ShaderData LoadShaderToMemoryData(const std::string& shaderFile);
		static std::vector<ShaderData> LoadShaderToMemoryDatas(const std::string& shaderFile);
		static const char* LoadShaderToMemoryChar(const std::string& shaderFile);
	};

	struct NKENTSEU_API BufferAttribute {
		std::string name = "";
		ShaderDataType::Code type = ShaderDataType::NotDefine;
		uint32 size = 0;
		usize offset = 0;
		uint32 location = 0;
		bool normalized = false;

		BufferAttribute() = default;

		BufferAttribute(ShaderDataType::Code type, const std::string& name, uint32 location, bool normalized = false);

		uint32 GetComponentCount() const;
		uint32 GetComponentSize() const;
	};

	struct NKENTSEU_API BufferLayout {
		std::vector<BufferAttribute> attributes;
		uint32 stride = 0;
		uint32 componentCount = 0;

		BufferLayout() = default;
		
		BufferLayout(const std::initializer_list<BufferAttribute>& attributes);

		BufferLayout& operator=(const BufferLayout& bufferLayout) {
			this->stride = bufferLayout.stride;
			this->componentCount = bufferLayout.componentCount;
			this->attributes = bufferLayout.attributes;

			//std::copy(attributes.begin(), attributes.end(), bufferLayout);

			return *this;
		}

		void CalculateOffsetsAndStride();

		const std::vector<BufferAttribute>& GetAttributes() const;

		uint32 GetStride() const;

		std::vector<BufferAttribute>::iterator begin();
		std::vector<BufferAttribute>::iterator end();
		std::vector<BufferAttribute>::const_iterator begin() const;
		std::vector<BufferAttribute>::const_iterator end() const;
	};

	using IndexBufferLayout = BufferLayout;
	using PushConstantLayout = BufferLayout;

	struct NKENTSEU_API UniformBufferType {
		using Code = uint64;
		enum : Code {
			NotDefine, Static, Dynamic
		};
	};

	struct NKENTSEU_API UniformBufferAttribut {
		usize size = 0;
		usize instance = 1;
		uint32 binding = 0;
		std::string name = "";
		ShaderType::Code type = ShaderType::NotDefine;
		BufferDataUsage::Code usage = BufferDataUsage::StaticDraw;
		UniformBufferType::Code uType = UniformBufferType::Static;

		// run time send
		void* data = nullptr; // internal use
		usize dataSize = 0;

		// Constructeur par défaut
		UniformBufferAttribut() = default;

		// Constructeur avec paramètres
		//UniformBufferAttribut(usize size, uint32 binding, const std::string& name, ShaderType::Code type, UniformBufferType::Code utype, usize instance = 1);
		UniformBufferAttribut(usize size, uint32 binding, const std::string& name, ShaderType::Code type, UniformBufferType::Code utype, usize instance = 1, BufferDataUsage::Code usage = BufferDataUsage::StaticDraw);
	};

	struct NKENTSEU_API UniformBufferLayout {
		std::unordered_map<std::string, UniformBufferAttribut> attributes;
		uint32 sizes = 0;

		// Constructeur par défaut
		UniformBufferLayout() = default;

		// Constructeur avec std::initializer_list
		UniformBufferLayout(const std::initializer_list<UniformBufferAttribut>& attributes);

		const UniformBufferAttribut& GetAttribut(uint32 index);
		const UniformBufferAttribut& GetAttribut(const std::string& name);

		void AddAttribut(const UniformBufferAttribut& attribut);
	};

	struct NKENTSEU_API ShaderBufferLayout {
		BufferLayout vertexInput;
		UniformBufferLayout uniformBuffer;
	};

	struct NKENTSEU_API VertexInputElement {
		std::string name;
		uint32 size;
		uint32 binding;
		uint32 location;
		ShaderDataType::Code type;
		uint32 offset;
		bool normalized;

		VertexInputElement() = default;

		VertexInputElement(const std::string& name, uint32 binding, uint32 location, ShaderDataType::Code type, uint32 size, uint32 offset, bool normalized = false);
	};

}  //  nkentseu

#endif  // __SHADER_INFO_H__!
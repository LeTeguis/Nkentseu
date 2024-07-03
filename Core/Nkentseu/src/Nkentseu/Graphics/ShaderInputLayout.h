//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-29 at 10:04:30 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __SHADER_INPUT_LAYOUT_H__
#define __SHADER_INPUT_LAYOUT_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>
#include <System/Nature/Base.h>

#include "Context.h"
#include "Shader.h"

//#include <atomic>
//#include <filesystem>
//#include <vector>

namespace nkentseu {
    
	class ShaderInputLayout;
	class Shader;
	class ShaderStage;
	/*
	enum class NKENTSEU_API ShaderStageEnum : uint32 {
		Vertex = 1 << 0,  // 0x01
		Fragment = 1 << 1,  // 0x02
		Geometry = 1 << 2,  // 0x04
		TesControl = 1 << 3,  // 0x08
		TesEvaluation = 1 << 4,  // 0x10
		Compute = 1 << 5   // 0x20
	};

	ENABLE_ENUM_TRAITS(ShaderStageEnum, true, true)

	class NKENTSEU_API ShaderStage : public EnumBase<ShaderStage, ShaderStageEnum> {
		public:
			DEFINE_ENUM_BASE_CONSTRUCTOR(ShaderStage, ShaderStageEnum);

			ShaderStage& operator=(const ShaderStage& other) {
				if (this != &other) {
					stage = other.stage;
				}
				return *this;
			}
	};

	/*/
	class NKENTSEU_API ShaderStage {
		public:
		enum class Enum : uint32 {
			Vertex = 1 << 0,  // 0x01
			Fragment = 1 << 1,  // 0x02
			Geometry = 1 << 2,  // 0x04
			TesControl = 1 << 3,  // 0x08
			TesEvaluation = 1 << 4,  // 0x10
			Compute = 1 << 5,  // 0x20
			NotDefine
		};

		#define SHADER_STAGE_TO_STRING(type) ((stage & (uint32)Enum::type != 0) ? #type : "")

		private:
			uint32 stage;
		public:
			ShaderStage() : stage((uint32)ShaderStage::Enum::Vertex) {}
			ShaderStage(ShaderStage::Enum s) : stage((uint32)s) {}
			ShaderStage(uint32 s) : stage(s) {}

			bool operator==(const ShaderStage& other) const {
				return stage == other.stage;
			}

			bool operator!=(const ShaderStage& other) const {
				return stage != other.stage;
			}

			ShaderStage operator|(ShaderStage::Enum s) const {
				return ShaderStage(stage | (uint32)s);
			}

			ShaderStage operator&(ShaderStage::Enum s) const {
				return ShaderStage(stage & (uint32)s);
			}

			ShaderStage& operator|=(ShaderStage::Enum s) {
				stage |= (uint32)s;
				return static_cast<ShaderStage&>(*this);
			}

			ShaderStage& operator&=(ShaderStage::Enum s) {
				stage &= (uint32)s;
				return static_cast<ShaderStage&>(*this);
			}

			operator uint32() const {
				return stage;
			}

			operator ShaderStage::Enum() const {
				return (ShaderStage::Enum)stage;
			}
			
			bool HasStage(ShaderStage::Enum s) const {
				return (stage & (uint32)s) != 0;
			}
			
			bool HasEnum(ShaderStage::Enum s) const {
				return (stage & (uint32)s) != 0;
			}

			static ShaderStage FromString(const std::string& stage) {
				return ShaderStage();
			}

			friend std::ostream& operator<<(std::ostream& os, const ShaderStage& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::string str("");
				str += SHADER_STAGE_TO_STRING(Vertex);
				str += SHADER_STAGE_TO_STRING(Fragment);
				str += SHADER_STAGE_TO_STRING(Geometry);
				str += SHADER_STAGE_TO_STRING(TesControl);
				str += SHADER_STAGE_TO_STRING(TesEvaluation);
				str += SHADER_STAGE_TO_STRING(Compute);
				return str == "" ? "NotDefine" : str;
			}

			friend std::string ToString(const ShaderStage& v) {
				return v.ToString();
			}
	};
	//*/

	struct NKENTSEU_API ShaderInternalType {
		enum Enum {
			NotDefine, Boolean, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Byte, Byte2, Byte3, Byte4, Mat2, Mat3, Mat4
		};

		static uint32 ComponentCount(ShaderInternalType::Enum type);
		static uint32 ComponentElementSize(ShaderInternalType::Enum type);
		static uint32 ComponentSize(ShaderInternalType::Enum type);
	};

	struct NKENTSEU_API BufferUsageType {
		enum Enum {
			StaticDraw, DynamicDraw, StreamDraw
		};
	};

	struct NKENTSEU_API BufferSpecificationType {
		using Code = uint64;
		enum : Code {
			Vertex, Index, Uniform, Storage, Texture, Constant
		};
	};

	struct NKENTSEU_API IndexBufferType {
		using Code = uint64;
		enum : Code {
			NotDefine, UInt8, UInt16, UInt32, UInt64
		};

		static usize SizeOf(IndexBufferType::Code indexType);
	};

	struct NKENTSEU_API SamplerType {
		enum Enum {
			CombineImage, SamplerImage, StorageImage
		};
	};

	struct NKENTSEU_API VertexInputAttribute {
		std::string name;
		ShaderInternalType::Enum type;
		uint32 location;
		uint32 offset;
		uint32 size = 0;
		bool normalized = false;

		VertexInputAttribute() = default;
		VertexInputAttribute(std::string n, ShaderInternalType::Enum t, uint32 loc, bool norm = false);
	};

	struct NKENTSEU_API VertexInputLayout {
		std::vector<VertexInputAttribute> attributes;
		uint32 stride;
		uint32 componentCount = 0;

		VertexInputLayout() = default;
		VertexInputLayout(std::initializer_list<VertexInputAttribute> attrList);
		VertexInputLayout(std::vector<VertexInputAttribute> attrList);

		void AddAttribute(const VertexInputAttribute& attribute);
		const VertexInputAttribute& GetAttribut(uint32 index) const;
		const VertexInputAttribute& GetAttribut(const std::string& name) const;
		std::vector<VertexInputAttribute>::iterator begin();
		std::vector<VertexInputAttribute>::iterator end();
		std::vector<VertexInputAttribute>::const_iterator begin() const;
		std::vector<VertexInputAttribute>::const_iterator end() const;

	private:
		friend struct ShaderInputLayout;
		void CalculateOffsetsAndStride();
	};

	struct NKENTSEU_API UniformInputAttribute {
		std::string name = "";
		ShaderStage stage = ShaderStage::Enum::Vertex;
		BufferUsageType::Enum usage;
		uint32 offset = 0;
		uint32 size = 0;
		usize instance = 1;
		uint32 binding = 0;

		//void* data = nullptr;
		//usize dataSize = 0;

		friend std::ostream& operator<<(std::ostream& os, const UniformInputAttribute& e) {
			return os << e.ToString();
		}

		std::string ToString() const;

		friend std::string ToString(const UniformInputAttribute& v) {
			return v.ToString();
		}

		UniformInputAttribute() = default;
		UniformInputAttribute(std::string n, ShaderStage st, BufferUsageType::Enum usg, uint32 sz, uint32 bind = 0, usize inst = 1);
	};

	struct NKENTSEU_API UniformInputLayout {
		std::vector<UniformInputAttribute> attributes;
		uint32 sizes = 0;
		usize maxBindingPoint = 0;

		UniformInputLayout() = default;
		UniformInputLayout(const UniformInputLayout& layout) = default;
		UniformInputLayout(std::initializer_list<UniformInputAttribute> attrList);
		UniformInputLayout(std::vector<UniformInputAttribute> attrList);

		void AddAttribute(const UniformInputAttribute& attribute);
		const UniformInputAttribute& GetAttribut(uint32 index) const;
		const UniformInputAttribute& GetAttribut(const std::string& name) const;

		std::vector<UniformInputAttribute>::iterator begin();
		std::vector<UniformInputAttribute>::iterator end();
		std::vector<UniformInputAttribute>::const_iterator begin() const;
		std::vector<UniformInputAttribute>::const_iterator end() const;

	private:
		friend struct ShaderInputLayout;
		void CalculateSize();
	};

	struct NKENTSEU_API SamplerInputAttribute {
		std::string name;
		uint32 binding;
		ShaderStage stage;
		SamplerType::Enum type;

		SamplerInputAttribute(std::string n, uint32 bind, ShaderStage st, SamplerType::Enum tp);
	};

	struct NKENTSEU_API SamplerInputLayout {
		std::vector<SamplerInputAttribute> attributes;

		SamplerInputLayout() = default;
		SamplerInputLayout(std::initializer_list<SamplerInputAttribute> attrList);
		SamplerInputLayout(std::vector<SamplerInputAttribute> attrList);

		void AddAttribute(const SamplerInputAttribute& attribute);
		const SamplerInputAttribute& GetAttribut(uint32 index) const;
		const SamplerInputAttribute& GetAttribut(const std::string& name) const;

		std::vector<SamplerInputAttribute>::iterator begin();
		std::vector<SamplerInputAttribute>::iterator end();
		std::vector<SamplerInputAttribute>::const_iterator begin() const;
		std::vector<SamplerInputAttribute>::const_iterator end() const;
	};

	struct NKENTSEU_API PushConstantInputAttribute {
		std::string name;
		ShaderStage stage;
		uint32 offset;
		uint32 size;

		PushConstantInputAttribute(std::string n, ShaderStage st, uint32 sz);
	};

	struct NKENTSEU_API PushConstantInputLayout {
		std::vector<PushConstantInputAttribute> attributes;
		uint32 sizes;

		PushConstantInputLayout() = default;
		PushConstantInputLayout(std::initializer_list<PushConstantInputAttribute> attrList);
		PushConstantInputLayout(std::vector<PushConstantInputAttribute> attrList);

		void AddAttribute(const PushConstantInputAttribute& attribute);
		const PushConstantInputAttribute& GetAttribut(uint32 index) const;
		const PushConstantInputAttribute& GetAttribut(const std::string& name) const;

		std::vector<PushConstantInputAttribute>::iterator begin();
		std::vector<PushConstantInputAttribute>::iterator end();
		std::vector<PushConstantInputAttribute>::const_iterator begin() const;
		std::vector<PushConstantInputAttribute>::const_iterator end() const;

	private:
		friend struct ShaderInputLayout;
		void CalculateSize();
	};

	class NKENTSEU_API ShaderInputLayout {
	public:
		VertexInputLayout vertexInput;
		UniformInputLayout uniformInput;
		SamplerInputLayout samplerInput;
		PushConstantInputLayout pushConstantInput;

		static Memory::Shared<ShaderInputLayout> Create(Memory::Shared<Context> context);

		virtual bool Initialize();
		virtual bool Release();

		virtual bool UpdatePushConstant(const std::string& name, void* data, usize size, Memory::Shared<Shader> shader = nullptr) = 0;
	};

	struct NKENTSEU_API ShaderFilePathAttribut {
		//std::filesystem::path path;
		std::string path;
		ShaderStage stage;

		ShaderFilePathAttribut() = default;
		ShaderFilePathAttribut(const std::string& path, const ShaderStage& stage);
	};

	class NKENTSEU_API ShaderFilePathLayout {
	public:
		std::vector<ShaderFilePathAttribut> attributs;

		ShaderFilePathLayout() = default;
		ShaderFilePathLayout(std::initializer_list<ShaderFilePathAttribut> attrList);
		bool AddPath(const std::string& path, const ShaderStage& stage); // on ajoute pas deux fichier de m�me type
		bool AddPath(const ShaderFilePathAttribut& attrib); // on ajoute pas deux fichier de m�me type

		usize size();
		usize size() const noexcept;

		std::vector<ShaderFilePathAttribut>::iterator begin();
		std::vector<ShaderFilePathAttribut>::iterator end();
		std::vector<ShaderFilePathAttribut>::const_iterator begin() const;
		std::vector<ShaderFilePathAttribut>::const_iterator end() const;
	};

}  //  nkentseu

#endif  // __SHADER_INPUT_LAYOUT_H__!
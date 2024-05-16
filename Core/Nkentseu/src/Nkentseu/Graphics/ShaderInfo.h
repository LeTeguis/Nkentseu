//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 06:01:02 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __SHADER_INFO_H__
#define __SHADER_INFO_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>

#include <string>

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

		static int32 ComponentCount(ShaderDataType::Code shaderDataType);
		static int32 ComponentSize(ShaderDataType::Code shaderDataType);
		static std::string ToString(ShaderDataType::Code shaderDataType);
		static ShaderType::Code FromString(const std::string& shaderDataTypeStr);
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
}  //  nkentseu

#endif  // __SHADER_INFO_H__!
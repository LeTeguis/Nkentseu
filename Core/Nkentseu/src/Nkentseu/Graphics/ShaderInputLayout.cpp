//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-29 at 10:04:30 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "ShaderInputLayout.h"
#include <Logger/Formatter.h>

#include "Internal/Opengl/OpenglShaderInputLayout.h"
#include "Internal/Vulkan/VulkanShaderInputLayout.h"
#include "Shader.h"

namespace nkentseu {
    
	uint32 ShaderInternalType::ComponentCount(ShaderInternalType::Enum type) {
		switch (type) {
		case Boolean: return 1;
		case Float: return 1;
		case Float2: return 2;
		case Float3: return 3;
		case Float4: return 4;
		case Int: return 1;
		case Int2: return 2;
		case Int3: return 3;
		case Int4: return 4;
		case Byte: return 1;
		case Byte2: return 2;
		case Byte3: return 3;
		case Byte4: return 4;
		case Mat2: return 4;
		case Mat3: return 9;
		case Mat4: return 16;
		default: return 0;
		}
	}

	uint32 ShaderInternalType::ComponentElementSize(ShaderInternalType::Enum type) {
		switch (type) {
		case Boolean: return sizeof(bool);
		case Float: return sizeof(float32);
		case Float2: return sizeof(float32);
		case Float3: return sizeof(float32);
		case Float4: return sizeof(float32);
		case Int: return sizeof(int32);
		case Int2: return sizeof(int32);
		case Int3: return sizeof(int32);
		case Int4: return sizeof(int32);
		case Byte: return sizeof(int8);
		case Byte2: return sizeof(int8);
		case Byte3: return sizeof(int8);
		case Byte4: return sizeof(int8);
		case Mat2: return sizeof(float32);
		case Mat3: return sizeof(float32);
		case Mat4: return sizeof(float32);
		default: return 0;
		}
	}

	uint32 ShaderInternalType::ComponentSize(ShaderInternalType::Enum type) {
		return ComponentCount(type) * ComponentElementSize(type);
	}

	usize IndexBufferType::SizeOf(IndexBufferType::Code indexType) {
		switch (indexType) {
		case UInt8: return sizeof(uint8);
		case UInt16: return sizeof(uint16);
		case UInt32: return sizeof(uint32);
		case UInt64: return sizeof(uint64);
		default: return 0;
		}
	}

	// VertexInputAttribute
	VertexInputAttribute::VertexInputAttribute(std::string n, ShaderInternalType::Enum t, uint32 loc, bool norm)
		: name(std::move(n)), type(t), location(loc), normalized(norm) {
		size = ShaderInternalType::ComponentSize(type);
	}

	// VertexInputLayout
	VertexInputLayout::VertexInputLayout(std::initializer_list<VertexInputAttribute> attrList)
		: attributes(attrList), stride(0), componentCount(0) {
		for (const auto& attr : attrList) {
			AddAttribute(attr);
		}
	}

	VertexInputLayout::VertexInputLayout(std::vector<VertexInputAttribute> attrList)
		: attributes(std::move(attrList)) {}

	void VertexInputLayout::AddAttribute(const VertexInputAttribute& attribute) {
		attributes.push_back(attribute);
	}

	const VertexInputAttribute& VertexInputLayout::GetAttribut(uint32 index) const {
		return attributes.at(index);
	}

	const VertexInputAttribute& VertexInputLayout::GetAttribut(const std::string& name) const {
		for (const auto& attr : attributes) {
			if (attr.name == name) {
				return attr;
			}
		}
		throw std::runtime_error("Attribute not found");
	}

	void VertexInputLayout::CalculateOffsetsAndStride() {
		uint32 offset = 0;
		for (auto& attr : attributes) {
			attr.offset = offset;
			offset += attr.size;
			componentCount += ShaderInternalType::ComponentElementSize(attr.type);
		}
		stride = offset;
	}

	std::vector<VertexInputAttribute>::iterator VertexInputLayout::begin() {
		return attributes.begin();
	}

	std::vector<VertexInputAttribute>::iterator VertexInputLayout::end() {
		return attributes.end();
	}

	std::vector<VertexInputAttribute>::const_iterator VertexInputLayout::begin() const {
		return attributes.begin();
	}

	std::vector<VertexInputAttribute>::const_iterator VertexInputLayout::end() const {
		return attributes.end();
	}

	std::string UniformInputAttribute::ToString() const
	{
		return FORMATTER.Format("name : {0}, binding : {1}, size : {2}, offset : {3}, instance : {4}", name, binding, size, offset, instance);
	}

	// UniformInputAttribute
	UniformInputAttribute::UniformInputAttribute(std::string n, ShaderStage st, BufferUsageType::Enum usg, uint32 sz, uint32 bind, usize inst)
		: name(std::move(n)), stage(st), usage(usg), size(sz), instance(inst), binding(bind) {}

	// UniformInputLayout
	UniformInputLayout::UniformInputLayout(std::initializer_list<UniformInputAttribute> attrList)
		: sizes(0) {
		for (const auto& attr : attrList) {
			AddAttribute(attr);
		}
	}

	UniformInputLayout::UniformInputLayout(std::vector<UniformInputAttribute> attrList)
		: attributes(std::move(attrList)) {}

	void UniformInputLayout::AddAttribute(const UniformInputAttribute& attribute) {
		attributes.push_back(attribute);
	}

	const UniformInputAttribute& UniformInputLayout::GetAttribut(uint32 index) const {
		return attributes.at(index);
	}

	const UniformInputAttribute& UniformInputLayout::GetAttribut(const std::string& name) const {
		for (const auto& attr : attributes) {
			if (attr.name == name) {
				return attr;
			}
		}
		throw std::runtime_error("Attribute not found");
	}

	void UniformInputLayout::CalculateSize() {
		uint32 totalSize = 0;
		for (const auto& attr : attributes) {
			totalSize += attr.size * attr.instance;

			if (attr.binding >= maxBindingPoint) {
				maxBindingPoint = attr.binding + 1;
			}
		}
		sizes = totalSize;
	}

	std::vector<UniformInputAttribute>::iterator UniformInputLayout::begin() {
		return attributes.begin();
	}

	std::vector<UniformInputAttribute>::iterator UniformInputLayout::end() {
		return attributes.end();
	}

	std::vector<UniformInputAttribute>::const_iterator UniformInputLayout::begin() const {
		return attributes.begin();
	}

	std::vector<UniformInputAttribute>::const_iterator UniformInputLayout::end() const {
		return attributes.end();
	}

	// SamplerInputAttribute
	SamplerInputAttribute::SamplerInputAttribute(std::string n, uint32 bind, ShaderStage st, SamplerType::Enum tp)
		: name(std::move(n)), binding(bind), stage(st), type(tp) {}

	// SamplerInputLayout
	SamplerInputLayout::SamplerInputLayout(std::initializer_list<SamplerInputAttribute> attrList){
		for (const auto& attr : attrList) {
			AddAttribute(attr);
		}
	}

	SamplerInputLayout::SamplerInputLayout(std::vector<SamplerInputAttribute> attrList)
		: attributes(std::move(attrList)) {}

	void SamplerInputLayout::AddAttribute(const SamplerInputAttribute& attribute) {
		attributes.push_back(attribute);
	}

	const SamplerInputAttribute& SamplerInputLayout::GetAttribut(uint32 index) const {
		return attributes.at(index);
	}

	const SamplerInputAttribute& SamplerInputLayout::GetAttribut(const std::string& name) const {
		for (const auto& attr : attributes) {
			if (attr.name == name) {
				return attr;
			}
		}
		throw std::runtime_error("Attribute not found");
	}

	std::vector<SamplerInputAttribute>::iterator SamplerInputLayout::begin() {
		return attributes.begin();
	}

	std::vector<SamplerInputAttribute>::iterator SamplerInputLayout::end() {
		return attributes.end();
	}

	std::vector<SamplerInputAttribute>::const_iterator SamplerInputLayout::begin() const {
		return attributes.begin();
	}

	std::vector<SamplerInputAttribute>::const_iterator SamplerInputLayout::end() const {
		return attributes.end();
	}

	// PushConstantInputAttribute
	PushConstantInputAttribute::PushConstantInputAttribute(std::string n, ShaderStage st, uint32 sz)
		: name(std::move(n)), stage(st), size(sz) {}

	// PushConstantInputLayout
	PushConstantInputLayout::PushConstantInputLayout(std::initializer_list<PushConstantInputAttribute> attrList)
		: sizes(0) {
		for (const auto& attr : attrList) {
			AddAttribute(attr);
		}
	}

	PushConstantInputLayout::PushConstantInputLayout(std::vector<PushConstantInputAttribute> attrList)
		: attributes(std::move(attrList)) {}

	void PushConstantInputLayout::AddAttribute(const PushConstantInputAttribute& attribute) {
		attributes.push_back(attribute);
		sizes += attribute.size;
	}

	const PushConstantInputAttribute& PushConstantInputLayout::GetAttribut(uint32 index) const {
		return attributes.at(index);
	}

	const PushConstantInputAttribute& PushConstantInputLayout::GetAttribut(const std::string& name) const {
		for (const auto& attr : attributes) {
			if (attr.name == name) {
				return attr;
			}
		}
		throw std::runtime_error("Attribute not found");
	}

	void PushConstantInputLayout::CalculateSize() {
		uint32 totalSize = 0;
		for (auto& attr : attributes) {
			attr.offset = totalSize;
			totalSize += attr.size;
		}
		sizes = totalSize;
	}

	std::vector<PushConstantInputAttribute>::iterator PushConstantInputLayout::begin() {
		return attributes.begin();
	}

	std::vector<PushConstantInputAttribute>::iterator PushConstantInputLayout::end() {
		return attributes.end();
	}

	std::vector<PushConstantInputAttribute>::const_iterator PushConstantInputLayout::begin() const {
		return attributes.begin();
	}

	std::vector<PushConstantInputAttribute>::const_iterator PushConstantInputLayout::end() const {
		return attributes.end();
	}

	Memory::Shared<ShaderInputLayout> ShaderInputLayout::Create(Memory::Shared<Context> context)
	{
		if (context == nullptr) return nullptr;

		if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			return Memory::Alloc<OpenglShaderInputLayout>(context);
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
			return Memory::Alloc<VulkanShaderInputLayout>(context);
		}

		return nullptr;
	}

	bool ShaderInputLayout::Initialize()
	{
		vertexInput.CalculateOffsetsAndStride();
		uniformInput.CalculateSize();
		pushConstantInput.CalculateSize();
		return true;
	}

	bool ShaderInputLayout::Release()
	{
		vertexInput.attributes.clear();
		uniformInput.attributes.clear();
		pushConstantInput.attributes.clear();
		samplerInput.attributes.clear();
		return true;
	}

	ShaderFilePathAttribut::ShaderFilePathAttribut(const std::string& path, const ShaderStage& stage) : path(path), stage(stage){
	}

	ShaderFilePathLayout::ShaderFilePathLayout(std::initializer_list<ShaderFilePathAttribut> attrList)
	{
		for (const auto& attr : attrList) {
			AddPath(attr);
		}
	}

	bool ShaderFilePathLayout::AddPath(const std::string& path, const ShaderStage& stage)
	{
		auto it = std::find_if(attributs.begin(), attributs.end(), [&](const ShaderFilePathAttribut& attribut) {
			return (attribut.stage == stage);
			});

		if (it != attributs.end()) {
			return false;
		}

		attributs.emplace_back(path, stage);
		return true;
	}

	bool ShaderFilePathLayout::AddPath(const ShaderFilePathAttribut& attrib)
	{
		return AddPath(attrib.path, attrib.stage);
	}

	usize ShaderFilePathLayout::size()
	{
		return attributs.size();
	}

	usize ShaderFilePathLayout::size() const noexcept
	{
		return attributs.size();
	}

	std::vector<ShaderFilePathAttribut>::iterator ShaderFilePathLayout::begin() {
		return attributs.begin();
	}

	std::vector<ShaderFilePathAttribut>::iterator ShaderFilePathLayout::end() {
		return attributs.end();
	}

	std::vector<ShaderFilePathAttribut>::const_iterator ShaderFilePathLayout::begin() const {
		return attributs.begin();
	}

	std::vector<ShaderFilePathAttribut>::const_iterator ShaderFilePathLayout::end() const {
		return attributs.end();
	}

}  //  nkentseu
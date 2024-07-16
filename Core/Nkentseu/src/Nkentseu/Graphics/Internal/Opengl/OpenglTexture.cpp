//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-16 at 11:23:31 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglTexture.h"
#include <Logger/Formatter.h>
#include <glad/gl.h>
#include "OpenGLUtils.h"

namespace nkentseu {
	using namespace maths;

	OpenglTexture2D::OpenglTexture2D(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil) : m_Context(Memory::SharedCast<OpenglContext>(context)), m_Handle(0)
	{
		if (sil != nullptr) {
			m_InputLayout = sil->samplerInput;
		}
	}

	OpenglTexture2D::~OpenglTexture2D()
	{
	}

	bool OpenglTexture2D::Create(TextureFormat textureFormat, const maths::Vector2u& size) {

		OpenGLResult result;
		bool first = true;

		if ((size.x == 0) || (size.y == 0)) {
			Log_nts.Error("Failed to create texture, invalid size ({0}x{1})", size.x, size.y);
			return result.success;
		}

		m_Format = textureFormat;

		// Compute the internal texture dimensions depending on NPOT textures support
		const maths::Vector2u actualSize(GetValidSize(m_Context, size.x), GetValidSize(m_Context, size.y));

		// Check the maximum texture size
		const unsigned int maxSize = GetMaximumSize(m_Context);
		if ((actualSize.x > maxSize) || (actualSize.y > maxSize)) {
			Log_nts.Error("Failed to create texture, its internal size is too high ({0}x{1}, maximum is {2}x{2})", actualSize.x, actualSize.y, maxSize);
			return result.success;
		}
		m_Size = size;
		m_ActualSize = actualSize;
		m_PixelsFlipped = false;
		m_FboAttachment = false; 

		if (m_Handle == 0) {
			glCheckError(first, result, glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle), "");
		}
		glCheckError(first, result, glTextureStorage2D(m_Handle, 1, GLConvert::ToTextureFormat(m_Format), m_ActualSize.x, m_ActualSize.y), "");

		glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, m_IsSmooth ? GL_LINEAR : GL_NEAREST), "");
		glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, m_IsSmooth ? GL_LINEAR : GL_NEAREST), "");

		glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, m_IsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE), "");
		glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, m_IsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE), "");

		m_HasMipmap = false;
		return result.success;
	}
	bool OpenglTexture2D::Create(const std::filesystem::path& filename, const maths::IntRect& area) {
		Image image;
		m_Path = filename;
		return image.LoadFromFile(filename) && Create(image, area);
	}

	bool OpenglTexture2D::Create(const void* data, std::size_t size, const maths::IntRect& area) {
		Image image;
		return image.LoadFromMemory(data, size) && Create(image, area);
	}

	bool OpenglTexture2D::Create(InputStream& stream, const maths::IntRect& area) {
		Image image;
		return image.LoadFromStream(stream) && Create(image, area);
	}

	bool OpenglTexture2D::Create(const Image& image, const maths::IntRect& area) {
		const int32 width = maths::Vector2i(image.GetSize().x, image.GetSize().y).x;
		const int32 height = maths::Vector2i(image.GetSize().x, image.GetSize().y).y;

		OpenGLResult result;
		bool first = true;

		if (area.width == 0 || (area.height == 0) ||
			((area.x <= 0) && (area.y <= 0) && (area.width >= width) && (area.height >= height))) {
			if (Create(m_Format, image.GetSize())) {
				Update(image);
				return true;
			}
			else {
				return result.success;
			}
		}
		else {
			maths::IntRect rectangle = area;
			if (rectangle.x < 0)
				rectangle.x = 0;
			if (rectangle.y < 0)
				rectangle.y = 0;
			if (rectangle.x + rectangle.width > width)
				rectangle.width = width - rectangle.x;
			if (rectangle.y + rectangle.height > height)
				rectangle.height = height - rectangle.y;

			if (Create(TextureFormat::Enum::RGBA8, maths::Vector2u(rectangle.width, rectangle.height))) {

				const uint8* pixels = image.GetPixels() + 4 * (rectangle.x + (width * rectangle.y));

				glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, m_Handle), "");
				for (int i = 0; i < rectangle.height; ++i) {
					glCheckError(first, result, glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, rectangle.width, 1, GLConvert::ToTextureDataFormat(m_Format), GL_UNSIGNED_BYTE, pixels), "");
					pixels += 4 * width;
				}

				glCheckError(first, result, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_IsSmooth ? GL_LINEAR : GL_NEAREST), "");
				m_HasMipmap = false;
				// Force an OpenGL flush, so that the texture will appear updated
				// in all contexts immediately (solves problems in multi-threaded apps)
				glCheckError(first, result, glFlush(), "");
				return result.success;
			}
			else {
				return result.success;
			}
		}
	}

	Color* OpenglTexture2D::GetColors()
	{
		return CopyToImage().GetColors();
	}

	uint8* OpenglTexture2D::GetPixels()
	{
		return CopyToImage().GetPixels();
	}

	maths::Vector2u OpenglTexture2D::GetSize() const {
		return m_Size;
	}

	Image OpenglTexture2D::CopyToImage() const {
		if (!m_Handle)
			return Image();

		OpenGLResult result;
		bool first = true;

		std::vector<std::uint8_t> pixels(m_Size.x * m_Size.y * 4);

		if ((m_Size == m_ActualSize) && !m_PixelsFlipped) {
			glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, m_Handle), "");
			glCheckError(first, result, glGetTexImage(GL_TEXTURE_2D, 0, GLConvert::ToTextureFormat(m_Format), GL_UNSIGNED_BYTE, pixels.data()), "");
		}
		else {
			std::vector<std::uint8_t> allPixels(m_ActualSize.x * m_ActualSize.y * 4);
			glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, m_Handle), "");
			glCheckError(first, result, glGetTexImage(GL_TEXTURE_2D, 0, GLConvert::ToTextureFormat(m_Format), GL_UNSIGNED_BYTE, allPixels.data()), "");

			const std::uint8_t* src = allPixels.data();
			std::uint8_t* dst = pixels.data();
			int srcPitch = static_cast<int>(m_ActualSize.x * 4);
			const unsigned int dstPitch = m_Size.x * 4;

			if (m_PixelsFlipped) {
				src += static_cast<unsigned int>(srcPitch * static_cast<int>((m_Size.y - 1)));
				srcPitch = -srcPitch;
			}

			for (unsigned int i = 0; i < m_Size.y; ++i) {
				std::memcpy(dst, src, dstPitch);
				src += srcPitch;
				dst += dstPitch;
			}
		}

		Image image;
		image.Create(m_Size, pixels.data());

		return image;
	}
	void OpenglTexture2D::Update(const std::uint8_t* pixels) { Update(pixels, m_Size, { 0, 0 }); }

	void OpenglTexture2D::Update(const std::uint8_t* pixels, const maths::Vector2u& size, const maths::Vector2i& dest) {
		if (!((dest.x + size.x <= m_Size.x) && (dest.y + size.y <= m_Size.y))) {
			return;
		}

		OpenGLResult result;
		bool first = true;

		if (pixels && m_Handle) {
			glCheckError(first, result, glTextureSubImage2D(m_Handle, 0, dest.x, dest.y, size.x, size.y, GLConvert::ToTextureDataFormat(m_Format), GL_UNSIGNED_BYTE, pixels), "");
		}
	}

	void OpenglTexture2D::Update(const Texture& texture) {
		Update(texture, { 0, 0 });
	}

	void OpenglTexture2D::Update(const Texture& texture, const maths::Vector2i& dest) {
		const OpenglTexture2D* texture_ = static_cast<const OpenglTexture2D*>(&texture);

		if (texture_ == nullptr || !((dest.x + texture_->m_Size.x <= m_Size.x) && (dest.y + texture_->m_Size.y <= m_Size.y))) {
			return;
		}

		if (!m_Handle || !texture_->m_Handle)
			return;

		if (GLAD_GL_EXT_framebuffer_object && GLAD_GL_EXT_framebuffer_blit) {

			OpenGLResult result;
			bool first = true;

			GLint readFramebuffer = 0;
			GLint drawFramebuffer = 0;

			glCheckError(first, result, glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer), "");
			glCheckError(first, result, glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer), "");

			GLuint sourceFrameBuffer = 0;
			GLuint destFrameBuffer = 0;
			glCheckError(first, result, glGenFramebuffers(1, &sourceFrameBuffer), "");
			glCheckError(first, result, glGenFramebuffers(1, &destFrameBuffer), "");

			if (!sourceFrameBuffer || !destFrameBuffer) {
				Log_nts.Error("Cannot copy texture, failed to create a frame buffer object");
				return;
			}

			glCheckError(first, result, glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFrameBuffer), "");
			glCheckError(first, result, glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_->m_Handle, 0), "");

			glCheckError(first, result, glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFrameBuffer), "");
			glCheckError(first, result, glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Handle, 0), "");

			GLenum sourceStatus;
			glCheckError(first, result, sourceStatus = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER), "");

			GLenum destStatus;
			glCheckError(first, result, destStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER), "");

			if ((sourceStatus == GL_FRAMEBUFFER_COMPLETE) && (destStatus == GL_FRAMEBUFFER_COMPLETE)) {
				glCheckError(first, result, glBlitFramebuffer(0,
					texture_->m_PixelsFlipped ? static_cast<GLint>(texture_->m_Size.y) : 0,
					static_cast<GLint>(texture_->m_Size.x),
					texture_->m_PixelsFlipped ? 0 : static_cast<GLint>(texture_->m_Size.y),
					static_cast<GLint>(dest.x),
					static_cast<GLint>(dest.y),
					static_cast<GLint>(dest.x + texture_->m_Size.x),
					static_cast<GLint>(dest.y + texture_->m_Size.y),
					GL_COLOR_BUFFER_BIT,
					GL_NEAREST), "");
			}
			else {
				Log_nts.Error("Cannot copy texture, failed to link texture to frame buffer");
			}

			glCheckError(first, result, glBindFramebuffer(GL_FRAMEBUFFER, readFramebuffer), "");
			glCheckError(first, result, glBindFramebuffer(GL_FRAMEBUFFER, drawFramebuffer), "");
			glCheckError(first, result, glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(readFramebuffer)), "");
			glCheckError(first, result, glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(drawFramebuffer)), "");

			glCheckError(first, result, glDeleteFramebuffers(1, &sourceFrameBuffer), "");
			glCheckError(first, result, glDeleteFramebuffers(1, &destFrameBuffer), "");

			glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, m_Handle), "");
			glCheckError(first, result, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_IsSmooth ? GL_LINEAR : GL_NEAREST), "");
			m_HasMipmap = false;
			m_PixelsFlipped = false;
			glCheckError(first, result, glFlush(), "");

			return;
		}

		Update(texture_->CopyToImage(), dest);
	}

	void OpenglTexture2D::Update(const Image& image) {
		Update(image.GetPixels(), image.GetSize(), { 0, 0 });
	}

	void OpenglTexture2D::Update(const Image& image, const maths::Vector2i& dest) {
		Update(image.GetPixels(), image.GetSize(), dest);
	}

	void OpenglTexture2D::SetSmooth(bool smooth) {
		if (smooth != m_IsSmooth) {
			m_IsSmooth = smooth;
			if (m_Handle) {

				OpenGLResult result;
				bool first = true;

				glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, m_Handle), "");
				glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, m_IsSmooth ? GL_LINEAR : GL_NEAREST), "");
				if (m_HasMipmap) {
					glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, m_IsSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST), "");
				}
				else {
					glCheckError(first, result, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_IsSmooth ? GL_LINEAR : GL_NEAREST), "");
				}
				glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, 0), "");
			}
		}
	}

	bool OpenglTexture2D::IsSmooth() const {
		return m_IsSmooth;
	}

	void OpenglTexture2D::SetTextureFormat(TextureFormat textureFormat) {
		if (textureFormat != m_Format) {

			OpenGLResult result;
			bool first = true;

			m_Format = textureFormat;
			glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, m_Handle), "");
			glCheckError(first, result, glTextureStorage2D(m_Handle, 1, GLConvert::ToTextureFormat(m_Format), m_ActualSize.x, m_ActualSize.y), "");

			glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, m_IsSmooth ? GL_LINEAR : GL_NEAREST), "");
			glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, m_IsSmooth ? GL_LINEAR : GL_NEAREST), "");

			glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, m_IsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE), "");
			glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, m_IsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE), "");
			glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, 0), "");
		}
	}

	TextureFormat OpenglTexture2D::GetTextureFormat() const {
		return m_Format;
	}

	void OpenglTexture2D::SetRepeated(bool repeated) {
		if (repeated != m_IsRepeated) {
			m_IsRepeated = repeated;

			if (m_Handle) {

				OpenGLResult result;
				bool first = true;

				glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, m_Handle), "");
				glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, m_IsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE), "");
				glCheckError(first, result, glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, m_IsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE), "");
				glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, 0), "");
			}
		}
	}

	bool OpenglTexture2D::IsRepeated() const {
		return m_IsRepeated;
	}

	bool OpenglTexture2D::GenerateMipmap() {
		if (!m_Handle)
			return false;

		OpenGLResult result;
		bool first = true;

		glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, m_Handle), "");
		glCheckError(first, result, glGenerateMipmap(GL_TEXTURE_2D), "");
		glCheckError(first, result, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_IsSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR), "");
		glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, 0), "");

		m_HasMipmap = true;

		return true;
	}

	void OpenglTexture2D::Swap(Texture& right) noexcept {
		OpenglTexture2D* right_ = static_cast<OpenglTexture2D*>(&right);

		if (right_ == nullptr)return;

		std::swap(m_Size, right_->m_Size);
		std::swap(m_ActualSize, right_->m_ActualSize);
		std::swap(m_Handle, right_->m_Handle);
		std::swap(m_IsSmooth, right_->m_IsSmooth);
		std::swap(m_Format, right_->m_Format);
		std::swap(m_IsRepeated, right_->m_IsRepeated);
		std::swap(m_PixelsFlipped, right_->m_PixelsFlipped);
		std::swap(m_FboAttachment, right_->m_FboAttachment);
		std::swap(m_HasMipmap, right_->m_HasMipmap);
	}

	void OpenglTexture2D::InvalidateMipmap() {
		if (!m_HasMipmap)
			return;

		OpenGLResult result;
		bool first = true;

		glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, m_Handle), "");
		glCheckError(first, result, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_IsSmooth ? GL_LINEAR : GL_NEAREST), "");
		glCheckError(first, result, glBindTexture(GL_TEXTURE_2D, 0), "");

		m_HasMipmap = false;
	}

	void OpenglTexture2D::Bind(const std::string& name)
	{
		OpenGLResult result;
		bool first = true;

		if (m_Handle) {
			for (auto attribut : m_InputLayout) {
				if (attribut.name == name) {
					glCheckError(first, result, glBindTextureUnit(attribut.binding, m_Handle), "cannot bind {0}", name);
					return;
				}
			}
		}
		Log_nts.Error("cannot bind {0}", name);
	}

	void OpenglTexture2D::Bind(uint32 binding)
	{
		OpenGLResult result;
		bool first = true;

		if (m_Handle) {
			for (auto attribut : m_InputLayout) {
				if (attribut.binding == binding) {
					glCheckError(first, result, glBindTextureUnit(attribut.binding, m_Handle), "cannot bind {0}", attribut.name);
					return;
				}
			}
		}
		Log_nts.Error("cannot bind {0}", binding);
	}

	const std::filesystem::path& OpenglTexture2D::GetPath() const {
		return m_Path;
	}

	uint32 OpenglTexture2D::GetMaximumSize(Memory::Shared<Context> context)
	{

		OpenGLResult result;
		bool first = true;

		GLint maxTextureSize;
		glCheckError(first, result, glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize), "");
		return maxTextureSize;
	}

	uint32 OpenglTexture2D::GetValidSize(Memory::Shared<Context> context, uint32 size)
	{
		if (size <= GetMaximumSize(context)) {
			return size;
		}
		else {
			uint32 powerOfTwo = 1;
			while (powerOfTwo < GetMaximumSize(context))
				powerOfTwo *= 2;

			return powerOfTwo;
		}
	}

	void OpenglTexture2D::Destroy()
	{
	}

}  //  nkentseu
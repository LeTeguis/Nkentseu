//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-18 at 07:57:18 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NSystemPch/ntspch.h"
#include "FileInputStream.h"

#include <memory>
#include <cstddef>

namespace nkentseu {

	void FileInputStream::FileCloser::operator()(std::FILE* file) {
		std::fclose(file);
	}
    
	FileInputStream::FileInputStream() = default;
	FileInputStream::~FileInputStream() = default;
	FileInputStream::FileInputStream(FileInputStream&&) noexcept = default;
	FileInputStream& FileInputStream::operator=(FileInputStream&&) noexcept = default;

	bool FileInputStream::Open(const std::filesystem::path& filename) {
#ifdef NKENTSEU_PLATFORM_WINDOWS
		m_File.reset(_wfopen(filename.c_str(), L"rb"));
#else
		m_File.reset(std::fopen(filename.c_str(), "rb"));
#endif
		return m_File != nullptr;
	}

	int64 FileInputStream::Read(void* data, int64 size) {
		if (m_File)
			return static_cast<int64>(std::fread(data, 1, static_cast<std::size_t>(size), m_File.get()));
		else
			return -1;
	}

	int64 FileInputStream::Seek(int64 position) {
		if (m_File) {
			if (std::fseek(m_File.get(), static_cast<long>(position), SEEK_SET))
				return -1;

			return Tell();
		}
		else {
			return -1;
		}
	}

	int64 FileInputStream::Tell() {
		if (m_File)
			return std::ftell(m_File.get());
		else
			return -1;
	}

	int64 FileInputStream::GetSize() {
		if (m_File) {
			const int64 position = Tell();
			std::fseek(m_File.get(), 0, SEEK_END);
			const int64 size = Tell();

			if (Seek(position) == -1)
				return -1;

			return size;
		}
		else {
			return -1;
		}
	}

}  //  nkentseu
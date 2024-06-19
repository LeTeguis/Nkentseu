//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-18 at 07:57:18 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __FILE_INPUT_STREAM_H__
#define __FILE_INPUT_STREAM_H__

#pragma once

#include <System/System.h>

#include "InputStream.h"

#include <filesystem>
#include <cstdio>

namespace nkentseu {
    
    class NKENTSEU_API FileInputStream : public InputStream {
	public:
		FileInputStream();
		~FileInputStream() override;
		FileInputStream(const FileInputStream&) = delete;
		FileInputStream& operator=(const FileInputStream&) = delete;
		FileInputStream(FileInputStream&&) noexcept;
		FileInputStream& operator=(FileInputStream&&) noexcept;
		bool Open(const std::filesystem::path& filename);
		int64 Read(void* data, int64 size) override;
		int64 Seek(int64 position) override;
		int64 Tell() override;
		int64 GetSize() override;

	private:
		struct FileCloser {
			void operator()(std::FILE* file);
		};

		std::unique_ptr<std::FILE, FileCloser> m_File;
	};

}  //  nkentseu

#endif  // __FILE_INPUT_STREAM_H__!
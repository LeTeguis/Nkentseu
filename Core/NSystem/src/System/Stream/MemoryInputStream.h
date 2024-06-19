//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-18 at 07:57:57 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __MEMORY_INPUT_STREAM_H__
#define __MEMORY_INPUT_STREAM_H__

#pragma once

#include <System/System.h>
#include "InputStream.h"

namespace nkentseu {
    
    class NKENTSEU_API MemoryInputStream : public InputStream {
	public:
		MemoryInputStream();
		void Open(const void* data, std::size_t sizeInBytes);
		int64 Read(void* data, int64 size) override;
		int64 Seek(int64 position) override;
		int64 Tell() override;
		int64 GetSize() override;

	private:
		const std::byte* m_Data{};
		int64     m_Size{};
		int64     m_Offset{};
	};

}  //  nkentseu

#endif  // __MEMORY_INPUT_STREAM_H__!
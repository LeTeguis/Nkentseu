//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-18 at 07:57:57 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "MemoryInputStream.h"

namespace nkentseu {
	MemoryInputStream::MemoryInputStream() = default;

	void MemoryInputStream::Open(const void* data, std::size_t sizeInBytes) {
		m_Data = static_cast<const std::byte*>(data);
		m_Size = static_cast<int64>(sizeInBytes);
		m_Offset = 0;
	}

	int64 MemoryInputStream::Read(void* data, int64 size) {
		if (!m_Data)
			return -1;

		const int64 endPosition = m_Offset + size;
		const int64 count = endPosition <= m_Size ? size : m_Size - m_Offset;

		if (count > 0) {
			std::memcpy(data, m_Data + m_Offset, static_cast<std::size_t>(count));
			m_Offset += count;
		}

		return count;
	}

	int64 MemoryInputStream::Seek(int64 position) {
		if (!m_Data)
			return -1;

		m_Offset = position < m_Size ? position : m_Size;
		return m_Offset;
	}

	int64 MemoryInputStream::Tell() {
		if (!m_Data)
			return -1;

		return m_Offset;
	}

	int64 MemoryInputStream::GetSize() {
		if (!m_Data)
			return -1;

		return m_Size;
	}
}  //  nkentseu
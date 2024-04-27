//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/19/2024 at 12:17:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_OPERATION_H__
#define __NKENTSEU_OPERATION_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include <string>

namespace nkentseu {
	class Operation {
	public:
		using Code = uint64;

		enum : Code {
			NotDefine = 0,
			Add,
			Multiply,
			Subtract,
			Divide,
			Modulo,
			And,
			Or,
			Xor
		};

		std::string ToString(Operation::Code operation);
		Operation::Code FromString(const std::string& operation);
	};
} // namespace nkentseu

#endif    // __NKENTSEU_OPERATION_H__
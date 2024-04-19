//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/19/2024 at 12:17:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_OPERATION_H__
#define __NKENTSEU_OPERATION_H__

#pragma once

#include "System/System.h"

namespace nkentseu {
	enum class Operation {
		Add,
		Multiply,
		Subtract,
		Divide,
		Modulo,
		And,
		Or,
		Xor
	};
} // namespace nkentseu

#endif    // __NKENTSEU_OPERATION_H__
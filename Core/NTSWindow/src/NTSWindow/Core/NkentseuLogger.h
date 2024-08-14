//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 3:18:31 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_NKENTSEULOGGER_H__
#define __NKENTSEU_NKENTSEULOGGER_H__

#pragma once

#include "NTSCore/System.h"
#include <NtsLogger/Log.h>

namespace nkentseu {

	#define NkentseuNameLogger "Nkentseu"
	#define Log_nts		LogBase(NkentseuNameLogger)
	#define Lognts		LogBase(NkentseuNameLogger)

} // namespace nkentseu

#endif    // __NKENTSEU_NKENTSEULOGGER_H__
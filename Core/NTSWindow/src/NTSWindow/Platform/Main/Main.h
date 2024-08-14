//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/14/2024 at 10:19:02 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_MAIN_H__
#define __NKENTSEU_MAIN_H__

#pragma once

#include <NTSCore/System.h>
#include <NTSCore/Nature/Base.h>
#include <string>
#include <vector>

nkentseu::int32 Main(const nkentseu::ARGV& argv);

class nkentseu_application {
public:
	virtual nkentseu::int32 main() = 0;
};


#endif
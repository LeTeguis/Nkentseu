//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024/04/05.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef NKENTSEU_ENTRY_POINT_H_
#define NKENTSEU_ENTRY_POINT_H_

#pragma once

#include <iostream>

#include <Nkentseu/Nkentseu.h>
#include "Log.h"
#include "Application.h"

extern nkentseu::Memory::Shared<nkentseu::Application> nkentseu::MainApplication(const nkentseu::ARGV& argv);

nkentseu::int32 Main(const nkentseu::ARGV& argv) {
    nkentseu::Log.Debug("Unkeny Engine");

	nkentseu::Memory::Shared<nkentseu::Application> app = nkentseu::MainApplication(argv);

	if (app->Initialize()) {

		app->Run();
		return (EXIT_SUCCESS);
	}

    return (EXIT_FAILURE);
}

#endif  // NKENTSEU_ENTRY_POINT_H_
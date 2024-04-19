//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 4/13/2024 at 9:55:04 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_WINDOW_LINUX_H__
#define __NKENTSEU_WINDOW_LINUX_H__

#include "Main.h"
#include "System/System.h"
#include "System/Nature/Base.h"
#include "Nkentseu/Platform/PlatformState.h"

#ifdef NKENTSEU_PLATFORM_LINUX

extern nkentseu::int32 Main(const nkentseu::ARGV& argv);

int main(int argc, const char** argv) {
    nkentseu::PlatformState.Init(argc, argv);
    nkentseu::int32 result = Main(nkentseu::PlatformState.argv);
    nkentseu::PlatformState.Close();
    return (result);
}

#endif


#endif	//	__NKENTSEU_WINDOW_LINUX_H__
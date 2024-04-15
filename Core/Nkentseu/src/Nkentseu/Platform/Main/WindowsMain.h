//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 9:55:04 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_WINDOW_MAIN_H__
#define __NKENTSEU_WINDOW_MAIN_H__

#include "Main.h"
#include "System/System.h"
#include "Nkentseu/Platform/PlatformState.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

extern nkentseu::int32 Main(const nkentseu::ARGV& argv);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw) {
    #ifdef NKENTSEU_DEBUG
    AllocConsole();
    FILE* pCout;
    FILE* pCerr;
    freopen_s(&pCout, "CONOUT$", "w+", stdout);
    freopen_s(&pCerr, "CONOUT$", "w+", stderr);
    #endif

    nkentseu::PlatformState.Init(hInst, hPrev, szCmdLine, sw);
    nkentseu::int32 result = Main(nkentseu::PlatformState.argv);

    #ifdef NKENTSEU_DEBUG
    FreeConsole();
    if (pCout != 0) fclose(pCout);
    if (pCerr != 0) fclose(pCerr);
    #endif

    return (result);
}

#endif

#endif  //  __NKENTSEU_WINDOW_MAIN_H__
//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 10:00:48 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "PlatformState.h"

namespace nkentseu {

#if defined(NKENTSEU_PLATFORM_WINDOWS)
    void PlatformState_::Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow) {
        MSG msg;    // message
        int argc = 0;

        msg.wParam = 0;
        LPWSTR* commandLineArgs = CommandLineToArgvW(GetCommandLineW(), &argc);

        if (NULL == commandLineArgs) {
            argc = 0;
        }

        if (argc > 0) {
            for (int iii = 0; iii < argc; iii++) {
                size_t wideCharLen = wcslen(commandLineArgs[iii]);
                size_t numConverted = 0;

                char* argv = (char*)malloc(sizeof(char) * (wideCharLen + 1));
                if (argv != NULL) {
                    wcstombs_s(&numConverted, argv, wideCharLen + 1, commandLineArgs[iii], wideCharLen + 1);
                    this->argv.push_back(argv);
                    free(argv);
                }
            }
        }

        HInstance = (hInstance);
        HPrevInstance = (hPrevInstance);
        LPCmdLine = (lpCmdLine);
        NCmdShow = (nCmdShow);
    }

#elif defined(NKENTSEU_PLATFORM_ANDROID)

    android_app* App;

    void PlatformState_::Init(android_app* app) {
        App = app;
    }

#else

    void PlatformState_::Init(int argc, const char** argv) {
        if (argc == 0 || argv == 0) {
            return;
        }
        else {
            int32 index = 0;
            for (index = 0; index < argc; index++) {
                this->argv.push_back(argv[index]);
            }
        }
    }

#endif

    PlatformState_::PlatformState_() {}

}    // namespace nkentseu
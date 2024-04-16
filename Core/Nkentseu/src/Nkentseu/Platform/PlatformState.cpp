//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 10:00:48 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "PlatformState.h"

#include "Nkentseu/Core/NkentseuLogger.h"

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

#elif defined(NKENTSEU_PLATFORM_LINUX)
    /*void PlatformState_::Init(int argc, const char** argv) {
        connection = xcb_connect(NULL, &screenNumber);

        Assert_nts.ATrue(xcb_connection_has_error(connection) != 0, "Erreur de connexion au serveur x");

        Log_nts.Trace("Connexion etablit avec le serveur X");
    }*/

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

        // Assurez-vous que la variable d'environnement DISPLAY est définie pour pointer vers votre serveur X
        if (getenv("DISPLAY") == nullptr) {
            // Affichez un message d'erreur et quittez ou configurez DISPLAY pour qu'il pointe vers votre serveur X
            Log_nts.Error("la variable d'environnement DISPLAY n'est pas définie.");
            exit(EXIT_FAILURE);
        }

        // Connectez-vous au serveur X en utilisant la variable d'environnement DISPLAY
        connection = xcb_connect(getenv("DISPLAY"), &screenNumber);

        // Vérifiez si la connexion au serveur X a réussi
        if (xcb_connection_has_error(connection)) {
            // Affichez un message d'erreur et quittez si la connexion a échoué
            Log_nts.Error("Erreur de connexion au serveur X.");
            exit(EXIT_FAILURE);
        }

        // Affichez un message de confirmation de la connexion réussie au serveur X
        Log_nts.Trace("Connexion établie avec le serveur X.");
    }

    void PlatformState_::Close() {
        xcb_disconnect(connection);
        Log_nts.Trace("Connexion terminer avec le serveur X");
    }

#elif defined(NKENTSEU_PLATFORM_ANDROID)

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
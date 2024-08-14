//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:13:20 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_GENERIC_INPUT_H__
#define __NKENTSEU_GENERIC_INPUT_H__

#pragma once

#include "NTSCore/System.h"


namespace nkentseu {
    class NKENTSEU_API GenericInput
    {
    public:

        using Code = uint64;
        using Button = uint64;
        using Axis = uint64;
        using Hat = uint64;

        struct Buttons {
            enum : Code {
                NotDefine = 0,
                B0 = 1,
                B1,
                B2,
                B3,
                B4,
                B5,
                B6,
                B7,
                B8,
                B9,
                B10,
                B11,
                B12,
                B13,
                B14,
                B15,
                B16,
                B17,
                B18,
                B19,
                B20,
                B21,
                B22,
                B23,
                B24,
                B25,
                B26,
                B27,
                B28,
                B29,
                B30,
                B31,
                B32,
                B33,
                B34,
                B35,
                B36,
                B37,
                B38,
                B39,
                B40,
                B41,
                B42,
                B43,
                B44,
                B45,
                B46,
                B47,
                B48,
                B49,
                B50,
                B51,
                B52,
                B53,
                B54,
                B55,
                B56,
                B57,
                B58,
                B59,
                B60,
                B61,
                B62,
                B63,
                B64,
                B65,
                B66,
                B67,
                B68,
                B69,
                B70,
                B71,
                B72,
                B73,
                B74,
                B75,
                B76,
                B77,
                B78,
                B79,
                B80,
                B81,
                B82,
                B83,
                B84,
                B85,
                B86,
                B87,
                B88,
                B89,
                B90,
                B91,
                B92,
                B93,
                B94,
                B95,
                B96,
                B97,
                B98,
                B99,
                B100,
                B101,
                B102,
                B103,
                B104,
                B105,
                B106,
                B107,
                B108,
                B109,
                B110,
                B111,
                B112,
                B113,
                B114,
                B115,
                B116,
                B117,
                B118,
                B119,
                B120,
                B121,
                B122,
                B123,
                B124,
                B125,
                B126,
                B127,
                B128,
                BMax
            };
        };

        struct Axes {
            enum : Code {
                NotDefine = 0,
                A0 = 1,
                A1,
                A2,
                A3,
                A4,
                A5,
                A6,
                A7,
                A8,
                A9,
                A10,
                A11,
                A12,
                A13,
                A14,
                A15,
                A16,
                A17,
                A18,
                A19,
                A20,
                A21,
                A22,
                A23,
                A24,
                A25,
                A26,
                A27,
                A28,
                A29,
                A30,
                A31,
                A32,
                A33,
                A34,
                A35,
                A36,
                A37,
                A38,
                A39,
                A40,
                A41,
                A42,
                A43,
                A44,
                A45,
                A46,
                A47,
                A48,
                A49,
                A50,
                A51,
                A52,
                A53,
                A54,
                A55,
                A56,
                A57,
                A58,
                A59,
                A60,
                A61,
                A62,
                A63,
                A64,
                A65,
                A66,
                A67,
                A68,
                A69,
                A70,
                A71,
                A72,
                A73,
                A74,
                A75,
                A76,
                A77,
                A78,
                A79,
                AMax
            };
        };

        
        struct Hats {
            enum : Code {
                NotDefine = 0,
                Left = 1,
                Right,
                Up,
                Down,
                Max
            };
        };

        static std::string GetButton(Button button);
        static Button GetButton(const std::string& button);
        static std::string GetAxis(Axis axis);
        static Axis GetAxis(const std::string& axis);
        static std::string GetHat(Hat axis);
        static Hat GetHat(const std::string& axis);

        static bool IsBoutton(Code code) {
            return GetButton(code) != "NotDefine";
        }

        static bool IsAxis(Code code) {
            return GetAxis(code) != "NotDefine";
        }

        static bool IsHate(Code code) {
            return GetHat(code) != "NotDefine";
        }

        static constexpr uint64 Count{ 8 };
        static constexpr uint64 ButtonCount{ GenericInput::Buttons::BMax };
        static constexpr uint64 AxisCount{ GenericInput::Axes::AMax };
    };

    #define GENERIC_INPUT(input) GenericInput::input
} // namespace nkentseu

#endif    // __NKENTSEU_GENERICINPUT_H__
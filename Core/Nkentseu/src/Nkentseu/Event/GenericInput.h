//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:13:20 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_GENERICINPUT_H__
#define __NKENTSEU_GENERICINPUT_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>

namespace nkentseu {
    class NKENTSEU_API GenericInput
    {
    public:
        using GICode = int64;
        using Button = int64;
        using Axis = int64;
        using Hat = int64;

        enum : GICode {
            InvalidButton = -1,

            Button0_ev,
            Button1_ev,
            Button2_ev,
            Button3_ev,
            Button4_ev,
            Button5_ev,
            Button6_ev,
            Button7_ev,
            Button8_ev,
            Button9_ev,
            Button10_ev,
            Button11_ev,
            Button12_ev,
            Button13_ev,
            Button14_ev,
            Button15_ev,
            Button16_ev,
            Button17_ev,
            Button18_ev,
            Button19_ev,
            Button20_ev,
            Button21_ev,
            Button22_ev,
            Button23_ev,
            Button24_ev,
            Button25_ev,
            Button26_ev,
            Button27_ev,
            Button28_ev,
            Button29_ev,
            Button30_ev,
            Button31_ev,
            Button32_ev,
            Button33_ev,
            Button34_ev,
            Button35_ev,
            Button36_ev,
            Button37_ev,
            Button38_ev,
            Button39_ev,
            Button40_ev,
            Button41_ev,
            Button42_ev,
            Button43_ev,
            Button44_ev,
            Button45_ev,
            Button46_ev,
            Button47_ev,
            Button48_ev,
            Button49_ev,
            Button50_ev,
            Button51_ev,
            Button52_ev,
            Button53_ev,
            Button54_ev,
            Button55_ev,
            Button56_ev,
            Button57_ev,
            Button58_ev,
            Button59_ev,
            Button60_ev,
            Button61_ev,
            Button62_ev,
            Button63_ev,
            Button64_ev,
            Button65_ev,
            Button66_ev,
            Button67_ev,
            Button68_ev,
            Button69_ev,
            Button70_ev,
            Button71_ev,
            Button72_ev,
            Button73_ev,
            Button74_ev,
            Button75_ev,
            Button76_ev,
            Button77_ev,
            Button78_ev,
            Button79_ev,
            Button80_ev,
            Button81_ev,
            Button82_ev,
            Button83_ev,
            Button84_ev,
            Button85_ev,
            Button86_ev,
            Button87_ev,
            Button88_ev,
            Button89_ev,
            Button90_ev,
            Button91_ev,
            Button92_ev,
            Button93_ev,
            Button94_ev,
            Button95_ev,
            Button96_ev,
            Button97_ev,
            Button98_ev,
            Button99_ev,
            Button100_ev,
            Button101_ev,
            Button102_ev,
            Button103_ev,
            Button104_ev,
            Button105_ev,
            Button106_ev,
            Button107_ev,
            Button108_ev,
            Button109_ev,
            Button110_ev,
            Button111_ev,
            Button112_ev,
            Button113_ev,
            Button114_ev,
            Button115_ev,
            Button116_ev,
            Button117_ev,
            Button118_ev,
            Button119_ev,
            Button120_ev,
            Button121_ev,
            Button122_ev,
            Button123_ev,
            Button124_ev,
            Button125_ev,
            Button126_ev,
            Button127_ev,
            Button128_ev,
            ButtonMAX_ev
        };

        enum : GICode {
            InvalidAxis_ev = -1,
            Axis0_ev,
            Axis1_ev,
            Axis2_ev,
            Axis3_ev,
            Axis4_ev,
            Axis5_ev,
            Axis6_ev,
            Axis7_ev,
            Axis8_ev,
            Axis9_ev,
            Axis10_ev,
            Axis11_ev,
            Axis12_ev,
            Axis13_ev,
            Axis14_ev,
            Axis15_ev,
            Axis16_ev,
            Axis17_ev,
            Axis18_ev,
            Axis19_ev,
            Axis20_ev,
            Axis21_ev,
            Axis22_ev,
            Axis23_ev,
            Axis24_ev,
            Axis25_ev,
            Axis26_ev,
            Axis27_ev,
            Axis28_ev,
            Axis29_ev,
            Axis30_ev,
            Axis31_ev,
            Axis32_ev,
            Axis33_ev,
            Axis34_ev,
            Axis35_ev,
            Axis36_ev,
            Axis37_ev,
            Axis38_ev,
            Axis39_ev,
            Axis40_ev,
            Axis41_ev,
            Axis42_ev,
            Axis43_ev,
            Axis44_ev,
            Axis45_ev,
            Axis46_ev,
            Axis47_ev,
            Axis48_ev,
            Axis49_ev,
            Axis50_ev,
            Axis51_ev,
            Axis52_ev,
            Axis53_ev,
            Axis54_ev,
            Axis55_ev,
            Axis56_ev,
            Axis57_ev,
            Axis58_ev,
            Axis59_ev,
            Axis60_ev,
            Axis61_ev,
            Axis62_ev,
            Axis63_ev,
            Axis64_ev,
            Axis65_ev,
            Axis66_ev,
            Axis67_ev,
            Axis68_ev,
            Axis69_ev,
            Axis70_ev,
            Axis71_ev,
            Axis72_ev,
            Axis73_ev,
            Axis74_ev,
            Axis75_ev,
            Axis76_ev,
            Axis77_ev,
            Axis78_ev,
            Axis79_ev,
            AxisMax_ev
        };

        enum : GICode {
            InvalidHat_ev = -1,
            Hat_Left_ev,
            Hat_Right_ev,
            Hat_Up_ev,
            Hat_Down_ev,
            Max_Hat_ev = Hat_Down_ev + 1
        };

        static std::string GetButton(Button button);
        static Button GetButton(const std::string& button);
        static std::string GetAxis(Axis axis);
        static Axis GetAxis(const std::string& axis);
        static std::string GetHat(Hat axis);
        static Hat GetHat(const std::string& axis);

        static bool IsBoutton(GICode code) {
            return GetButton(code) != "InvalidButton";
        }

        static bool IsAxis(GICode code) {
            return GetAxis(code) != "InvalidAxis";
        }

        static bool IsHate(GICode code) {
            return GetHat(code) != "InvalidHat";
        }
    };

    #define GENERIC_INPUT(input) GenericInput::input##_ev
} // namespace nkentseu

#endif    // __NKENTSEU_GENERICINPUT_H__
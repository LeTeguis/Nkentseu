//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/19/2024 at 11:10:08 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_GAMEPAD_H__
#define __NKENTSEU_GAMEPAD_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>

namespace nkentseu {
    class NKENTSEU_API Gamepad
    {
        public:
			enum class Type {
				NotDefine, XBox, DualShock
			};

			enum class DataType {
				Button, Axis, NoneDefine
			};

			using Code = uint32;
			using Button = uint32;
			using Axis = uint32;
			using Hat = uint32;

			// Playstation
			struct DualShock {
				struct Buttons {
					enum : Button {
						NotDefine = 0,

						Triangle,
						Circle,
						Cross,
						Square,

						L1,
						R1,

						Select,
						Start,

						L2,
						R2,

						DpadUp,
						DpadRight,
						DpadDown,
						DpadLeft,

						LeftStick,
						RightStick,

						ButtonMax
					};
				};

				struct Axes {
					enum : Axis {
						NotDefine = 0,

						LeftStickLeft,
						LeftStickRight,
						LeftStickUp,
						LeftStickDown,
						RightStickLeft,
						RightStickRight,
						RightStickUp,
						RightStickDown,

						AxisMax
					};
				};

				static std::string ButtonNames(Button code);
				static std::string AxisNames(Axis axis);
			};

			// Playstation
			struct XBox {
				struct Buttons {
					enum : Button {
						NotDefine = 0,

						A = 1,
						B,
						X,
						Y,

						DpadLeft,
						DpadRight,
						DpadUp,
						DpadDown,

						LeftBumper,
						RightBumper,
						LeftStickButton,
						RightStickButton,

						Back,
						Start,

						LeftTrigger,
						RightTrigger
					};
				};

				struct Axes {
					enum : Axis {
						NotDefine = 0,

						LeftStickLeft,
						LeftStickRight,
						LeftStickUp,
						LeftStickDown,
						RightStickLeft,
						RightStickRight,
						RightStickUp,
						RightStickDown,

						AxisMax
					};
				};

				static std::string ButtonNames(Button code);
				static std::string AxisNames(Axis axis);
			};

			static std::string ToString(Code code, Type type, DataType dataType);
			static std::string ToString(Type code);
    };
} // namespace nkentseu

#endif    // __NKENTSEU_GAMEPAD_H__
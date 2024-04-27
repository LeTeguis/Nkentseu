//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/19/2024 at 11:10:08 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Gamepad.h"

namespace nkentseu {
    std::string Gamepad::DualShock::ButtonNames(Button code) {
		#define BUTTON_STR(code_id) if (code == DualShock::Buttons::code_id) return #code_id

		BUTTON_STR(Start);
		BUTTON_STR(Select);
		BUTTON_STR(Square);
		BUTTON_STR(Cross);
		BUTTON_STR(Circle);
		BUTTON_STR(Triangle);
		BUTTON_STR(L1);
		BUTTON_STR(R1);
		BUTTON_STR(RightStick);
		BUTTON_STR(LeftStick);
		BUTTON_STR(L2);
		BUTTON_STR(R2);
		BUTTON_STR(DpadLeft);
		BUTTON_STR(DpadRight);
		BUTTON_STR(DpadUp);
		BUTTON_STR(DpadDown);

		return "NotDefine";
	}

	std::string Gamepad::DualShock::AxisNames(Axis axis) {
		#define CODE_STR(code_id) if (axis == DualShock::Axes::code_id) return #code_id

		CODE_STR(LeftStickLeft);
		CODE_STR(LeftStickRight);
		CODE_STR(LeftStickUp);
		CODE_STR(LeftStickDown);
		CODE_STR(RightStickLeft);
		CODE_STR(RightStickRight);
		CODE_STR(RightStickUp);
		CODE_STR(RightStickDown);

		return "NotDefine";
	}

	std::string Gamepad::XBox::ButtonNames(Button code)
	{
		#define BUTTON_STR_S(code_id) if (code == XBox::Buttons::code_id) return #code_id

		BUTTON_STR_S(Start);
		BUTTON_STR_S(Back);
		BUTTON_STR_S(A);
		BUTTON_STR_S(B);
		BUTTON_STR_S(X);
		BUTTON_STR_S(Y);
		BUTTON_STR_S(LeftBumper);
		BUTTON_STR_S(RightBumper);
		BUTTON_STR_S(LeftStickButton);
		BUTTON_STR_S(RightStickButton);
		BUTTON_STR_S(LeftTrigger);
		BUTTON_STR_S(RightTrigger);
		BUTTON_STR_S(DpadLeft);
		BUTTON_STR_S(DpadRight);
		BUTTON_STR_S(DpadUp);
		BUTTON_STR_S(DpadDown);

		return "NotDefine";
	}

	std::string Gamepad::XBox::AxisNames(Axis axis)
	{
		#define CODE_STR_S(code_id) if (axis == XBox::Axes::code_id) return #code_id

		CODE_STR_S(LeftStickLeft);
		CODE_STR_S(LeftStickRight);
		CODE_STR_S(LeftStickUp);
		CODE_STR_S(LeftStickDown);
		CODE_STR_S(RightStickLeft);
		CODE_STR_S(RightStickRight);
		CODE_STR_S(RightStickUp);
		CODE_STR_S(RightStickDown);

		return "NotDefine";
	}

	std::string Gamepad::ToString(Code code, Type type, DataType dataType) {
		if (type == Type::DualShock) {
			if (dataType == DataType::Button) {
				return DualShock::ButtonNames(code);
			}
			else if (dataType == DataType::Axis) {
				return DualShock::AxisNames(code);
			}
		}
		else if (type == Type::XBox) {
			if (dataType == DataType::Button) {
				return XBox::ButtonNames(code);
			}
			else if (dataType == DataType::Axis) {
				return XBox::AxisNames(code);
			}
		}
		return "NotDefine";
	}


	std::string Gamepad::ToString(Type code) {
		#define CODE_STR_T(code_id) if (code == Type::code_id) return #code_id

		CODE_STR_T(XBox);
		CODE_STR_T(DualShock);

		return "NotDefine";
	}

}    // namespace nkentseu
//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/17/2024 at 5:18:47 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once
#include "System/System.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#ifndef __NKENTSEU_WINDOW_GENERIC_INPUT_H__
#define __NKENTSEU_WINDOW_GENERIC_INPUT_H__

#pragma once

#include <stdio.h>
#include <Windows.h>
#include <Xinput.h>
#include <Dbt.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <inttypes.h>

#include <System/Nature/Base.h>

namespace nkentseu {
	enum class NKENTSEU_API JoystickType
	{
		JoystickTypeGeneric,
		JoystickTypeXbox,
		JoystickTypeDualshock4
	};

	struct NKENTSEU_API JoystickState
	{
		static const unsigned int inputCount = 64;
		static const unsigned int maxNameLength = 128;

		// Inputs
		bool connected;
		JoystickType type;
		float currentInputs[inputCount];
		float previousInputs[inputCount];
		wchar_t deviceName[maxNameLength];
		char productName[maxNameLength];
		char manufacturerName[maxNameLength];

		// Outputs
		float lightRumble;
		float heavyRumble;
		float ledRed;
		float ledGreen;
		float ledBlue;

		BYTE outputBuffer[96];
		HANDLE outputFile;
		OVERLAPPED overlapped;
	};

	struct NKENTSEU_API Joysticks
	{
		static const unsigned int maxXinputControllers = 4;
		unsigned int count;
		JoystickState* states;
		HWND hwnd;
	};

	enum class NKENTSEU_API XboxInputs : int32
	{
		XboxInputA = 0,
		XboxInputB,
		XboxInputX,
		XboxInputY,
		XboxInputDpadLeft,
		XboxInputDpadRight,
		XboxInputDpadUp,
		XboxInputDpadDown,
		XboxInputLeftBumper,
		XboxInputRightBumper,
		XboxInputLeftStickButton,
		XboxInputRightStickButton,
		XboxInputBack,
		XboxInputStart,
		XboxInputLeftTrigger,
		XboxInputRightTrigger,
		XboxInputLeftStickLeft,
		XboxInputLeftStickRight,
		XboxInputLeftStickUp,
		XboxInputLeftStickDown,
		XboxInputRightStickLeft,
		XboxInputRightStickRight,
		XboxInputRightStickUp,
		XboxInputRightStickDown
	};

	enum class NKENTSEU_API DS4Inputs : int32 {
		DS4InputSquare = 0,
		DS4InputX,
		DS4InputCircle,
		DS4InputTriangle,
		DS4InputL1,
		DS4InputR1,
		DS4InputL3,
		DS4InputR3,
		DS4InputOptions,
		DS4InputShare,
		DS4InputPS,
		DS4InputTouchPadButton,
		DS4InputLeftStickLeft,
		DS4InputLeftStickRight,
		DS4InputLeftStickUp,
		DS4InputLeftStickDown,
		DS4InputRightStickLeft,
		DS4InputRightStickRight,
		DS4InputRightStickUp,
		DS4InputRightStickDown,
		DS4InputL2,
		DS4InputR2,
		DS4InputDpadLeft,
		DS4InputDpadRight,
		DS4InputDpadUp,
		DS4InputDpadDown
	};

	enum class NKENTSEU_API DS3Inputs : int32{
		DS3InputSquare = 0,
		DS3InputX,
		DS3InputCircle,
		DS3InputTriangle,
		DS3InputL1,
		DS3InputR1,
		DS3InputL3,
		DS3InputR3,
		DS3InputOptions,
		DS3InputShare,
		DS3InputPS,
		DS3InputTouchPadButton,
		DS3InputLeftStickLeft,
		DS3InputLeftStickRight,
		DS3InputLeftStickUp,
		DS3InputLeftStickDown,
		DS3InputRightStickLeft,
		DS3InputRightStickRight,
		DS3InputRightStickUp,
		DS3InputRightStickDown,
		DS3InputL2,
		DS3InputR2,
		DS3InputDpadLeft,
		DS3InputDpadRight,
		DS3InputDpadUp,
		DS3InputDpadDown
	};

	enum class NKENTSEU_API GenericInputs : int32 {
		GenericInputButton0 = 0,
		GenericInputButton1,
		GenericInputButton2,
		GenericInputButton3,
		GenericInputButton4,
		GenericInputButton5,
		GenericInputButton6,
		GenericInputButton7,
		GenericInputButton8,
		GenericInputButton9,
		GenericInputButton10,
		GenericInputButton11,
		GenericInputButton12,
		GenericInputButton13,
		GenericInputButton14,
		GenericInputButton15,
		GenericInputButton16,
		GenericInputButton17,
		GenericInputButton18,
		GenericInputButton19,
		GenericInputButton20,
		GenericInputButton21,
		GenericInputButton22,
		GenericInputButton23,
		GenericInputButton24,
		GenericInputButton25,
		GenericInputButton26,
		GenericInputButton27,
		GenericInputButton28,
		GenericInputButton29,
		GenericInputButton30,
		GenericInputButton31,
		GenericInputAxis0Positive,
		GenericInputAxis0Negative,
		GenericInputAxis1Positive,
		GenericInputAxis1Negative,
		GenericInputAxis2Positive,
		GenericInputAxis2Negative,
		GenericInputAxis3Positive,
		GenericInputAxis3Negative,
		GenericInputAxis4Positive,
		GenericInputAxis4Negative,
		GenericInputAxis5Positive,
		GenericInputAxis5Negative,
		GenericInputAxis6Positive,
		GenericInputAxis6Negative,
		GenericInputAxis7Positive,
		GenericInputAxis7Negative,
		GenericInputHatLeft,
		GenericInputHatRight,
		GenericInputHatUp,
		GenericInputHatDown,
	};

    class NKENTSEU_API WindowGenericInput
    {
		const char* genericInputNames[52] = {
	"Controller-Button 0",
	"Controller-Button 1",
	"Controller-Button 2",
	"Controller-Button 3",
	"Controller-Button 4",
	"Controller-Button 5",
	"Controller-Button 6",
	"Controller-Button 7",
	"Controller-Button 8",
	"Controller-Button 9",
	"Controller-Button 10",
	"Controller-Button 11",
	"Controller-Button 12",
	"Controller-Button 13",
	"Controller-Button 14",
	"Controller-Button 15",
	"Controller-Button 16",
	"Controller-Button 17",
	"Controller-Button 18",
	"Controller-Button 19",
	"Controller-Button 20",
	"Controller-Button 21",
	"Controller-Button 22",
	"Controller-Button 23",
	"Controller-Button 24",
	"Controller-Button 25",
	"Controller-Button 26",
	"Controller-Button 27",
	"Controller-Button 28",
	"Controller-Button 29",
	"Controller-Button 30",
	"Controller-Button 31",
	"Controller-Axis 0+",
	"Controller-Axis 0-",
	"Controller-Axis 1+",
	"Controller-Axis 1-",
	"Controller-Axis 2+",
	"Controller-Axis 2-",
	"Controller-Axis 3+",
	"Controller-Axis 3-",
	"Controller-Axis 4+",
	"Controller-Axis 4-",
	"Controller-Axis 5+",
	"Controller-Axis 5-",
	"Controller-Axis 6+",
	"Controller-Axis 6-",
	"Controller-Axis 7+",
	"Controller-Axis 7-",
	"Controller-Hat Left",
	"Controller-Hat Right",
	"Controller-Hat Up",
	"Controller-Hat Down",
		};

		const char* ps4InputNames[26] = {
	"DS4-Square",
	"DS4-X",
	"DS4-Circle",
	"DS4-Triangle",
	"DS4-L1",
	"DS4-R1",
	"DS4-L3",
	"DS4-R3",
	"DS4-Options",
	"DS4-Share",
	"DS4-PlayStation Button",
	"DS4-Touch Pad Button",
	"DS4-Left Stick Left",
	"DS4-Left Stick Right",
	"DS4-Left Stick Up",
	"DS4-Left Stick Down",
	"DS4-Right Stick Left",
	"DS4-Right Stick Right",
	"DS4-Right Stick Up",
	"DS4-Right Stick Down",
	"DS4-L2",
	"DS4-R2",
	"DS4-Dpad Left",
	"DS4-Dpad Right",
	"DS4-Dpad Up",
	"DS4-Dpad Down",
		};

		const char* xboxInputNames[24] = {
			"XBox-A",
			"XBox-B",
			"XBox-X",
			"XBox-Y",
			"XBox-Left",
			"XBox-Right",
			"XBox-Up",
			"XBox-Down",
			"XBox-LB",
			"XBox-RB",
			"XBox-LS",
			"XBox-RS",
			"XBox-Back",
			"XBox-Start",
			"XBox-Left Trigger",
			"XBox-Right Trigger",
			"XBox-Left Stick Left",
			"XBox-Left Stick Right",
			"XBox-Left Stick Up",
			"XBox-Left Stick Down",
			"XBox-Right Stick Left",
			"XBox-Right Stick Right",
			"XBox-Right Stick Up",
			"XBox-Right Stick Down",
		};
        public:

			bool isXboxController(char* deviceName);

			bool isDualshock4(RID_DEVICE_INFO_HID info);

			uint32_t makeReflectedCRC32(BYTE* data, uint32_t byteCount);

			void updateDualshock4(JoystickState* state, BYTE rawData[], DWORD byteCount);

			void parseGenericController(JoystickState* out, BYTE rawData[], DWORD dataSize, _HIDP_PREPARSED_DATA* preparsedData);

			void updateRawInput(Joysticks* joysticks, LPARAM lParam);

			void connectHIDJoystick(Joysticks* joysticks, const WCHAR* deviceName);

			void disconnectHIDJoystick(Joysticks* joysticks, const WCHAR* deviceName);

			void updateConnectionStatus(Joysticks* joysticks, HANDLE device, WPARAM status);

			Joysticks createJoysticks();

			void updateJoysticks(Joysticks* joysticks);

			const char* getInputName(Joysticks joysticks, unsigned int joystickIndex, unsigned int inputIndex);

			static WindowGenericInput& Instance() {
				static WindowGenericInput wgi;
				return wgi;
			}
        private:
			WindowGenericInput() {
			}
    };

#define GInput WindowGenericInput::Instance()
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_GENERIC_INPUT_H__

#endif
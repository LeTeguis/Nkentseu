//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/17/2024 at 5:18:47 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "WindowGenericInput.h"


#ifdef NKENTSEU_PLATFORM_WINDOWS


namespace nkentseu {

	bool WindowGenericInput::isXboxController(char* deviceName)
	{
		return strstr(deviceName, "IG_");
	}

	bool WindowGenericInput::isDualshock4(RID_DEVICE_INFO_HID info)
	{
		const DWORD sonyVendorID = 0x054C;
		const DWORD ds4Gen1ProductID = 0x05C4;
		const DWORD ds4Gen2ProductID = 0x09CC;

		return info.dwVendorId == sonyVendorID && (info.dwProductId == ds4Gen1ProductID || info.dwProductId == ds4Gen2ProductID);
	}

	uint32_t WindowGenericInput::makeReflectedCRC32(BYTE* data, uint32_t byteCount)
	{
		static const uint32_t crcTable[] = {
		0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,
			0x7EB17CBD,0xE7B82D07,0x90BF1D91,0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,
			0xFD62F97A,0x8A65C9EC,0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,
			0xA50AB56B,0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,
			0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,0x76DC4190,
			0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,0x086D3D2D,
			0x91646C97,0xE6635C01,0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,
			0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,
			0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,
			0x206F85B3,0xB966D409,0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,0xEDB88320,0x9ABFB3B6,
			0x03B6E20C,0x74B1D29A,0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,0xE40ECF0B,0x9309FF9D,0x0A00AE27,
			0x7D079EB1,0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,
			0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,0xD80D2BDA,
			0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,
			0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,0x9B64C2B0,0xEC63F226,0x756AA39C,
			0x026D930A,0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,
			0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,
			0xF862AE69,0x616BFFD3,0x166CCF45,0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,0xAED16A4A,0xD9D65ADC,
			0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,0x47B2CF7F,0x30B5FFE9,0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,0xCDD70693,0x54DE5729,
			0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D
		};
		uint32_t crc = ~0;
		while (byteCount--) {
			crc = (crc >> 8) ^ crcTable[(uint8_t)crc ^ *data];
			++data;
		}
		return ~crc;
	}

	Joysticks WindowGenericInput::createJoysticks()
	{
		Joysticks joysticks = { 0 };

		// Register devices
		RAWINPUTDEVICE deviceList[2];
		deviceList[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		deviceList[0].usUsage = HID_USAGE_GENERIC_GAMEPAD;
		deviceList[0].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
		deviceList[0].hwndTarget = joysticks.hwnd;
		deviceList[1] = deviceList[0];
		deviceList[1].usUsage = HID_USAGE_GENERIC_JOYSTICK;
		RegisterRawInputDevices(deviceList, sizeof(deviceList) / sizeof(*deviceList), sizeof(RAWINPUTDEVICE));

		// Allocate xbox controllers
		joysticks.count = Joysticks::maxXinputControllers;
		joysticks.states = (JoystickState*)calloc(joysticks.count, sizeof(JoystickState));
		for (UINT i = 0; i < Joysticks::maxXinputControllers; ++i) joysticks.states[i].type = JoystickType::JoystickTypeXbox;

		return joysticks;
	}

	void WindowGenericInput::updateJoysticks(Joysticks* joysticks)
	{
		// if (joysticks == nullptr) return;

		for (unsigned int i = 0; i < joysticks->count; ++i) {
			memcpy(joysticks->states[i].previousInputs, joysticks->states[i].currentInputs, sizeof(joysticks->states[i].previousInputs));
		}

		// HID controllers
		SetPropA(joysticks->hwnd, "userData", joysticks);
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Xbox controllers
		for (unsigned int playerIndex = 0; playerIndex < Joysticks::maxXinputControllers; ++playerIndex)
		{
			JoystickState* state = &joysticks->states[playerIndex];
			XINPUT_STATE xinput;
			if (state->connected && XInputGetState(playerIndex, &xinput) == ERROR_SUCCESS) {
				state->currentInputs[(int32)XboxInputs::XboxInputA] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputB] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputX] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputY] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputDpadLeft] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputDpadRight] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputDpadUp] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputDpadDown] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputLeftBumper] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputRightBumper] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputLeftStickButton] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputRightStickButton] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputBack] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputStart] = (xinput.Gamepad.wButtons & XINPUT_GAMEPAD_START) ? 1.0f : 0.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputLeftTrigger] = xinput.Gamepad.bLeftTrigger / 255.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputRightTrigger] = xinput.Gamepad.bRightTrigger / 255.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputLeftStickLeft] = -xinput.Gamepad.sThumbLX / 32767.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputLeftStickRight] = xinput.Gamepad.sThumbLX / 32767.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputLeftStickUp] = xinput.Gamepad.sThumbLY / 32767.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputLeftStickDown] = -xinput.Gamepad.sThumbLY / 32767.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputRightStickLeft] = -xinput.Gamepad.sThumbRX / 32767.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputRightStickRight] = xinput.Gamepad.sThumbRX / 32767.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputRightStickUp] = xinput.Gamepad.sThumbRY / 32767.0f;
				state->currentInputs[(int32)XboxInputs::XboxInputRightStickDown] = -xinput.Gamepad.sThumbRY / 32767.0f;

				XINPUT_VIBRATION vibration;
				vibration.wLeftMotorSpeed = (WORD)(state->lightRumble * 0xFFFF);
				vibration.wRightMotorSpeed = (WORD)(state->heavyRumble * 0xFFFF);
				XInputSetState(playerIndex, &vibration);
			}
			else {
				state->connected = false;
			}
		}

		// Clamp negative inputs to 0
		for (unsigned int joystickIndex = 0; joystickIndex < joysticks->count; ++joystickIndex) {
			for (unsigned int inputIndex = 0; inputIndex < JoystickState::inputCount; ++inputIndex) {
				float* input = &joysticks->states[joystickIndex].currentInputs[inputIndex];
				if (*input < 0) *input = 0;
			}
		}
	}

	void WindowGenericInput::updateDualshock4(JoystickState* state, BYTE rawData[], DWORD byteCount)
	{
		//if (state == nullptr) return;

		const DWORD usbInputByteCount = 64;
		const DWORD bluetoothInputByteCount = 547;

		unsigned int offset = 0;
		if (byteCount == bluetoothInputByteCount) {
			offset = 2;
		}
		else if (byteCount != usbInputByteCount) {
			return;
		}

		unsigned char leftStickX = *(rawData + offset + 1);
		unsigned char leftStickY = *(rawData + offset + 2);
		unsigned char rightStickX = *(rawData + offset + 3);
		unsigned char rightStickY = *(rawData + offset + 4);
		unsigned char leftTrigger = *(rawData + offset + 8);
		unsigned char rightTrigger = *(rawData + offset + 9);
		unsigned char dpad = 0b1111 & *(rawData + offset + 5);

		state->currentInputs[(int32)DS4Inputs::DS4InputSquare] = (float)(1 & (*(rawData + offset + 5) >> 4));
		state->currentInputs[(int32)DS4Inputs::DS4InputX] = (float)(1 & (*(rawData + offset + 5) >> 5));
		state->currentInputs[(int32)DS4Inputs::DS4InputCircle] = (float)(1 & (*(rawData + offset + 5) >> 6));
		state->currentInputs[(int32)DS4Inputs::DS4InputTriangle] = (float)(1 & (*(rawData + offset + 5) >> 7));
		state->currentInputs[(int32)DS4Inputs::DS4InputL1] = (float)(1 & (*(rawData + offset + 6) >> 0));
		state->currentInputs[(int32)DS4Inputs::DS4InputR1] = (float)(1 & (*(rawData + offset + 6) >> 1));
		state->currentInputs[(int32)DS4Inputs::DS4InputShare] = (float)(1 & (*(rawData + offset + 6) >> 4));
		state->currentInputs[(int32)DS4Inputs::DS4InputOptions] = (float)(1 & (*(rawData + offset + 6) >> 5));
		state->currentInputs[(int32)DS4Inputs::DS4InputL3] = (float)(1 & (*(rawData + offset + 6) >> 6));
		state->currentInputs[(int32)DS4Inputs::DS4InputR3] = (float)(1 & (*(rawData + offset + 6) >> 7));
		state->currentInputs[(int32)DS4Inputs::DS4InputPS] = (float)(1 & (*(rawData + offset + 7) >> 0));
		state->currentInputs[(int32)DS4Inputs::DS4InputTouchPadButton] = (float)(1 & (*(rawData + offset + 7) >> 1));
		state->currentInputs[(int32)DS4Inputs::DS4InputLeftStickLeft] = -(leftStickX / 255.0f * 2 - 1);
		state->currentInputs[(int32)DS4Inputs::DS4InputLeftStickRight] = (leftStickX / 255.0f * 2 - 1);
		state->currentInputs[(int32)DS4Inputs::DS4InputLeftStickUp] = -(leftStickY / 255.0f * 2 - 1);
		state->currentInputs[(int32)DS4Inputs::DS4InputLeftStickDown] = (leftStickY / 255.0f * 2 - 1);
		state->currentInputs[(int32)DS4Inputs::DS4InputRightStickLeft] = -(rightStickX / 255.0f * 2 - 1);
		state->currentInputs[(int32)DS4Inputs::DS4InputRightStickRight] = (rightStickX / 255.0f * 2 - 1);
		state->currentInputs[(int32)DS4Inputs::DS4InputRightStickUp] = -(rightStickY / 255.0f * 2 - 1);
		state->currentInputs[(int32)DS4Inputs::DS4InputRightStickDown] = (rightStickY / 255.0f * 2 - 1);
		state->currentInputs[(int32)DS4Inputs::DS4InputL2] = leftTrigger / 255.0f;
		state->currentInputs[(int32)DS4Inputs::DS4InputR2] = rightTrigger / 255.0f;
		state->currentInputs[(int32)DS4Inputs::DS4InputDpadUp] = (dpad == 0 || dpad == 1 || dpad == 7) ? 1.0f : 0.0f;
		state->currentInputs[(int32)DS4Inputs::DS4InputDpadRight] = (dpad == 1 || dpad == 2 || dpad == 3) ? 1.0f : 0.0f;
		state->currentInputs[(int32)DS4Inputs::DS4InputDpadDown] = (dpad == 3 || dpad == 4 || dpad == 5) ? 1.0f : 0.0f;
		state->currentInputs[(int32)DS4Inputs::DS4InputDpadLeft] = (dpad == 5 || dpad == 6 || dpad == 7) ? 1.0f : 0.0f;
		state->type = JoystickType::JoystickTypeDualshock4;

		int headerSize = 0;
		int outputByteCount = 0;
		if (byteCount == usbInputByteCount) {
			outputByteCount = 32;
			state->outputBuffer[0] = 0x05;
			state->outputBuffer[1] = 0xFF;
		}
		if (byteCount == bluetoothInputByteCount) {
			outputByteCount = 78;
			state->outputBuffer[0] = 0x11;
			state->outputBuffer[1] = 0XC0;
			state->outputBuffer[3] = 0x07;
			headerSize = 1;
		}
		state->outputBuffer[4 + offset + headerSize] = (BYTE)(state->lightRumble * 0xFF);
		state->outputBuffer[5 + offset + headerSize] = (BYTE)(state->heavyRumble * 0xFF);
		state->outputBuffer[6 + offset + headerSize] = (BYTE)(state->ledRed * 0xFF);
		state->outputBuffer[7 + offset + headerSize] = (BYTE)(state->ledGreen * 0xFF);
		state->outputBuffer[8 + offset + headerSize] = (BYTE)(state->ledBlue * 0xFF);

		if (byteCount == bluetoothInputByteCount) {
			uint32_t crc = makeReflectedCRC32(state->outputBuffer, 75);
			memcpy(state->outputBuffer + 75, &crc, sizeof(crc));
		}

		DWORD bytesTransferred;
		if (GetOverlappedResult(state->outputFile, &state->overlapped, &bytesTransferred, false)) {
			if (state->outputFile != INVALID_HANDLE_VALUE) {
				WriteFile(state->outputFile, (void*)(state->outputBuffer + headerSize), outputByteCount, 0, &state->overlapped);
			}
		}
	}

	void WindowGenericInput::parseGenericController(JoystickState* out, BYTE rawData[], DWORD dataSize, _HIDP_PREPARSED_DATA* preparsedData)
	{
		// if (out == nullptr) return;

		memset(out->currentInputs, 0, sizeof(out->currentInputs));

		HIDP_CAPS caps;
		HidP_GetCaps(preparsedData, &caps);

		HIDP_VALUE_CAPS* valueCaps = (HIDP_VALUE_CAPS*)malloc(caps.NumberInputValueCaps * sizeof(HIDP_VALUE_CAPS));
		HidP_GetValueCaps(HidP_Input, valueCaps, &caps.NumberInputValueCaps, preparsedData);
		for (unsigned int i = 0; i < caps.NumberInputValueCaps; ++i)
		{
			ULONG value;
			NTSTATUS status = HidP_GetUsageValue(HidP_Input, valueCaps[i].UsagePage, 0, valueCaps[i].Range.UsageMin, &value, preparsedData, (PCHAR)rawData, dataSize);
			float maxValue = (float)(1 << (valueCaps[i].BitSize)) - 1;
			float normalizedValue = (value / maxValue) * 2 - 1;
			unsigned int usage = valueCaps[i].Range.UsageMin;
			if (usage >= 0x30 && usage <= 0x37) {
				int axisIndex = usage - 0x30;
				out->currentInputs[(int32)GenericInputs::GenericInputAxis0Positive + 2 * axisIndex] = normalizedValue;
				out->currentInputs[(int32)GenericInputs::GenericInputAxis0Negative + 2 * axisIndex] = -normalizedValue;
			}
			if (usage == 0x39) {
				LONG hat = value - valueCaps[i].LogicalMin;
				out->currentInputs[(int32)GenericInputs::GenericInputHatUp] = (hat == 0 || hat == 1 || hat == 7) ? 1.0f : 0.1f;
				out->currentInputs[(int32)GenericInputs::GenericInputHatRight] = (hat == 1 || hat == 2 || hat == 3) ? 1.0f : 0.1f;
				out->currentInputs[(int32)GenericInputs::GenericInputHatDown] = (hat == 3 || hat == 4 || hat == 5) ? 1.0f : 0.1f;
				out->currentInputs[(int32)GenericInputs::GenericInputHatLeft] = (hat == 5 || hat == 6 || hat == 7) ? 1.0f : 0.1f;
			}
		}
		free(valueCaps);

		HIDP_BUTTON_CAPS* buttonCaps = (HIDP_BUTTON_CAPS*)malloc(caps.NumberInputButtonCaps * sizeof(HIDP_BUTTON_CAPS));
		HidP_GetButtonCaps(HidP_Input, buttonCaps, &caps.NumberInputButtonCaps, preparsedData);
		for (unsigned int i = 0; i < caps.NumberInputButtonCaps; ++i)
		{
			unsigned int buttonCount = buttonCaps->Range.UsageMax - buttonCaps->Range.UsageMin + 1;
			USAGE* usages = (USAGE*)malloc(sizeof(USAGE) * buttonCount);
			HidP_GetUsages(HidP_Input, buttonCaps[i].UsagePage, 0, usages, (PULONG)&buttonCount, preparsedData, (PCHAR)rawData, dataSize);
			for (unsigned int usagesIndex = 0; usagesIndex < buttonCount; ++usagesIndex) {
				unsigned int buttonIndex = usages[usagesIndex] - 1;
				if (buttonIndex < 32) out->currentInputs[(int32)GenericInputs::GenericInputButton0 + buttonIndex] = 1.0f;
			}
			free(usages);
		}
		out->type = JoystickType::JoystickTypeGeneric;
	}

	const char* WindowGenericInput::getInputName(Joysticks joysticks, unsigned int joystickIndex, unsigned int inputIndex)
	{
		switch (joysticks.states[joystickIndex].type)
		{
		case JoystickType::JoystickTypeGeneric: return genericInputNames[inputIndex];
		case JoystickType::JoystickTypeDualshock4: return ps4InputNames[inputIndex];
		case JoystickType::JoystickTypeXbox: return xboxInputNames[inputIndex];
		default: return 0;
		}
	}

	void WindowGenericInput::updateRawInput(Joysticks* joysticks, LPARAM lParam)
	{
		// if (joysticks == nullptr) return;

		UINT size = 0;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
		RAWINPUT* input = (RAWINPUT*)malloc(size);
		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, input, &size, sizeof(RAWINPUTHEADER)) > 0)
		{
			RID_DEVICE_INFO deviceInfo;
			UINT deviceInfoSize = sizeof(deviceInfo);
			bool gotInfo = GetRawInputDeviceInfo(input->header.hDevice, RIDI_DEVICEINFO, &deviceInfo, &deviceInfoSize) > 0;

			GetRawInputDeviceInfo(input->header.hDevice, RIDI_PREPARSEDDATA, 0, &size);
			_HIDP_PREPARSED_DATA* data = (_HIDP_PREPARSED_DATA*)malloc(size);
			bool gotPreparsedData = GetRawInputDeviceInfo(input->header.hDevice, RIDI_PREPARSEDDATA, data, &size) > 0;

			WCHAR deviceName[1024] = { 0 };
			UINT deviceNameLength = sizeof(deviceName) / sizeof(*deviceName);
			bool gotName = GetRawInputDeviceInfoW(input->header.hDevice, RIDI_DEVICENAME, deviceName, &deviceNameLength) > 0;

			if (gotInfo && gotPreparsedData && gotName)
			{
				for (UINT i = Joysticks::maxXinputControllers; i < joysticks->count; ++i)
				{
					if (wcscmp(deviceName, joysticks->states[i].deviceName) == 0)
					{
						JoystickState* state = &joysticks->states[i];
						if (isDualshock4(deviceInfo.hid)) {
							updateDualshock4(state, input->data.hid.bRawData, input->data.hid.dwSizeHid);
						}
						else {
							parseGenericController(state, input->data.hid.bRawData, input->data.hid.dwSizeHid, data);
						}
					}
				}
			}
			free(data);
		}
		free(input);
	}

	void WindowGenericInput::connectHIDJoystick(Joysticks* joysticks, const WCHAR* deviceName)
	{
		// if (joysticks == nullptr) return;

		unsigned int joystickIndex = Joysticks::maxXinputControllers;
		while (joystickIndex < joysticks->count && wcscmp(deviceName, joysticks->states[joystickIndex].deviceName) != 0) {
			++joystickIndex;
		}
		if (joystickIndex == joysticks->count) {
			joysticks->count += 1;
			joysticks->states = (JoystickState*)realloc(joysticks->states, joysticks->count * sizeof(JoystickState));
			JoystickState newState = { 0 };
			wcscpy_s(newState.deviceName, deviceName);
			joysticks->states[joystickIndex] = newState;
		}

		JoystickState* state = &joysticks->states[joystickIndex];
		state->outputFile = CreateFileW(deviceName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		HidD_GetProductString(state->outputFile, state->productName, JoystickState::maxNameLength);
		HidD_GetManufacturerString(state->outputFile, state->manufacturerName, JoystickState::maxNameLength);
		joysticks->states[joystickIndex].connected = true;
	}

	void WindowGenericInput::disconnectHIDJoystick(Joysticks* joysticks, const WCHAR* deviceName)
	{
		if (joysticks == nullptr) return;

		for (uint32_t i = Joysticks::maxXinputControllers; i < joysticks->count; ++i)
		{
			if (wcscmp(deviceName, joysticks->states[i].deviceName) != 0)
			{
				JoystickState* state = &joysticks->states[i];
				state->connected = false;
				if (state->outputFile != INVALID_HANDLE_VALUE)
				{
					DWORD bytesTransferred;
					GetOverlappedResult(state->outputFile, &state->overlapped, &bytesTransferred, true);
					CloseHandle(state->outputFile);
				}
			}
		}
	}

	void WindowGenericInput::updateConnectionStatus(Joysticks* joysticks, HANDLE device, WPARAM status)

	{
		// if (joysticks == nullptr) return;

		// Check all XInput devices
		for (unsigned int playerIndex = 0; playerIndex < Joysticks::maxXinputControllers; ++playerIndex) {
			XINPUT_STATE state;
			joysticks->states[playerIndex].connected = (XInputGetState(playerIndex, &state) == ERROR_SUCCESS);
		}

		WCHAR deviceName[1024] = { 0 };
		UINT deviceNameLength = sizeof(deviceName) / sizeof(*deviceName);
		bool gotName = GetRawInputDeviceInfoW(device, RIDI_DEVICENAME, deviceName, &deviceNameLength) > 0;

		if (gotName) {
			if (status == GIDC_ARRIVAL) connectHIDJoystick(joysticks, deviceName);
			else if (status == GIDC_REMOVAL) disconnectHIDJoystick(joysticks, deviceName);
		}
	}

}    // namespace nkentseu

#endif
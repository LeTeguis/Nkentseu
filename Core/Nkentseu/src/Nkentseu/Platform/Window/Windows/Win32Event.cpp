//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-12 at 05:42:10 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Win32Event.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include "Win32ECode.h"
#include "Nkentseu/Event/InputManager.h"
#include "Nkentseu/Core/NkentseuLogger.h"
#include <Logger/Formatter.h>

#include "Win32Window.h"
#include <Nkentseu/Event/WindowEvent.h>

#include <stdio.h>
#include <Dbt.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <windowsx.h>

#pragma comment(lib, "hid.lib")
#pragma comment(lib, "dwmapi.lib")

#define QWORD uint64_t

namespace nkentseu {
	using namespace maths;

//#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#define WC_MAINFRAME	TEXT("MainFrame")
#define MAX_BUTTONS		128
#define CHECK(exp)		{ if(!(exp)) goto Error; }
#define SAFE_FREE(p)	{ if(p) { HeapFree(hHeap, 0, p); (p) = NULL; } }

#define MINX 200
#define MINY 200
#define BORDERWIDTH 5
#define TITLEBARHEIGHT 32
#define TITLEBARZOOMHEIGHT 23

#define MOUSE_BUTTON_IS_PRESSED(v) (raw->data.mouse.ulButtons & (RI_MOUSE_##v##_DOWN))
#define PPU(v, d) (raw->data.mouse.ulButtons & (RI_MOUSE_##v##_UP))
#define PP(v) (raw->data.mouse.ulButtons & (RI_MOUSE_##v##_DOWN | RI_MOUSE_##v##_UP))

	ButtonState::Code bButtonStates_01[MAX_BUTTONS];
	LONG lAxisX_01;
	LONG lAxisY_01;
	LONG lAxisZ_01;
	LONG lAxisRz_01;
	LONG lHat_01;
	INT  g_NumberOfButtons_01;

	Win32Event& Win32Event::UniqueInstance()
	{
		static Win32Event eventManager;
		return eventManager;
	}

	void Win32Event::AddObserver(EventObserver observer)
	{
		if (observer == 0) return;

		auto itr = std::find_if(eventObservers.begin(), eventObservers.end(), [&observer](const EventObserver& c) {
			return &c == &observer;
			});

		if (itr == eventObservers.end()) {
			eventObservers.push_back(observer);
			return;
		}
	}

	void Win32Event::RemoveObserver(const EventObserver& observer)
	{
		if (observer == 0) return;

		auto itr = std::find_if(eventObservers.begin(), eventObservers.end(), [&observer](const EventObserver& c) {
			return &c == &observer;
			});

		if (itr != eventObservers.end()) {
			eventObservers.erase(itr);
			return;
		}
	}

	void Win32Event::Loop()
	{
		isQueueLocked = true;
		Update();
	}

	bool Win32Event::ProcessQueue()
	{
		static bool update_it = true;
		static bool first_time = true;
		static uint32 size_events_list = 0;

		first_time = false;

		if (update_it) {
			first_time = true;
			isQueueLocked = false;
			Update();
			update_it = false;
			isPick = false;

			size_events_list = eventQueue.size();
		}

		if (isPick) {
			Pop();
			isPick = false;
		}

		if (!first_time && size_events_list > 0 && eventQueue.size() >= size_events_list) {
			do {
				Pop();
			} while (!Empty());
		}

		if (Empty()) {
			update_it = true;
			isPick = false;
			return false;
		}

		isPick = true;

		return true;
	}

	Event& Win32Event::Front()
	{
		return *eventQueue.front();
	}

	void Win32Event::Update()
	{
		MSG msg = {};

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Input.private__update__axis();
	}

	void Win32Event::Pop()
	{
		if (eventQueue.empty()) return;

		Event* event = eventQueue.front();
		eventQueue.erase(eventQueue.begin());

		delete event;
	}

	bool Win32Event::Empty()
	{
		return eventQueue.empty();
	}

	usize Win32Event::Size()
	{
		return eventQueue.size();
	}

	LRESULT Win32Event::StaticProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (hwnd == nullptr) return 0;

		Win32Window* window = Win32Window::GetCurrent(hwnd);

		if (window == nullptr) {
			return 0;
		}

		return Win32Event::UniqueInstance().Procedure(msg, wparam, lparam, window);
	}

	LRESULT Win32Event::Procedure(uint32 msg, WPARAM wparam, LPARAM lparam, Win32Window* window)
	{
		if (window == nullptr || window->GetData() == nullptr) return 0;

		MSG message;
		message.hwnd = window->GetData()->windowHandle;
		message.lParam = lparam;
		message.wParam = wparam;
		message.message = msg;
		message.time = 0;

		LRESULT result = PushEvent(message, window);

		// We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
		if (message.message == WM_CLOSE)
			return 0;

		// Don't forward the menu system command, so that pressing ALT or F10 doesn't steal the focus
		if ((message.message == WM_SYSCOMMAND) && (message.wParam == SC_KEYMENU))
			return 0;

		if (result > 0) return result;

		return DefWindowProc(window->GetData()->windowHandle, msg, wparam, lparam);
	}

	LRESULT Win32Event::PushEvent(MSG msg, Win32Window* window)
	{
		if (window->GetData()->windowHandle == nullptr) return 0;

		UINT message = msg.message;
		LRESULT result = 0;
		RECT currentWindowRect = { -1, -1, -1, -1 };
		static bool move = false;

		#define CBTN ((HIWORD(msg.wParam) & XBUTTON1) ? Mouse::Buttons::X1 : Mouse::Buttons::X2)

		switch (message) {
		case WM_SETCURSOR:
		{
			if (LOWORD(msg.lParam) == HTCLIENT) {
				SetCursor(window->GetData()->isCursorVisible ? window->GetData()->lastCursor : nullptr);
			}

			break;
		}
		// input
		case WM_INPUT:
		{ return HandleInputRawEvent(msg, window); break; }
		case WM_DEVICECHANGE:
		case WM_INPUT_DEVICE_CHANGE:
		{ return HandleInputDeviceChangeEvent(msg, window); break; }

		// create event
		case WM_CREATE: { return HandleCreateEvent(msg, window); break; }

		case WM_ACTIVATEAPP: { break; }

						   // window event
		case WM_PAINT: { return HandleWindowPaintEvent(msg, window); break; }
		case WM_ERASEBKGND: { break; }
		case WM_CLOSE: case WM_DESTROY:
		{ return HandleWindowCloseEvent(msg, window); break; }
		case WM_SETFOCUS: case WM_KILLFOCUS:
		{ return HandleWindowFocusedEvent(msg, window, message == WM_SETFOCUS); break; }
		case WM_SIZE:
		case WM_SIZING: { return HandleWindowResizeEvent(msg, window, message == WM_SIZING); break; }
		case WM_MOVE: { return HandleWindowMoveEvent(msg, window, 0); break; }
		case WM_MOVING: { return HandleWindowMoveEvent(msg, window, 1); break; }
		case WM_EXITSIZEMOVE: { return HandleWindowMoveEvent(msg, window, 2); break; }
		case WM_NCHITTEST: { return HandleWindowNCHITTESTEvent(msg, window); break; }
		case WM_DPICHANGED: { return HandleWindowDpiEvent(msg, window); break; }
		case WM_NCCALCSIZE: { return HandleWindowNCCALCSIZEEvent(msg, window); break; }
		case WM_GETMINMAXINFO: { return HandleWindowGETMINMAXINFOEvent(msg, window); break; }

							 //Mouse event
		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL: { return HandleMouseWheelEvent(msg, window, WM_MOUSEWHEEL == message); break; }
		case WM_LBUTTONDBLCLK: case WM_LBUTTONDOWN: case WM_LBUTTONUP:
		{ return HandleMouseButtonEvent(msg, window, Mouse::Buttons::Left, WM_LBUTTONDOWN == message, WM_LBUTTONDBLCLK == message); break; }
		case WM_MBUTTONDBLCLK: case WM_MBUTTONDOWN: case WM_MBUTTONUP:
		{ return HandleMouseButtonEvent(msg, window, Mouse::Buttons::Middle, WM_MBUTTONDOWN == message, WM_MBUTTONDBLCLK == message); break; }
		case WM_RBUTTONDBLCLK: case WM_RBUTTONDOWN: case WM_RBUTTONUP:
		{ return HandleMouseButtonEvent(msg, window, Mouse::Buttons::Right, WM_RBUTTONDOWN == message, WM_RBUTTONDBLCLK == message); break; }
		case WM_XBUTTONDBLCLK: case WM_XBUTTONDOWN: case WM_XBUTTONUP:
		{ return HandleMouseButtonEvent(msg, window, CBTN, WM_XBUTTONDOWN == message, WM_XBUTTONDBLCLK == message); break; }
		case WM_MOUSEMOVE: { return HandleMouseMoveEvent(msg, window); break; }
		case WM_MOUSELEAVE: { return HandleMouseLeaveEvent(msg, window); break; }

						  // keyboard event
		case WM_KEYDOWN: case WM_KEYUP: case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		{ return HandleKeyboardEvent(msg, window, (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)); break; }
		case WM_SYSCHAR: case WM_UNICHAR: case WM_CHAR: case WM_DEADCHAR: case WM_IME_CHAR:
		{ return HandleCharEvent(msg, window, message != WM_SYSCHAR); break; }

		case WM_DROPFILES: { return HandleDropFilesEvent(msg, window); break; }
		case WM_WINDOWPOSCHANGED: return RestricWindowSize(msg, window); break;
		default:
			// Do nothing
			break;
		}
		return 0;
	}

	LRESULT Win32Event::FinalizePushEvent(Event* event, LRESULT info, MSG msg, Win32Window* native, const RECT& currentWindowRect)
	{
		if (event->GetEventType() != EventType::NotDefine) {
			if (!isQueueLocked) {
				eventQueue.push_back(event);
			}

			for (auto& eventCallback : eventObservers) {
				if (eventCallback != nullptr) {
					eventCallback(*event);
				}
			}
		}

		if (!(currentWindowRect.right == currentWindowRect.left &&
			currentWindowRect.right == currentWindowRect.top &&
			currentWindowRect.right == currentWindowRect.bottom &&
			currentWindowRect.right == -1)) {
			RECT* const prcNewWindow = (RECT*)msg.lParam;
			SetWindowPos(native->GetData()->windowHandle, NULL, currentWindowRect.left,
				currentWindowRect.top,
				currentWindowRect.right - currentWindowRect.left,
				currentWindowRect.bottom - currentWindowRect.top,
				SWP_NOZORDER | SWP_NOACTIVATE);
		}

		return info;
	}

	LRESULT Win32Event::HandleInputRawEvent(MSG msg, Win32Window* window)
	{
		LRESULT result = 0;

		UINT size;
		GetRawInputBuffer(0, &size, sizeof(RAWINPUTHEADER));
		size *= 8;
		RAWINPUT* rawInput = (RAWINPUT*)malloc(size);
		UINT inputCount = GetRawInputBuffer(rawInput, &size, sizeof(RAWINPUTHEADER));
		if (inputCount != -1) {
			RAWINPUT* nextInput = rawInput;
			for (UINT i = 0; i < inputCount; ++i) {
				UpdateRawInputData(msg, window, nextInput);
				nextInput = NEXTRAWINPUTBLOCK(nextInput);
			}
		}
		free(rawInput);
		Sleep(16);
		return result;
	}

	void Win32Event::UpdateRawInputData(MSG msg, Win32Window* window, RAWINPUT* input)
	{
		UINT size;
		GetRawInputDeviceInfo(input->header.hDevice, RIDI_PREPARSEDDATA, 0, &size);
		_HIDP_PREPARSED_DATA* data = (_HIDP_PREPARSED_DATA*)malloc(size);
		UINT bytesWritten = GetRawInputDeviceInfo(input->header.hDevice, RIDI_PREPARSEDDATA, data, &size);

		RID_DEVICE_INFO			deviceInfo;
		UINT deviceInfoSize = sizeof(deviceInfo);
		bool gotInfo = GetRawInputDeviceInfo(input->header.hDevice, RIDI_DEVICEINFO, &deviceInfo, &deviceInfoSize) > 0;

		WCHAR deviceName[1024] = { 0 };
		UINT deviceNameLength = sizeof(deviceName) / sizeof(*deviceName);
		bool gotName = GetRawInputDeviceInfoW(input->header.hDevice, RIDI_DEVICENAME, deviceName, &deviceNameLength) > 0;
		std::wstring wname(deviceName);
		std::string name = std::string(wname.begin(), wname.end());

		if (bytesWritten > 0) {
			HIDP_CAPS caps;
			HidP_GetCaps(data, &caps);

			HIDP_VALUE_CAPS* valueCaps = (HIDP_VALUE_CAPS*)malloc(caps.NumberInputValueCaps * sizeof(HIDP_VALUE_CAPS));
			HidP_GetValueCaps(HidP_Input, valueCaps, &caps.NumberInputValueCaps, data);
			for (USHORT i = 0; i < caps.NumberInputValueCaps; ++i) {
				ULONG value;
				HidP_GetUsageValue(HidP_Input, valueCaps[i].UsagePage, 0, valueCaps[i].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);

				float32 value_concret = (float32)((value - 127.0) / 127.0f);
				if (value_concret < -1) value_concret = -1.0f;
				if (value_concret > 1) value_concret = 1.0f;

				AnalyzeAxisRawInput(msg, window, deviceInfo.hid, name, (GenericInput::Axis)valueCaps[i].Range.UsageMin, value_concret);
			}
			free(valueCaps);

			HIDP_BUTTON_CAPS* buttonCaps = (HIDP_BUTTON_CAPS*)malloc(caps.NumberInputButtonCaps * sizeof(HIDP_BUTTON_CAPS));
			HidP_GetButtonCaps(HidP_Input, buttonCaps, &caps.NumberInputButtonCaps, data);
			for (USHORT i = 0; i < caps.NumberInputButtonCaps; ++i) {
				ULONG usageCount = buttonCaps->Range.UsageMax - buttonCaps->Range.UsageMin + 1;
				USAGE* usages = (USAGE*)malloc(sizeof(USAGE) * usageCount);
				HidP_GetUsages(HidP_Input, buttonCaps[i].UsagePage, 0, usages, &usageCount, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);

				for (ULONG usageIndex = 0; usageIndex < usageCount; ++usageIndex) {
					GenericInput::Button button = usages[usageIndex];
					bButtonStates_01[button] = ButtonState::NotDefine;
				}

				free(usages);

				for (GenericInput::Button j = GenericInput::Buttons::B0; j < GenericInput::Buttons::BMax; j++) {
					if (bButtonStates_01[j] == ButtonState::NotDefine) {
						bButtonStates_01[j] = ButtonState::Pressed;
						AnalyzeButtonRawInput(msg, window, deviceInfo.hid, name, j, true);
					}
					else if (bButtonStates_01[j] == ButtonState::Pressed) {
						bButtonStates_01[j] = ButtonState::Released;
						AnalyzeButtonRawInput(msg, window, deviceInfo.hid, name, j, false);
					}
				}
			}
			free(buttonCaps);
		}
		free(data);//*/
	}

	void Win32Event::AnalyzeAxisRawInput(MSG msg, Win32Window* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value)
	{
		if (axis >= GenericInput::Axes::AMax) return;

		//if ((devicecInfoHid.dwProductId == 0x09CC || devicecInfoHid.dwProductId == 0x05C4) && devicecInfoHid.dwVendorId == 0x054C) // PS4

		// ps3
		if (devicecInfoHid.dwProductId == 0x0268 && devicecInfoHid.dwVendorId == 0x054C) {
			SetPS3GamepadAxis(msg, window, devicecInfoHid, name, axis, transmit_value);
		}
		else {
			GenericInputInfos ginfos(devicecInfoHid.dwVendorId, devicecInfoHid.dwProductId, devicecInfoHid.dwVersionNumber, devicecInfoHid.usUsage, devicecInfoHid.usUsagePage, name);
			//FinalizePushEvent(new GenericAxisEvent(window->ID(), ginfos, axis, transmit_value), 0, msg, window);
		}
	}

	void Win32Event::AnalyzeButtonRawInput(MSG msg, Win32Window* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed)
	{
		if (button >= GenericInput::Buttons::BMax) return;

		if (devicecInfoHid.dwProductId == 0x0268 && devicecInfoHid.dwVendorId == 0x054C) {
			SetPS3GamepadButton(msg, window, devicecInfoHid, name, button, isPressed);
		}
		else {
			GenericInputInfos ginfos(devicecInfoHid.dwVendorId, devicecInfoHid.dwProductId, devicecInfoHid.dwVersionNumber, devicecInfoHid.usUsage, devicecInfoHid.usUsagePage, name);
			if (isPressed) {
				//FinalizePushEvent(new GenericInputButtonPressedEvent(window->ID(), ginfos, button), 0, msg, window);
			}
			else {
				//FinalizePushEvent(new GenericInputButtonReleasedEvent(window->ID(), ginfos, button), 0, msg, window);
			}
		}
	}

	void Win32Event::SetPS3GamepadAxis(MSG msg, Win32Window* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value)
	{
	}

	void Win32Event::SetPS3GamepadButton(MSG msg, Win32Window* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed)
	{
	}

	LRESULT Win32Event::HandleInputDeviceChangeEvent(MSG msg, Win32Window* window)
	{
		// Some sort of device change has happened, update joystick connections
		if ((msg.wParam == DBT_DEVICEARRIVAL) || (msg.wParam == DBT_DEVICEREMOVECOMPLETE))
		{
			// Some sort of device change has happened, update joystick connections if it is a device interface
			auto* deviceBroadcastHeader = reinterpret_cast<DEV_BROADCAST_HDR*>(msg.lParam);

			if (deviceBroadcastHeader && (deviceBroadcastHeader->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)) {
				// connection to joystick
				Log_nts.Debug("arrival");
			}
		}
		return 0;
	}

	LRESULT Win32Event::HandleCreateEvent(MSG msg, Win32Window* window) {
		HandleWindowCreateEvent(msg, window);

		RAWINPUTDEVICE deviceList[2] = { 0 };

		deviceList[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		deviceList[0].usUsage = HID_USAGE_GENERIC_JOYSTICK;
		deviceList[0].dwFlags = RIDEV_INPUTSINK;
		deviceList[0].hwndTarget = window->GetData()->windowHandle;

		deviceList[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
		deviceList[1].usUsage = HID_USAGE_GENERIC_GAMEPAD;
		deviceList[1].dwFlags = RIDEV_INPUTSINK;
		deviceList[1].hwndTarget = window->GetData()->windowHandle;

		UINT deviceCount = sizeof(deviceList) / sizeof(*deviceList);
		RegisterRawInputDevices(deviceList, deviceCount, sizeof(RAWINPUTDEVICE));

		return 0;
	}

	LRESULT Win32Event::HandleWindowCreateEvent(MSG msg, Win32Window* window) {
		// Input;
		return FinalizePushEvent(new WindowStatusEvent(window->ID(), WindowState::Created, window->GetProperties()), 0, msg, window);
	}

	LRESULT Win32Event::HandleWindowPaintEvent(MSG msg, Win32Window* window)
	{
		// Call InvalidateRect. This function forces the window to be repainted. InvalidateRect(m_hwnd, NULL, FALSE);

		LRESULT result = 0;

#if defined(NKENTSEU_GRAPHICS_API_SOFTWARE)

		PAINTSTRUCT ps;
		HDC         hDC;
		HBRUSH hOldBrush;
		HBRUSH hBr;
		Color bg;
		RECT rect;
		Rectangle rectangle;

		hDC = BeginPaint(window->GetData()->windowHandle, &ps);
		SetBkMode(hDC, TRANSPARENT);

		{
			//GetWindowRect(window->GetWindowDisplay()->windowHandle, &rect);
			GetClientRect(window->GetData()->windowHandle, &rect);
			rectangle = Rectangle(rect.left, rect.bottom, rect.right - rect.left, rect.bottom - rect.top);

			bg = window->GetBackgroundColor();
			hBr = CreateSolidBrush(RGB(bg.R(), bg.G(), bg.B()));
			hOldBrush = (HBRUSH)SelectObject(hDC, hBr);
		}
		FillRect(ps.hdc, &rect, hBr);

		result = FinalizePushEvent(new WindowRenderedEvent(window->ID(), rectangle, bg), 0, msg, window);

		{
			SelectObject(hDC, hOldBrush);
			DeleteObject(hBr);
		}

		EndPaint(window->GetData()->windowHandle, &ps);

#endif

		return result;
	}

	LRESULT Win32Event::HandleWindowCloseEvent(MSG msg, Win32Window* window)
	{
		// Restore the previous video mode (in case we were running in fullscreen)
		ChangeDisplaySettingsW(nullptr, 0);

		// Unhide the mouse cursor (in case it was hidden)
		window->ShowMouse(true);

		// No longer track the cursor
		SetMouseTracking(false, window);

		// No longer capture the cursor
		ReleaseCapture();
		return FinalizePushEvent(new WindowStatusEvent(window->ID(), WindowState::Closed, window->GetProperties()), 0, msg, window);
	}

	LRESULT Win32Event::HandleWindowFocusedEvent(MSG msg, Win32Window* window, bool focused)
	{
		return FinalizePushEvent(new WindowFocusedEvent(window->ID(), focused ? FocusState::Focused : FocusState::Unfocused), 0, msg, window);
	}

	LRESULT Win32Event::HandleWindowResizeEvent(MSG msg, Win32Window* window, bool resizing)
	{
		LRESULT result = 0;

		float32 area = (float32)(window->GetSize().width * window->GetSize().height);

		Vector2u size;

		if (!resizing) {
			RECT rect;//, frame, border;
			GetClientRect(window->GetData()->windowHandle, &rect);
			size.width = (float32)(rect.right - rect.left);
			size.height = (float32)(rect.bottom - rect.top);
			RedrawWindow(window->GetData()->windowHandle, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);
		}
		else {
			uint32 STEP = 1;
			PRECT rectp = (PRECT)msg.lParam;

			// Get the window and client dimensions
			tagRECT wind, rect;
			GetWindowRect(window->GetData()->windowHandle, &wind);
			GetClientRect(window->GetData()->windowHandle, &rect);

			size.width = (float32)(rect.right - rect.left);
			size.height = (float32)(rect.bottom - rect.top);

			// Redraw window to refresh it while resizing
			RedrawWindow(window->GetData()->windowHandle, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);
			result = WVR_REDRAW;
		}

		//Log_nts.Debug();

		float32 scaleFactor = window->GetDpiScale();
		size.width /= scaleFactor;
		size.height /= scaleFactor;

		float32 newarea = size.width * size.height;
		ResizeState::Code state = ResizeState::NotChange;

		if (newarea > area) {
			state = ResizeState::Expanded;
		}
		else if (newarea < area) {
			state = ResizeState::Reduced;
		}

		if (state == ResizeState::NotChange) {
			return 0;
		}

		window->GetProps().size = size;

		return FinalizePushEvent(new WindowResizedEvent(window->ID(), state, window->GetProps().size), result, msg, window);
	}

	LRESULT Win32Event::HandleWindowMoveEvent(MSG msg, Win32Window* window, uint8 t)
	{
		if (!window->GetProperties().movable) {
			window->SetPosition(window->GetPosition());
			return 0;
		}
		Vector2i lasPosition = window->GetPosition();
		Vector2i position = lasPosition;
		RECT currentWindowRect = { -1, -1, -1, -1 };

		float32 scaleFactor = window->GetDpiScale();

		if (scaleFactor == 0) {
			scaleFactor = 1.0;
		}

		if (t == 0) {
			position = Vector2i(LOWORD(msg.lParam) / scaleFactor, HIWORD(msg.lParam) / scaleFactor);
			window->GetProps().position = position;
			return FinalizePushEvent(new WindowMovedEvent(window->ID(), position, lasPosition), 0, msg, window, currentWindowRect);
		}
		else if (t == 1) {
			RECT* const r = (LPRECT)msg.lParam;
			if (r) {
				currentWindowRect = *r;
				position = Vector2i(r->left / scaleFactor, r->top / scaleFactor);
				window->GetProps().position = position;
				return FinalizePushEvent(new WindowMovedEvent(window->ID(), position, lasPosition), 0, msg, window, currentWindowRect);
			}
		}
		else if (t == 2) {
			RECT* const r = (LPRECT)msg.lParam;
			if (r) {
				currentWindowRect = *r;
				float32 x = (float32)r->left / scaleFactor;
				float32 y = (float32)r->top / scaleFactor;
				position = Vector2i((int32)x, (int32)y);
				window->GetProps().position = position;
				return FinalizePushEvent(new WindowMovedEvent(window->ID(), position, lasPosition), 0, msg, window, currentWindowRect);
			}
		}
		return 0;
	}

	LRESULT Win32Event::HandleWindowNCHITTESTEvent(MSG msg, Win32Window* window)
	{
		LRESULT result = 0;
		RECT rect;
		GetWindowRect(window->GetData()->windowHandle, &rect);
		int x, y, width, height;
		x = static_cast<int>(GET_X_LPARAM(msg.lParam)) - rect.left;
		y = static_cast<int>(GET_Y_LPARAM(msg.lParam)) - rect.top;
		width = static_cast<int>(rect.right - rect.left);
		height = static_cast<int>(rect.bottom - rect.top);
		int topBorder = IsZoomed(window->GetData()->windowHandle) ? 0 : BORDERWIDTH;

		if (y > topBorder && x > 260 && x < (width - 260) && y < 32) {
			return HTCAPTION;
		}
		if (x > BORDERWIDTH && y > BORDERWIDTH && x < width - BORDERWIDTH &&
			y < height - BORDERWIDTH) {
			return HTCLIENT;
		}
		return 0;
	}

	LRESULT Win32Event::HandleWindowBackgroundEraseEvent(MSG msg, Win32Window* window)
	{
		LRESULT result = 0;

#if defined(NKENTSEU_GRAPHICS_API_SOFTWARE)

		PAINTSTRUCT ps;
		BeginPaint(window->GetData()->windowHandle, &ps);
		RECT rect;
		GetWindowRect(window->GetData()->windowHandle, &rect);
		Rectangle r(rect.left, rect.bottom, rect.right - rect.left, rect.bottom - rect.top);
		Color bg = window->GetBackgroundColor();
		HBRUSH BorderBrush = CreateSolidBrush(RGB(bg.R(), bg.G(), bg.B()));
		FillRect(ps.hdc, &rect, BorderBrush);

		result = FinalizePushEvent(new WindowRenderedEvent(window->ID(), r, bg), 0, msg, window);

		EndPaint(window->GetData()->windowHandle, &ps);

#endif

		return result;
	}

	LRESULT Win32Event::HandleWindowDpiEvent(MSG msg, Win32Window* window)
	{
		RECT currentWindowRect = { -1, -1, -1, -1 };
		WORD curDPI = HIWORD(msg.wParam);
		FLOAT fscale = (float)curDPI / USER_DEFAULT_SCREEN_DPI;

		if (!IsZoomed(window->GetData()->windowHandle)) {
			RECT* const prcNewWindow = (RECT*)msg.lParam;
			if (prcNewWindow) {
				currentWindowRect = *prcNewWindow;
			}
		}
		// definir une variable pour avoir le dpi depuis les properties
		return FinalizePushEvent(new WindowDpiChangedEvent(window->ID(), fscale), 0, msg, window, currentWindowRect);
	}

	LRESULT Win32Event::HandleWindowNCCALCSIZEEvent(MSG msg, Win32Window* window)
	{
		if (!window->GetProperties().frame) {
			if (msg.lParam && msg.wParam) {
				NCCALCSIZE_PARAMS* sz = (NCCALCSIZE_PARAMS*)msg.lParam;
				int32 titleHeight = TITLEBARHEIGHT;
				if (IsZoomed(window->GetData()->windowHandle)) {
					titleHeight = TITLEBARZOOMHEIGHT;
				}
				int32 iDpi = GetDpiForWindow(window->GetData()->windowHandle);
				if (iDpi != USER_DEFAULT_SCREEN_DPI) {
					titleHeight = MulDiv(titleHeight, iDpi, USER_DEFAULT_SCREEN_DPI);
				}
				sz->rgrc[0].top += -titleHeight;
			}
		}
		return 0;
	}

	LRESULT Win32Event::HandleWindowGETMINMAXINFOEvent(MSG msg, Win32Window* window)
	{
		MINMAXINFO* min_max = reinterpret_cast<MINMAXINFO*>(msg.lParam);

		RECT rectangleMin = { 0, 0, static_cast<long>(window->GetProperties().minSize.x), static_cast<long>(window->GetProperties().minSize.y) };
		AdjustWindowRect(&rectangleMin, static_cast<DWORD>(GetWindowLongPtr(window->GetData()->windowHandle, GWL_STYLE)), false);
		const auto widthMin = rectangleMin.right - rectangleMin.left;
		const auto heightMin = rectangleMin.bottom - rectangleMin.top;

		RECT rectangleMax = { 0, 0, static_cast<long>(window->GetProperties().maxSize.x), static_cast<long>(window->GetProperties().maxSize.y) };
		AdjustWindowRect(&rectangleMax, static_cast<DWORD>(GetWindowLongPtr(window->GetData()->windowHandle, GWL_STYLE)), false);
		const auto widthMax = rectangleMax.right - rectangleMax.left;
		const auto heightMax = rectangleMax.bottom - rectangleMax.top;

		min_max->ptMinTrackSize.x = widthMin;
		min_max->ptMinTrackSize.y = heightMin;

		min_max->ptMaxSize.x = widthMax;
		min_max->ptMaxSize.y = heightMax;
		return 0;
	}

	LRESULT Win32Event::HandleMouseWheelEvent(MSG msg, Win32Window* window, bool vertical)
	{
		short modifiers = LOWORD(msg.wParam);
		float32 delta = GET_WHEEL_DELTA_WPARAM(msg.wParam) / (float32)WHEEL_DELTA;
		ModifierState ms(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0);

		POINT position;
		position.x = static_cast<std::int16_t>(LOWORD(msg.lParam));
		position.y = static_cast<std::int16_t>(HIWORD(msg.lParam));

		ScreenToClient(window->GetData()->windowHandle, &position);

		Mouse::Wheel wheel = (vertical) ? Mouse::Wheels::Vertical : Mouse::Wheels::Horizontal;

		return FinalizePushEvent(new MouseWheelEvent(window->ID(), wheel, delta, ms, Vector2i(position.x, position.y)), 0, msg, window);
	}

	LRESULT Win32Event::HandleMouseButtonEvent(MSG msg, Win32Window* window, uint8 btn, bool pressed, bool dbclick)
	{
		if (pressed) {
			SetCapture(window->GetData()->windowHandle);
		}
		else {
			ReleaseCapture();
		}

		previousMousePosition = window->GetPosition();

		float32 scaleFactor = window->GetDpiScale();

		short modifiers = LOWORD(msg.wParam);
		ModifierState ms(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0);

		Vector2i positionGlobal(GET_X_LPARAM(msg.lParam) / scaleFactor, GET_Y_LPARAM(msg.lParam) / scaleFactor);

		RECT area;
		GetClientRect(window->GetData()->windowHandle, &area);

		Vector2i position(positionGlobal.x - (area.left / scaleFactor), positionGlobal.y - (area.top / scaleFactor));
		// previousMousePosition = positionGlobal;

		mousePosition = position;
		globalMousePosition = positionGlobal;
		bool doubleClick = false;
		ButtonState::Code state = ButtonState::Released;

		if (dbclick) {
			doubleClick = true;
		}
		if (pressed) {
			state = ButtonState::Pressed;
		}
		return FinalizePushEvent(new MouseInputEvent(window->ID(), state, ms, btn, doubleClick, position, globalMousePosition), 0, msg, window);
	}

	LRESULT Win32Event::HandleMouseButtonRawEvent(MSG msg, Win32Window* window, RAWINPUT* raw)
	{
		short modifiers = LOWORD(msg.wParam);
		ModifierState ms(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0);

		raw->data.mouse.usFlags,
			raw->data.mouse.ulButtons,
			raw->data.mouse.usButtonFlags,
			raw->data.mouse.usButtonData,
			raw->data.mouse.ulRawButtons,
			raw->data.mouse.lLastX,
			raw->data.mouse.lLastY,
			raw->data.mouse.ulExtraInformation;

		Vector2i delta(static_cast<int>(raw->data.mouse.lLastX), static_cast<int>(raw->data.mouse.lLastY));

		bool pressed = MOUSE_BUTTON_IS_PRESSED(LEFT_BUTTON) || MOUSE_BUTTON_IS_PRESSED(RIGHT_BUTTON) || MOUSE_BUTTON_IS_PRESSED(MIDDLE_BUTTON) || MOUSE_BUTTON_IS_PRESSED(BUTTON_4) || MOUSE_BUTTON_IS_PRESSED(BUTTON_5);
		uint8 btn = Mouse::Buttons::NotDefine;
		if (PP(LEFT_BUTTON)) btn = Mouse::Buttons::Left;
		if (PP(RIGHT_BUTTON)) btn = Mouse::Buttons::Right;
		if (PP(MIDDLE_BUTTON)) btn = Mouse::Buttons::Middle;
		if (PP(BUTTON_4)) btn = Mouse::Buttons::X1;
		if (PP(BUTTON_5)) btn = Mouse::Buttons::X2;

		ButtonState::Code state = ButtonState::Released;
		if (pressed) {
			state = ButtonState::Pressed;
		}

		// FinalizePushEvent(new new MouseInputEvent(window->ID(), state, ms, btn, false, delta, globalMousePosition), 0, msg, window);
		return 0;
	}

	LRESULT Win32Event::HandleMouseMoveEvent(MSG msg, Win32Window* window)
	{
		LRESULT result = 0;

		float32 scaleFactor = window->GetDpiScale();
		//Vector2i positionGlobal(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
		Vector2i positionGlobal(static_cast<std::int16_t>(LOWORD(msg.lParam)) / scaleFactor, static_cast<std::int16_t>(HIWORD(msg.lParam)) / scaleFactor);

		RECT area;
		GetClientRect(window->GetData()->windowHandle, &area);

		if ((msg.wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0) {
			if (GetCapture() == window->GetData()->windowHandle) {
				ReleaseCapture();
			}
		}
		else if (GetCapture() != window->GetData()->windowHandle) {
			SetCapture(window->GetData()->windowHandle);
		}

		if ((positionGlobal.x < area.left) || (positionGlobal.x > area.right) ||
			(positionGlobal.y < area.top) || (positionGlobal.y > area.bottom)) {
			if (window->GetData()->isMouseInside) {
				window->GetData()->isMouseInside = false;
				SetMouseTracking(false, window);
				FinalizePushEvent(new MouseWindowEvent(window->ID(), RegionState::Exited, Vector2i(positionGlobal.x - (float32)area.left / scaleFactor, positionGlobal.y - (float32)area.top / scaleFactor)), 0, msg, window);
			}
		}
		else {
			if (!window->GetData()->isMouseInside) {
				window->GetData()->isMouseInside = true;
				SetMouseTracking(true, window);
				FinalizePushEvent(new MouseWindowEvent(window->ID(), RegionState::Entered, Vector2i(positionGlobal.x - (float32)area.left / scaleFactor, positionGlobal.y - (float32)area.top / scaleFactor)), 0, msg, window);
			}
		}

		Vector2i position(positionGlobal.x - area.left / scaleFactor, positionGlobal.y - area.top / scaleFactor);
		Vector2i move = position - previousMousePosition;
		previousMousePosition = mousePosition;

		mousePosition = position;
		globalMousePosition = window->GetPosition() + position;

		static bool isFirst = true;
		if (isFirst) {
			isFirst = false;
			return 0;
		}

		return FinalizePushEvent(new MouseMovedEvent(window->ID(), position, globalMousePosition, move), 0, msg, window);
	}

	LRESULT Win32Event::HandleMouseLeaveEvent(MSG msg, Win32Window* window)
	{
		// Avoid this firing a second time in case the cursor is dragged outside
		if (window->GetData()->isMouseInside) {
			window->GetData()->isMouseInside = false;
			return FinalizePushEvent(new MouseWindowEvent(window->ID(), RegionState::Exited), 0, msg, window);
		}
		return 0;
	}

	LRESULT Win32Event::HandleKeyboardEvent(MSG msg, Win32Window* window, bool keydown)
	{
		short modifiers = LOWORD(msg.wParam);
		ModifierState ms = Win32ECode::ModifierStateToWinkey();

		WORD keycode = LOWORD(msg.wParam);

		WORD keyFlags = HIWORD(msg.lParam);

		WORD scancode = LOBYTE(keyFlags);
		BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED;

		if (isExtendedKey) {
			scancode = MAKEWORD(scancode, 0xE0);
		}

		BOOL wasKeyDown = (keyFlags & KF_REPEAT) == KF_REPEAT;
		WORD repeatCount = LOWORD(msg.lParam);

		BOOL isKeyReleased = (keyFlags & KF_UP) == KF_UP;

		Keyboard::Keycode nts_keycode = Win32ECode::WinkeyToKeycodeSpecial(keycode, ms.shift);
		Keyboard::Scancode nts_scancode = Win32ECode::WinkeyToScancodeSpecial(scancode, ms.shift);

		ButtonState::Code state = ButtonState::Released;
		if (keydown) {
			state = ButtonState::Pressed;
		}
		return FinalizePushEvent(new KeyboardEvent(window->ID(), state, nts_keycode, nts_scancode, ms, (uint64)msg.wParam), 0, msg, window);
	}

	LRESULT Win32Event::HandleCharEvent(MSG msg, Win32Window* window, bool interpret)
	{
		if (interpret) {
			uint64 code = (uint64)msg.wParam;
			return FinalizePushEvent(new CharEnteredEvent(window->ID(), code, UnicodeToUTF8(code)), 0, msg, window);
		}
		return 0;
	}

	LRESULT Win32Event::RestricWindowSize(MSG msg, Win32Window* window)
	{
		WINDOWPOS& pos = *reinterpret_cast<PWINDOWPOS>(msg.lParam);
		if (pos.flags & SWP_NOSIZE)
			return 0;

		RECT rectangleMax = { 0, 0, static_cast<long>(window->GetProperties().maxSize.x), static_cast<long>(window->GetProperties().maxSize.y) };
		AdjustWindowRect(&rectangleMax, static_cast<DWORD>(GetWindowLongPtr(window->GetData()->windowHandle, GWL_STYLE)), false);
		const auto widthMax = rectangleMax.right - rectangleMax.left;
		const auto heightMax = rectangleMax.bottom - rectangleMax.top;

		Vector2i maximumSize = { widthMax, heightMax };

		RECT rectangleMin = { 0, 0, static_cast<long>(window->GetProperties().minSize.x), static_cast<long>(window->GetProperties().minSize.y) };
		AdjustWindowRect(&rectangleMin, static_cast<DWORD>(GetWindowLongPtr(window->GetData()->windowHandle, GWL_STYLE)), false);
		const auto widthMin = rectangleMin.right - rectangleMin.left;
		const auto heightMin = rectangleMin.bottom - rectangleMin.top;

		Vector2i minimumSize = { widthMin, heightMin };

		bool shouldResize = false;
		if (pos.cx > maximumSize.x)
		{
			pos.cx = maximumSize.x;
			shouldResize = true;
		}
		if (pos.cy > maximumSize.y)
		{
			pos.cy = maximumSize.y;
			shouldResize = true;
		}
		if (pos.cx < minimumSize.x)
		{
			pos.cx = minimumSize.x;
			shouldResize = true;
		}
		if (pos.cy < minimumSize.y)
		{
			pos.cy = minimumSize.y;
			shouldResize = true;
		}

		if (shouldResize)
			SetWindowPos(window->GetData()->windowHandle, pos.hwndInsertAfter, pos.x, pos.y, pos.cx, pos.cy, 0);
		return 0;
	}

	LRESULT Win32Event::HandleDropFilesEvent(MSG msg, Win32Window* window)
	{
		return 0;
	}

	void Win32Event::SetMouseTracking(bool track, Win32Window* window) {
		TRACKMOUSEEVENT mouseEvent;
		mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		mouseEvent.dwFlags = track ? TME_LEAVE : TME_CANCEL;
		mouseEvent.hwndTrack = window->GetData()->windowHandle;
		mouseEvent.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&mouseEvent);
	}

}  //  nkentseu

#endif
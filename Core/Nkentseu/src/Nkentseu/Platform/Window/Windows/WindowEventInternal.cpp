//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 4:51:55 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowEventInternal.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include <System/Nature/Base.h>
#include "Nkentseu/Event/Event.h"
#include "Nkentseu/Event/WindowEvent.h"
#include "Nkentseu/Event/KeyboardEvent.h"
#include "Nkentseu/Event/MouseEvent.h"
#include "Nkentseu/Event/GenericInputEvent.h"

#include "WindowEventCode.h"
#include "WindowInternal.h"

#include <windowsx.h>

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT)0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT)0x02)
#endif

// some sizing border definitions

#define MINX 200
#define MINY 200
#define BORDERWIDTH 5
#define TITLEBARHEIGHT 32
#define TITLEBARZOOMHEIGHT 23

#pragma comment(lib,"winmm.lib")

#include <tchar.h>
#define _USE_MATH_DEFINES
#define QWORD uint64_t

#include <math.h>
#include <hidsdi.h>
#include <inttypes.h>
#include <Dbt.h>
#include <hidpi.h>
#include <Nkentseu/Event/InputManager.h>
#include <Nkentseu/Core/NkentseuLogger.h>
#include <dwmapi.h>

#pragma comment(lib, "hid.lib")

//#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#define WC_MAINFRAME	TEXT("MainFrame")
#define MAX_BUTTONS		128
#define CHECK(exp)		{ if(!(exp)) goto Error; }
#define SAFE_FREE(p)	{ if(p) { HeapFree(hHeap, 0, p); (p) = NULL; } }

namespace nkentseu {
	//
	// Global variables
	//

	ButtonState::Code bButtonStates[MAX_BUTTONS];
	LONG lAxisX;
	LONG lAxisY;
	LONG lAxisZ;
	LONG lAxisRz;
	LONG lHat;
	INT  g_NumberOfButtons;

	WindowEventInternal& WindowEventInternal::GetInstance() {
        static WindowEventInternal eventManager;
        return eventManager;
    }

	WindowEventInternal::WindowEventInternal() : isInitialized(false) {
		joysticks = GInput.createJoysticks();
	}

	WindowEventInternal::~WindowEventInternal() {
	}

	void WindowEventInternal::Update() {
		MSG msg = {};

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GInput.updateJoysticks(&joysticks);
		for (unsigned int joystickIndex = 0; joystickIndex < joysticks.count; ++joystickIndex)
		{
			JoystickState* state = &joysticks.states[joystickIndex];
			for (unsigned int inputIndex = 0; inputIndex < state->inputCount; ++inputIndex)
			{
				if (state->currentInputs[inputIndex] > 0.5 && state->previousInputs[inputIndex] <= 0.5f)
				{
					const char* inputName = GInput.getInputName(joysticks, joystickIndex, inputIndex);
					Log_nts.Trace("{0}", inputName);
				}

				float rumbleLeft = 0;
				float rumbleRight = 0;
				if (state->type == JoystickType::JoystickTypeXbox) {
					state->lightRumble = state->currentInputs[(int32)XboxInputs::XboxInputLeftTrigger];
					state->heavyRumble = state->currentInputs[(int32)XboxInputs::XboxInputRightTrigger];
				}
				else if (state->type == JoystickType::JoystickTypeDualshock4) {
					state->heavyRumble = state->currentInputs[(int32)DS4Inputs::DS4InputL2];
					state->lightRumble = state->currentInputs[(int32)DS4Inputs::DS4InputR2];
					state->ledRed = (state->currentInputs[(int32)DS4Inputs::DS4InputCircle] || state->currentInputs[(int32)DS4Inputs::DS4InputSquare]) ? 1.0f : 0.0f;
					state->ledGreen = state->currentInputs[(int32)DS4Inputs::DS4InputTriangle];
					state->ledBlue = (state->currentInputs[(int32)DS4Inputs::DS4InputX] || state->currentInputs[(int32)DS4Inputs::DS4InputSquare]) ? 1.0f : 0.0f;
				}
			}
		}

		Input.private__update__axis();
	}

	Event* WindowEventInternal::Front() {
		return eventQueue.front();
	}

	Event& WindowEventInternal::FrontReference()
	{
		return *eventQueue.front();
	}

	void WindowEventInternal::Pop() {
		if (eventQueue.empty()) return;

		Event* event = eventQueue.front();
		//eventQueue.pop();
		eventQueue.erase(eventQueue.begin());

		delete event;
	}

	bool WindowEventInternal::Empty() {
		return eventQueue.empty();
	}

	size_t WindowEventInternal::Size() {
		return eventQueue.size();
	}

	LRESULT WindowEventInternal::WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (hwnd == nullptr) return 0;

		WindowInternal* nativeWindow = WindowInternal::GetCurrent(hwnd);

		if (nativeWindow == nullptr) {
			return 0;
		}

		return EventInternal.WindowProc(msg, wparam, lparam, nativeWindow);
	}

	LRESULT WindowEventInternal::WindowProc(UINT msg, WPARAM wparam, LPARAM lparam, WindowInternal* nativeWindow) {
		if (nativeWindow == nullptr || nativeWindow->GetWindowDisplay() == nullptr) return 0;

		MSG message;
		message.hwnd = nativeWindow->GetWindowDisplay()->windowHandle;
		message.lParam = lparam;
		message.wParam = wparam;
		message.message = msg;
		message.time = 0;

		LRESULT result = PushEvent(message, nativeWindow);

		// We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
		if (message.message == WM_CLOSE)
			return 0;

		// Don't forward the menu system command, so that pressing ALT or F10 doesn't steal the focus
		if ((message.message == WM_SYSCOMMAND) && (message.wParam == SC_KEYMENU))
			return 0;

		if (result > 0) return result;

		return DefWindowProc(nativeWindow->GetWindowDisplay()->windowHandle, msg, wparam, lparam);
	}

	LRESULT WindowEventInternal::PushEvent(MSG msg, WindowInternal* window) {
		if (window->GetWindowDisplay()->windowHandle == nullptr) return 0;

		Joysticks* joysticks = (Joysticks*)GetPropA(window->GetWindowDisplay()->windowHandle, "userData");

		UINT message = msg.message;
		LRESULT result = 0;
		RECT currentWindowRect = { -1, -1, -1, -1 };
		static bool move = false;

		#define CBTN ((HIWORD(msg.wParam) & XBUTTON1) ? Mouse::Buttons::X1 : Mouse::Buttons::X2)
		//InvalidateRect(window->GetWindowDisplay()->windowHandle, NULL, TRUE);

		switch (message) {
		case WM_SETCURSOR:
		{
			if (LOWORD(msg.lParam) == HTCLIENT) {
				SetCursor(window->GetWindowDisplay()->isCursorVisible ? window->GetWindowDisplay()->lastCursor : nullptr);
			}

			break;
		}
		// input
		case WM_INPUT:
		{ GInput.updateRawInput(joysticks, msg.lParam); return HandleInputRawEvent(msg, window); break; }
		case WM_DEVICECHANGE:
		case WM_INPUT_DEVICE_CHANGE:
		{ GInput.updateConnectionStatus(joysticks, (HANDLE)msg.lParam, msg.wParam); return HandleInputDeviceChangeEvent(msg, window); break; }

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

		// SwapBuffers(window->m_NativeWindow->deviceContext);

		return 0;
	}

	LRESULT WindowEventInternal::FinalizePushEvent(Event* event, LRESULT info, MSG msg, WindowInternal* native, const RECT& currentWindowRect) {
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
			SetWindowPos(native->GetWindowDisplay()->windowHandle, NULL, currentWindowRect.left,
				currentWindowRect.top,
				currentWindowRect.right - currentWindowRect.left,
				currentWindowRect.bottom - currentWindowRect.top,
				SWP_NOZORDER | SWP_NOACTIVATE);
		}

		return info;
	}

	void WindowEventInternal::TriggerEvent(Event* event)
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
	}

	LRESULT WindowEventInternal::HandleCreateEvent(MSG msg, WindowInternal* window) {
		HandleWindowCreateEvent(msg, window);

		RAWINPUTDEVICE deviceList[2] = { 0 };

		deviceList[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		deviceList[0].usUsage = HID_USAGE_GENERIC_JOYSTICK;
		deviceList[0].dwFlags = RIDEV_INPUTSINK;
		deviceList[0].hwndTarget = window->GetWindowDisplay()->windowHandle;

		deviceList[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
		deviceList[1].usUsage = HID_USAGE_GENERIC_GAMEPAD;
		deviceList[1].dwFlags = RIDEV_INPUTSINK;
		deviceList[1].hwndTarget = window->GetWindowDisplay()->windowHandle;

		UINT deviceCount = sizeof(deviceList) / sizeof(*deviceList);
		RegisterRawInputDevices(deviceList, deviceCount, sizeof(RAWINPUTDEVICE));

		return 0;
	}

	LRESULT WindowEventInternal::HandleWindowCreateEvent(MSG msg, WindowInternal* window) {
		// Input;
		return FinalizePushEvent(new WindowStatusEvent(window->ID(), WindowState::Created, window->GetProperties()), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleWindowPaintEvent(MSG msg, WindowInternal* window) {
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

				hDC = BeginPaint(window->GetWindowDisplay()->windowHandle, &ps);
				SetBkMode(hDC, TRANSPARENT);

				{
					//GetWindowRect(window->GetWindowDisplay()->windowHandle, &rect);
					GetClientRect(window->GetWindowDisplay()->windowHandle, &rect);
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

				EndPaint(window->GetWindowDisplay()->windowHandle, &ps);

		#endif

		return result;
	}

	LRESULT WindowEventInternal::HandleWindowBackgroundEraseEvent(MSG msg, WindowInternal* window) {
		LRESULT result = 0;

#if defined(NKENTSEU_GRAPHICS_API_SOFTWARE)

		PAINTSTRUCT ps;
		BeginPaint(window->GetWindowDisplay()->windowHandle, &ps);
		RECT rect;
		GetWindowRect(window->GetWindowDisplay()->windowHandle, &rect);
		Rectangle r(rect.left, rect.bottom, rect.right - rect.left, rect.bottom - rect.top);
		Color bg = window->GetBackgroundColor();
		HBRUSH BorderBrush = CreateSolidBrush(RGB(bg.R(), bg.G(), bg.B()));
		FillRect(ps.hdc, &rect, BorderBrush);

		result = FinalizePushEvent(new WindowRenderedEvent(window->ID(), r, bg), 0, msg, window);

		EndPaint(window->GetWindowDisplay()->windowHandle, &ps);

#endif

		return result;
	}

	LRESULT WindowEventInternal::HandleWindowCloseEvent(MSG msg, WindowInternal* window) {
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

	LRESULT WindowEventInternal::HandleWindowFocusedEvent(MSG msg, WindowInternal* window, bool focused) {
		return FinalizePushEvent(new WindowFocusedEvent(window->ID(), focused ? FocusState::Focused : FocusState::Unfocused), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleWindowResizeEvent(MSG msg, WindowInternal* window, bool resizing) {
		LRESULT result = 0;

		float32 area = (float32)(window->GetSize().width * window->GetSize().height);

		Rectangle win;
		win.corner = window->GetProperties().position;

		if (!resizing) {
			RECT rect;//, frame, border;
			GetClientRect(window->GetWindowDisplay()->windowHandle, &rect);
			win.size.width = (float32)(rect.right - rect.left);
			win.size.height = (float32)(rect.bottom - rect.top);
			RedrawWindow(window->GetWindowDisplay()->windowHandle, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);
		}
		else {
			uint32 STEP = 1;
			PRECT rectp = (PRECT)msg.lParam;

			// Get the window and client dimensions
			tagRECT wind, rect;
			GetWindowRect(window->GetWindowDisplay()->windowHandle, &wind);
			GetClientRect(window->GetWindowDisplay()->windowHandle, &rect);

			win.size.width = (float32)(rect.right - rect.left);
			win.size.height = (float32)(rect.bottom - rect.top);

			// Redraw window to refresh it while resizing
			RedrawWindow(window->GetWindowDisplay()->windowHandle, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);
			result = WVR_REDRAW;
		}

		float32 scaleFactor = window->GetDpiScale();
		win.size.width /= scaleFactor;
		win.size.height /= scaleFactor;

		float32 newarea = win.size.width * win.size.height;
		ResizeState::Code state = ResizeState::NotChange;

		if (newarea > area) {
			state = ResizeState::Expanded;
		}
		else if (newarea < area) {
			state = ResizeState::Reduced;
		}

		window->m_Properties.size = win.size;

		return FinalizePushEvent(new WindowResizedEvent(window->ID(), state, win), result, msg, window);
	}

	LRESULT WindowEventInternal::HandleWindowNCHITTESTEvent(MSG msg, WindowInternal* window) {
		LRESULT result = 0;
		RECT rect;
		GetWindowRect(window->GetWindowDisplay()->windowHandle, &rect);
		int x, y, width, height;
		x = static_cast<int>(GET_X_LPARAM(msg.lParam)) - rect.left;
		y = static_cast<int>(GET_Y_LPARAM(msg.lParam)) - rect.top;
		width = static_cast<int>(rect.right - rect.left);
		height = static_cast<int>(rect.bottom - rect.top);
		int topBorder = IsZoomed(window->GetWindowDisplay()->windowHandle) ? 0 : BORDERWIDTH;

		if (y > topBorder && x > 260 && x < (width - 260) && y < 32) {
			return HTCAPTION;
		}
		if (x > BORDERWIDTH && y > BORDERWIDTH && x < width - BORDERWIDTH &&
			y < height - BORDERWIDTH) {
			return HTCLIENT;
		}
		return 0;
	}

	LRESULT WindowEventInternal::HandleWindowDpiEvent(MSG msg, WindowInternal* window) {
		RECT currentWindowRect = { -1, -1, -1, -1 };
		WORD curDPI = HIWORD(msg.wParam);
		FLOAT fscale = (float)curDPI / USER_DEFAULT_SCREEN_DPI;

		if (!IsZoomed(window->GetWindowDisplay()->windowHandle)) {
			RECT* const prcNewWindow = (RECT*)msg.lParam;
			if (prcNewWindow) {
				currentWindowRect = *prcNewWindow;
			}
		}
		return FinalizePushEvent(new WindowDpiChangedEvent(window->ID(), fscale), 0, msg, window, currentWindowRect);
	}

	LRESULT WindowEventInternal::HandleWindowNCCALCSIZEEvent(MSG msg, WindowInternal* window) {
		if (!window->GetProperties().frame) {
			if (msg.lParam && msg.wParam) {
				NCCALCSIZE_PARAMS* sz = (NCCALCSIZE_PARAMS*)msg.lParam;
				int32 titleHeight = TITLEBARHEIGHT;
				if (IsZoomed(window->GetWindowDisplay()->windowHandle)) {
					titleHeight = TITLEBARZOOMHEIGHT;
				}
				int32 iDpi = GetDpiForWindow(window->GetWindowDisplay()->windowHandle);
				if (iDpi != USER_DEFAULT_SCREEN_DPI) {
					titleHeight = MulDiv(titleHeight, iDpi, USER_DEFAULT_SCREEN_DPI);
				}
				sz->rgrc[0].top += -titleHeight;
			}
		}
		return 0;
	}

	LRESULT WindowEventInternal::HandleWindowGETMINMAXINFOEvent(MSG msg, WindowInternal* window) {
		MINMAXINFO* min_max = reinterpret_cast<MINMAXINFO*>(msg.lParam);

		RECT rectangleMin = { 0, 0, static_cast<long>(window->GetProperties().minSize.x), static_cast<long>(window->GetProperties().minSize.y) };
		AdjustWindowRect(&rectangleMin, static_cast<DWORD>(GetWindowLongPtr(window->GetWindowDisplay()->windowHandle, GWL_STYLE)), false);
		const auto widthMin = rectangleMin.right - rectangleMin.left;
		const auto heightMin = rectangleMin.bottom - rectangleMin.top;

		RECT rectangleMax = { 0, 0, static_cast<long>(window->GetProperties().maxSize.x), static_cast<long>(window->GetProperties().maxSize.y) };
		AdjustWindowRect(&rectangleMax, static_cast<DWORD>(GetWindowLongPtr(window->GetWindowDisplay()->windowHandle, GWL_STYLE)), false);
		const auto widthMax = rectangleMax.right - rectangleMax.left;
		const auto heightMax = rectangleMax.bottom - rectangleMax.top;

		min_max->ptMinTrackSize.x = widthMin;
		min_max->ptMinTrackSize.y = heightMin;

		min_max->ptMaxSize.x = widthMax;
		min_max->ptMaxSize.y = heightMax;
		return 0;
	}

	LRESULT WindowEventInternal::HandleWindowMoveEvent(MSG msg, WindowInternal* window, uint8 t) {
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
			window->m_Properties.position = position;
			return FinalizePushEvent(new WindowMovedEvent(window->ID(), position, lasPosition), 0, msg, window, currentWindowRect);
		}
		else if (t == 1) {
			RECT* const r = (LPRECT)msg.lParam;
			if (r) {
				currentWindowRect = *r;
				position = Vector2i(r->left / scaleFactor, r->top / scaleFactor);
				window->m_Properties.position = position;
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
				window->m_Properties.position = position;
				return FinalizePushEvent(new WindowMovedEvent(window->ID(), position, lasPosition), 0, msg, window, currentWindowRect);
			}
		}
		return 0;
	}

	LRESULT WindowEventInternal::HandleMouseWheelEvent(MSG msg, WindowInternal* window, bool vertical) {
		short modifiers = LOWORD(msg.wParam);
		float32 delta = GET_WHEEL_DELTA_WPARAM(msg.wParam) / (float32)WHEEL_DELTA;
		ModifierState ms(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0);

		POINT position;
		position.x = static_cast<std::int16_t>(LOWORD(msg.lParam));
		position.y = static_cast<std::int16_t>(HIWORD(msg.lParam));

		ScreenToClient(window->GetWindowDisplay()->windowHandle, &position);

		Mouse::Wheel wheel = (vertical) ? Mouse::Wheels::Vertical : Mouse::Wheels::Horizontal;

		return FinalizePushEvent(new MouseWheelEvent(window->ID(), wheel, delta, ms, Vector2i(position.x, position.y)), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleMouseButtonEvent(MSG msg, WindowInternal* window, uint8 btn, bool pressed, bool dbclick) {
		if (pressed) {
			SetCapture(window->GetWindowDisplay()->windowHandle);
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
		GetClientRect(window->GetWindowDisplay()->windowHandle, &area);

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

	#define MOUSE_BUTTON_IS_PRESSED(v) (raw->data.mouse.ulButtons & (RI_MOUSE_##v##_DOWN))
	#define PPU(v, d) (raw->data.mouse.ulButtons & (RI_MOUSE_##v##_UP))
	#define PP(v) (raw->data.mouse.ulButtons & (RI_MOUSE_##v##_DOWN | RI_MOUSE_##v##_UP))

	LRESULT WindowEventInternal::HandleMouseButtonRawEvent(MSG msg, WindowInternal* window, RAWINPUT* raw) {
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

		// FinalizePushEvent(new MouseInputEvent(window->ID(), state, ms, btn, false, delta, globalMousePosition), 0, msg, window);
		return 0;
	}

	LRESULT WindowEventInternal::HandleMouseMoveEvent(MSG msg, WindowInternal* window) {
		LRESULT result = 0;

		float32 scaleFactor = window->GetDpiScale();
		//Vector2i positionGlobal(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
		Vector2i positionGlobal(static_cast<std::int16_t>(LOWORD(msg.lParam)) / scaleFactor, static_cast<std::int16_t>(HIWORD(msg.lParam)) / scaleFactor);

		RECT area;
		GetClientRect(window->GetWindowDisplay()->windowHandle, &area);

		if ((msg.wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0) {
			if (GetCapture() == window->GetWindowDisplay()->windowHandle) {
				ReleaseCapture();
			}
		}
		else if (GetCapture() != window->GetWindowDisplay()->windowHandle) {
			SetCapture(window->GetWindowDisplay()->windowHandle);
		}

		if ((positionGlobal.x < area.left) || (positionGlobal.x > area.right) ||
			(positionGlobal.y < area.top) || (positionGlobal.y > area.bottom)) {
			if (window->GetWindowDisplay()->isMouseInside) {
				window->GetWindowDisplay()->isMouseInside = false;
				SetMouseTracking(false, window);
				FinalizePushEvent(new MouseWindowEvent(window->ID(), RegionState::Exited, Vector2i(positionGlobal.x - (float32)area.left / scaleFactor, positionGlobal.y - (float32)area.top / scaleFactor)), 0, msg, window);
			}
		}
		else {
			if (!window->GetWindowDisplay()->isMouseInside) {
				window->GetWindowDisplay()->isMouseInside = true;
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
		if (isFirst){
			isFirst = false;
			return 0;
		}

		return FinalizePushEvent(new MouseMovedEvent(window->ID(), position, globalMousePosition, move), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleMouseLeaveEvent(MSG msg, WindowInternal* window) {
		// Avoid this firing a second time in case the cursor is dragged outside
		if (window->GetWindowDisplay()->isMouseInside) {
			window->GetWindowDisplay()->isMouseInside = false;
			return FinalizePushEvent(new MouseWindowEvent(window->ID(), RegionState::Exited), 0, msg, window);
		}
		return 0;
	}

	void WindowEventInternal::SetMouseTracking(bool track, WindowInternal* window) {
		TRACKMOUSEEVENT mouseEvent;
		mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		mouseEvent.dwFlags = track ? TME_LEAVE : TME_CANCEL;
		mouseEvent.hwndTrack = window->GetWindowDisplay()->windowHandle;
		mouseEvent.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&mouseEvent);
	}

	LRESULT WindowEventInternal::HandleKeyboardEvent(MSG msg, WindowInternal* window, bool keydown) {
		short modifiers = LOWORD(msg.wParam);
		ModifierState ms = WindowEventCode::ModifierStateToWinkey();

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

		Keyboard::Keycode nts_keycode = WindowEventCode::WinkeyToKeycodeSpecial(keycode, ms.shift);
		Keyboard::Scancode nts_scancode = WindowEventCode::WinkeyToScancodeSpecial(scancode, ms.shift);

		ButtonState::Code state = ButtonState::Released;
		if (keydown) {
			state = ButtonState::Pressed;
		}
		return FinalizePushEvent(new KeyboardEvent(window->ID(), state, nts_keycode, nts_scancode, ms, (uint64)msg.wParam), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleCharEvent(MSG msg, WindowInternal* window, bool interpret) {
		if (interpret) {
			uint64 code = (uint64)msg.wParam;
			return FinalizePushEvent(new CharEnteredEvent(window->ID(), code, UnicodeToUTF8(code)), 0, msg, window);
		}
		return 0;
	}

	LRESULT WindowEventInternal::HandleInputRawEvent(MSG msg, WindowInternal* window) {

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

	void WindowEventInternal::ParseRawInputData(MSG msg, WindowInternal* window, PRAWINPUT pRawInput, UINT size) {
	}

	void WindowEventInternal::UpdateRawInputData(MSG msg, WindowInternal* window, RAWINPUT* input) {
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
					bButtonStates[button] = ButtonState::NotDefine;
				}

				free(usages);

				for (GenericInput::Button j = GenericInput::Buttons::B0; j < GenericInput::Buttons::BMax; j++) {
					if (bButtonStates[j] == ButtonState::NotDefine) {
						bButtonStates[j] = ButtonState::Pressed;
						AnalyzeButtonRawInput(msg, window, deviceInfo.hid, name, j, true);
					}
					else if (bButtonStates[j] == ButtonState::Pressed) {
						bButtonStates[j] = ButtonState::Released;
						AnalyzeButtonRawInput(msg, window, deviceInfo.hid, name, j, false);
					}
				}
			}
			free(buttonCaps);
		}
		free(data);
	}

	void WindowEventInternal::AnalyzeAxisRawInput(MSG msg, WindowInternal* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value) {
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

	void WindowEventInternal::AnalyzeButtonRawInput(MSG msg, WindowInternal* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed) {
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

	LRESULT WindowEventInternal::HandleInputDeviceChangeEvent(MSG msg, WindowInternal* window) {
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

	LRESULT WindowEventInternal::HandleDropFilesEvent(MSG msg, WindowInternal* window) {
		return 0;
	}

	LRESULT WindowEventInternal::RestricWindowSize(MSG msg, WindowInternal* window)
	{
		WINDOWPOS& pos = *reinterpret_cast<PWINDOWPOS>(msg.lParam);
		if (pos.flags & SWP_NOSIZE)
			return 0;

		RECT rectangleMax = { 0, 0, static_cast<long>(window->GetProperties().maxSize.x), static_cast<long>(window->GetProperties().maxSize.y) };
		AdjustWindowRect(&rectangleMax, static_cast<DWORD>(GetWindowLongPtr(window->GetWindowDisplay()->windowHandle, GWL_STYLE)), false);
		const auto widthMax = rectangleMax.right - rectangleMax.left;
		const auto heightMax = rectangleMax.bottom - rectangleMax.top;

		Vector2i maximumSize = { widthMax, heightMax };

		RECT rectangleMin = { 0, 0, static_cast<long>(window->GetProperties().minSize.x), static_cast<long>(window->GetProperties().minSize.y) };
		AdjustWindowRect(&rectangleMin, static_cast<DWORD>(GetWindowLongPtr(window->GetWindowDisplay()->windowHandle, GWL_STYLE)), false);
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
			SetWindowPos(window->GetWindowDisplay()->windowHandle, pos.hwndInsertAfter, pos.x, pos.y, pos.cx, pos.cy, 0);
		return 0;
	}

	void WindowEventInternal::SetPS3GamepadAxis(MSG msg, WindowInternal* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value) {
	}

	void WindowEventInternal::SetPS3GamepadButton(MSG msg, WindowInternal* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed) {
	}
}    // namespace nkentseu

#endif
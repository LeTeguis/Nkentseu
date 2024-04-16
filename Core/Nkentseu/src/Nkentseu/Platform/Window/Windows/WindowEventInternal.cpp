//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 4:51:55 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowEventInternal.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include "Nkentseu/Event/Event.h"
#include "Nkentseu/Event/WindowEvent.h"
#include "Nkentseu/Event/KeyboardEvent.h"
#include "Nkentseu/Event/MouseEvent.h"
#include "Nkentseu/Event/GenericInputEvent.h"

#include "WindowDisplay.h"
#include "Nkentseu/Core/Window.h"

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

#pragma comment(lib, "hid.lib")

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#define WC_MAINFRAME	TEXT("MainFrame")
#define MAX_BUTTONS		128
#define CHECK(exp)		{ if(!(exp)) goto Error; }
#define SAFE_FREE(p)	{ if(p) { HeapFree(hHeap, 0, p); (p) = NULL; } }

namespace nkentseu {
	//
	// Global variables
	//

	enum class GInputState {
		PrepareToPressed, Pressed, Released
	};

	GInputState bButtonStates[MAX_BUTTONS];
	LONG lAxisX;
	LONG lAxisY;
	LONG lAxisZ;
	LONG lAxisRz;
	LONG lHat;
	INT  g_NumberOfButtons;

	WindowEventInternal::WindowEventInternal() : isInitialized(false) {
		//JoystickInternalInof = PtrAlloc(JoystickInternal);
	}

	void WindowEventInternal::Update() {
		MSG msg = {};

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Input.private__update__axis();
	}

	Event* WindowEventInternal::Front() {
		return eventQueue.front();
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

		WindowDisplay* nativeWindow = WindowDisplay::GetCurrent(hwnd);

		if (nativeWindow == nullptr) {
			return 0;
		}

		return EventInternal->WindowProc(msg, wparam, lparam, nativeWindow);
	}

	LRESULT WindowEventInternal::WindowProc(UINT msg, WPARAM wparam, LPARAM lparam, WindowDisplay* nativeWindow) {
		if (nativeWindow == nullptr) return 0;

		MSG message;
		message.hwnd = nativeWindow->windowHandle;
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

		return DefWindowProc(nativeWindow->windowHandle, msg, wparam, lparam);
	}

	LRESULT WindowEventInternal::PushEvent(MSG msg, WindowDisplay* window) {
		if (window == nullptr || window->windowHandle == nullptr) return 0;

		if (window->windowSuper == nullptr) {
			return 0;
		}

		UINT message = msg.message;
		LRESULT result = 0;
		RECT currentWindowRect = { -1, -1, -1, -1 };
		static bool move = false;

		#define CBTN ((HIWORD(msg.wParam) & XBUTTON1) ? Mouse::X1_ev : Mouse::X2_ev)

		switch (message) {
		case WM_SETCURSOR:
		{
			if (LOWORD(msg.lParam) == HTCLIENT) {
				SetCursor(window->isCursorVisible ? window->lastCursor : nullptr);
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
		case WM_MOUSEWHEEL: { return HandleMouseWheelEvent(msg, window); break; }
		case WM_LBUTTONDBLCLK: case WM_LBUTTONDOWN: case WM_LBUTTONUP:
		{ return HandleMouseButtonEvent(msg, window, Mouse::Left_ev, WM_LBUTTONDOWN == message, WM_LBUTTONDBLCLK == message); break; }
		case WM_MBUTTONDBLCLK: case WM_MBUTTONDOWN: case WM_MBUTTONUP:
		{ return HandleMouseButtonEvent(msg, window, Mouse::Middle_ev, WM_MBUTTONDOWN == message, WM_MBUTTONDBLCLK == message); break; }
		case WM_RBUTTONDBLCLK: case WM_RBUTTONDOWN: case WM_RBUTTONUP:
		{ return HandleMouseButtonEvent(msg, window, Mouse::Right_ev, WM_RBUTTONDOWN == message, WM_RBUTTONDBLCLK == message); break; }
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
		default:
			// Do nothing
			break;
		}

		return 0;
	}

	LRESULT WindowEventInternal::FinalizePushEvent(Event* event, LRESULT info, MSG msg, WindowDisplay* native, const RECT& currentWindowRect) {
		if (event->GetEventType() != EventType::None_ev) {
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
			SetWindowPos(native->windowHandle, NULL, currentWindowRect.left,
				currentWindowRect.top,
				currentWindowRect.right - currentWindowRect.left,
				currentWindowRect.bottom - currentWindowRect.top,
				SWP_NOZORDER | SWP_NOACTIVATE);
		}

		return info;
	}

	LRESULT WindowEventInternal::HandleCreateEvent(MSG msg, WindowDisplay* window) {
		HandleWindowCreateEvent(msg, window);

		RAWINPUTDEVICE deviceList[2] = { 0 };

		deviceList[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		deviceList[0].usUsage = HID_USAGE_GENERIC_JOYSTICK;
		deviceList[0].dwFlags = RIDEV_INPUTSINK;
		deviceList[0].hwndTarget = window->windowHandle;

		deviceList[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
		deviceList[1].usUsage = HID_USAGE_GENERIC_GAMEPAD;
		deviceList[1].dwFlags = RIDEV_INPUTSINK;
		deviceList[1].hwndTarget = window->windowHandle;

		UINT deviceCount = sizeof(deviceList) / sizeof(*deviceList);
		RegisterRawInputDevices(deviceList, deviceCount, sizeof(RAWINPUTDEVICE));

		return 0;
	}

	LRESULT WindowEventInternal::HandleWindowCreateEvent(MSG msg, WindowDisplay* window) {
		//Input;
		return FinalizePushEvent(new WindowCreateEvent(window->windowSuper->ID()), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleWindowPaintEvent(MSG msg, WindowDisplay* window) {
		// Call InvalidateRect. This function forces the window to be repainted. InvalidateRect(m_hwnd, NULL, FALSE);

		LRESULT result = 0;

		#if defined(NKENTSEU_GAPI_SOFTWARE)

				PAINTSTRUCT ps;
				HDC         hDC;
				HBRUSH hOldBrush;
				HBRUSH hBr;
				Color bg;
				RECT rect;
				Rectangle rectangle;

				hDC = BeginPaint(window->windowHandle, &ps);
				SetBkMode(hDC, TRANSPARENT);

				{
					//GetWindowRect(window->windowHandle, &rect);
					GetClientRect(window->windowHandle, &rect);
					rectangle = Rectangle(rect.left, rect.bottom, rect.right - rect.left, rect.bottom - rect.top);

					bg = window->Properties__.BackgroundColor;
					hBr = CreateSolidBrush(RGB(bg.R(), bg.G(), bg.B()));
					hOldBrush = (HBRUSH)SelectObject(hDC, hBr);
				}
				FillRect(ps.hdc, &rect, hBr);

				result = FinalizePushEvent(new WindowPaintEvent(window->Window__->ID(), rectangle, bg), 0, msg, window);

				{
					SelectObject(hDC, hOldBrush);
					DeleteObject(hBr);
				}

				EndPaint(window->windowHandle, &ps);

		#endif

		return result;
	}

	LRESULT WindowEventInternal::HandleWindowBackgroundEraseEvent(MSG msg, WindowDisplay* window) {
		LRESULT result = 0;

#if defined(NKENTSEU_GAPI_SOFTWARE)

		PAINTSTRUCT ps;
		BeginPaint(window->windowHandle, &ps);
		RECT rect;
		GetWindowRect(window->windowHandle, &rect);
		Rectangle r(rect.left, rect.bottom, rect.right - rect.left, rect.bottom - rect.top);
		Color bg = window->Properties__.BackgroundColor;
		HBRUSH BorderBrush = CreateSolidBrush(RGB(bg.R(), bg.G(), bg.B()));
		FillRect(ps.hdc, &rect, BorderBrush);

		result = FinalizePushEvent(new WindowPaintEvent(window->windowSuper->ID(), r, bg), 0, msg, window);

		EndPaint(window->windowHandle, &ps);

#endif

		return result;
	}

	LRESULT WindowEventInternal::HandleWindowCloseEvent(MSG msg, WindowDisplay* window) {
		// Restore the previous video mode (in case we were running in fullscreen)
		ChangeDisplaySettingsW(nullptr, 0);

		// Unhide the mouse cursor (in case it was hidden)
		window->windowSuper->ShowMouse(true);

		// No longer track the cursor
		SetMouseTracking(false, window);

		// No longer capture the cursor
		ReleaseCapture();
		return FinalizePushEvent(new WindowCloseEvent(window->windowSuper->ID()), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleWindowFocusedEvent(MSG msg, WindowDisplay* window, bool focused) {
		return FinalizePushEvent(new WindowFocusEvent(window->windowSuper->ID(), focused), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleWindowResizeEvent(MSG msg, WindowDisplay* window, bool resizing) {
		LRESULT result = 0;

		Vector2u size;
		if (!resizing) {
			RECT r;
			GetClientRect(window->windowHandle, &r);
			size.width = r.right - r.left;
			size.height = r.bottom - r.top;
			RedrawWindow(window->windowHandle, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);
		}
		else {
			uint32 STEP = 1;
			PRECT rectp = (PRECT)msg.lParam;

			// Get the window and client dimensions
			tagRECT wind, rect;
			GetWindowRect(window->windowHandle, &wind);
			GetClientRect(window->windowHandle, &rect);

			size.width = rect.right - rect.left;
			size.height = rect.bottom - rect.top;

			// Redraw window to refresh it while resizing
			RedrawWindow(window->windowHandle, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);
			result = WVR_REDRAW;
		}
		return FinalizePushEvent(new WindowResizeEvent(window->windowSuper->ID(), size, resizing), result, msg, window);
	}

	LRESULT WindowEventInternal::HandleWindowNCHITTESTEvent(MSG msg, WindowDisplay* window) {
		LRESULT result = 0;
		RECT rect;
		GetWindowRect(window->windowHandle, &rect);
		int x, y, width, height;
		x = static_cast<int>(GET_X_LPARAM(msg.lParam)) - rect.left;
		y = static_cast<int>(GET_Y_LPARAM(msg.lParam)) - rect.top;
		width = static_cast<int>(rect.right - rect.left);
		height = static_cast<int>(rect.bottom - rect.top);
		int topBorder = IsZoomed(window->windowHandle) ? 0 : BORDERWIDTH;

		if (y > topBorder && x > 260 && x < (width - 260) && y < 32) {
			return HTCAPTION;
		}
		if (x > BORDERWIDTH && y > BORDERWIDTH && x < width - BORDERWIDTH &&
			y < height - BORDERWIDTH) {
			return HTCLIENT;
		}
		return 0;
	}

	LRESULT WindowEventInternal::HandleWindowDpiEvent(MSG msg, WindowDisplay* window) {
		RECT currentWindowRect = { -1, -1, -1, -1 };
		WORD curDPI = HIWORD(msg.wParam);
		FLOAT fscale = (float)curDPI / USER_DEFAULT_SCREEN_DPI;

		if (!IsZoomed(window->windowHandle)) {
			RECT* const prcNewWindow = (RECT*)msg.lParam;
			if (prcNewWindow) {
				currentWindowRect = *prcNewWindow;
			}
		}
		return FinalizePushEvent(new WindowDpiEvent(window->windowSuper->ID(), fscale), 0, msg, window, currentWindowRect);
	}

	LRESULT WindowEventInternal::HandleWindowNCCALCSIZEEvent(MSG msg, WindowDisplay* window) {
		if (!window->windowProperties.frame) {
			if (msg.lParam && msg.wParam) {
				NCCALCSIZE_PARAMS* sz = (NCCALCSIZE_PARAMS*)msg.lParam;
				int32 titleHeight = TITLEBARHEIGHT;
				if (IsZoomed(window->windowHandle)) {
					titleHeight = TITLEBARZOOMHEIGHT;
				}
				int32 iDpi = GetDpiForWindow(window->windowHandle);
				if (iDpi != USER_DEFAULT_SCREEN_DPI) {
					titleHeight = MulDiv(titleHeight, iDpi, USER_DEFAULT_SCREEN_DPI);
				}
				sz->rgrc[0].top += -titleHeight;
			}
		}
		return 0;
	}

	LRESULT WindowEventInternal::HandleWindowGETMINMAXINFOEvent(MSG msg, WindowDisplay* window) {
		MINMAXINFO* min_max = reinterpret_cast<MINMAXINFO*>(msg.lParam);

		min_max->ptMinTrackSize.x = window->windowProperties.minSize.width;
		min_max->ptMinTrackSize.y = window->windowProperties.minSize.height;

		min_max->ptMaxSize.x = window->windowProperties.maxSize.width;
		min_max->ptMaxSize.y = window->windowProperties.maxSize.height;
		return 0;
	}

	LRESULT WindowEventInternal::HandleWindowMoveEvent(MSG msg, WindowDisplay* window, uint8 t) {
		if (!window->windowProperties.movable) {
			window->windowSuper->SetPosition(window->windowProperties.position);
			return 0;
		}
		Vector2i position = window->windowProperties.position;
		RECT currentWindowRect = { -1, -1, -1, -1 };

		if (t % 3 == 0) {
			//position = { (LOWORD)msg.lParam, (HIWORD)msg.lParam };
		}
		else if (t % 3 == 1) {
			RECT* const r = (LPRECT)msg.lParam;
			if (r) {
				currentWindowRect = *r;
				position = Vector2i(r->left, r->top);
			}
		}
		else {
			RECT* const r = (LPRECT)msg.lParam;
			if (r) {
				currentWindowRect = *r;
				position = Vector2i(r->left, r->top);
			}
		}
		window->windowProperties.position = position;
		return FinalizePushEvent(new WindowMovedEvent(window->windowSuper->ID(), position), 0, msg, window, currentWindowRect);
	}

	LRESULT WindowEventInternal::HandleMouseWheelEvent(MSG msg, WindowDisplay* window) {
		short modifiers = LOWORD(msg.wParam);
		float32 delta = GET_WHEEL_DELTA_WPARAM(msg.wParam) / (float32)WHEEL_DELTA;
		ModifierState ms(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0);
		return FinalizePushEvent(new MouseWheelEvent(window->windowSuper->ID(), delta, ms), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleMouseButtonEvent(MSG msg, WindowDisplay* window, uint8 btn, bool pressed, bool dbclick) {
		if (pressed) {
			SetCapture(window->windowHandle);
		}
		else {
			ReleaseCapture();
		}

		short modifiers = LOWORD(msg.wParam);
		ModifierState ms(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0);

		Vector2i positionGlobal(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));

		RECT area;
		GetClientRect(window->windowHandle, &area);

		Vector2i position(positionGlobal.x - area.left, positionGlobal.y - area.top);
		Vector2i move = positionGlobal - previousMousePosition;
		previousMousePosition = positionGlobal;

		mousePosition = position;
		globalMousePosition = positionGlobal;

		if (dbclick) {
			return FinalizePushEvent(new MouseButtonDBCLKEvent(window->windowSuper->ID(), ms, btn, position), 0, msg, window);
		}
		if (pressed) {
			return FinalizePushEvent(new MouseButtonPressedEvent(window->windowSuper->ID(), ms, btn, position), 0, msg, window);
		}
		return FinalizePushEvent(new MouseButtonReleasedEvent(window->windowSuper->ID(), ms, btn, position), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleMouseButtonRawEvent(MSG msg, WindowDisplay* window, RAWINPUT* raw) {
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

#define PPD(v, d) (raw->data.mouse.ulButtons & (RI_MOUSE_##v##_DOWN))
#define PPU(v, d) (raw->data.mouse.ulButtons & (RI_MOUSE_##v##_UP))
#define PP(v) (raw->data.mouse.ulButtons & (RI_MOUSE_##v##_DOWN | RI_MOUSE_##v##_UP))

		bool pressed = PPD(LEFT_BUTTON) || PPD(RIGHT_BUTTON) || PPD(MIDDLE_BUTTON) || PPD(BUTTON_4) || PPD(BUTTON_5);
		uint8 btn = Mouse::Unknown_ev;
		if (PP(LEFT_BUTTON)) btn = Mouse::Left_ev;
		if (PP(RIGHT_BUTTON)) btn = Mouse::Right_ev;
		if (PP(MIDDLE_BUTTON)) btn = MOUSE(Middle);
		if (PP(BUTTON_4)) btn = Mouse::X1_ev;
		if (PP(BUTTON_5)) btn = Mouse::X2_ev;

		return FinalizePushEvent(new MouseButtonRawEvent(window->windowSuper->ID(), ms, btn, delta, pressed, mousePosition), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleMouseMoveEvent(MSG msg, WindowDisplay* window) {
		LRESULT result = 0;
		//Vector2i positionGlobal(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
		Vector2i positionGlobal(static_cast<std::int16_t>(LOWORD(msg.lParam)), static_cast<std::int16_t>(HIWORD(msg.lParam)));

		RECT area;
		GetClientRect(window->windowHandle, &area);

		if ((msg.wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0) {
			if (GetCapture() == window->windowHandle) {
				ReleaseCapture();
			}
		}
		else if (GetCapture() != window->windowHandle) {
			SetCapture(window->windowHandle);
		}

		if ((positionGlobal.x < area.left) || (positionGlobal.x > area.right) ||
			(positionGlobal.y < area.top) || (positionGlobal.y > area.bottom)) {
			if (window->isMouseInside) {
				window->isMouseInside = false;
				SetMouseTracking(false, window);
				FinalizePushEvent(new MouseExitedEvent(window->windowSuper->ID()), 0, msg, window);
			}
		}
		else {
			if (!window->isMouseInside) {
				window->isMouseInside = true;
				SetMouseTracking(true, window);
				FinalizePushEvent(new MouseEnteredEvent(window->windowSuper->ID()), 0, msg, window);
			}
		}

		Vector2i position(positionGlobal.x - area.left, positionGlobal.y - area.top);
		Vector2i move = positionGlobal - previousMousePosition;
		previousMousePosition = positionGlobal;

		mousePosition = position;
		globalMousePosition = positionGlobal;

		return FinalizePushEvent(new MouseMovedEvent(window->windowSuper->ID(), position, move, positionGlobal), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleMouseLeaveEvent(MSG msg, WindowDisplay* window) {
		// Avoid this firing a second time in case the cursor is dragged outside
		if (window->isMouseInside) {
			window->isMouseInside = false;
			return FinalizePushEvent(new MouseExitedEvent(window->windowSuper->ID()), 0, msg, window);
		}
		return 0;
	}

	void WindowEventInternal::SetMouseTracking(bool track, WindowDisplay* window) {
		TRACKMOUSEEVENT mouseEvent;
		mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		mouseEvent.dwFlags = track ? TME_LEAVE : TME_CANCEL;
		mouseEvent.hwndTrack = window->windowHandle;
		mouseEvent.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&mouseEvent);
	}

	LRESULT WindowEventInternal::HandleKeyboardEvent(MSG msg, WindowDisplay* window, bool keydown) {
		#define SCANDCODE_TO_KEYCODE(scancode) static_cast<uint64>(MapVirtualKey(scancode, MAPVK_VSC_TO_VK))
		#define KEYCODE_TO_SCANCODE(keycode) static_cast<uint64>(MapVirtualKey(keycode, MAPVK_VK_TO_VSC))

		short modifiers = LOWORD(msg.wParam);
		ModifierState ms(GetKeyState(VK_CONTROL) & 0x8000, GetKeyState(VK_MENU) & 0x8000,
			(GetKeyState(VK_SHIFT) & 0x8000) | (GetKeyState(VK_CAPITAL) & 0x0001), false);

		// Le parametre wParam contient le code de la touche (keycode)
		uint64 keycode = static_cast<uint64>(msg.wParam);

		// Le parametre lParam contient le scancode dans les bits 16 a 23
		uint64 scancode = static_cast<uint64>((msg.lParam >> 16) & 0xFF);
		scancode = SCANDCODE_TO_KEYCODE(scancode);

		#define CORRECT_ASSIGN(k, v1, v2, r) if (k == Keyboard::v1##_ev && GetKeyState(v2)) { k = Keyboard::r##_ev; }
		#define CORRECT_SHIFT(k, v, r) if (k == Keyboard::v##_ev) { k = Keyboard::r##_ev; }

		CORRECT_ASSIGN(keycode, LCtrl, VK_RCONTROL, RCtrl)
		CORRECT_ASSIGN(keycode, LAlt, VK_RMENU, RAlt)
		CORRECT_ASSIGN(keycode, LShift, VK_RSHIFT, RShift)

		CORRECT_ASSIGN(scancode, LCtrl, VK_RCONTROL, RCtrl)
		CORRECT_ASSIGN(scancode, LAlt, VK_RMENU, RAlt)
		CORRECT_ASSIGN(scancode, LShift, VK_RSHIFT, RShift)

		if (ms.Shift) {
			CORRECT_SHIFT(keycode, Semicolon, Colon)
			CORRECT_SHIFT(keycode, Apostrophe, Quotation)
			CORRECT_SHIFT(keycode, Equal, Plus)

			CORRECT_SHIFT(scancode, Semicolon, Colon)
			CORRECT_SHIFT(scancode, Apostrophe, Quotation)
			CORRECT_SHIFT(scancode, Equal, Plus)
		}

		if (keydown) {
			//Key::Distribution(keycode, true);
			return FinalizePushEvent(new KeyPressedEvent(window->windowSuper->ID(), keycode, scancode, ms), 0, msg, window);
		}
		//Key::Distribution(keycode, false);
		return FinalizePushEvent(new KeyReleasedEvent(window->windowSuper->ID(), keycode, scancode, ms), 0, msg, window);
	}

	LRESULT WindowEventInternal::HandleCharEvent(MSG msg, WindowDisplay* window, bool interpret) {
		if (interpret) {
			return FinalizePushEvent(new CharPressedEvent(window->windowSuper->ID(), (uint64)msg.wParam), 0, msg, window);
		}
		return 0;
	}

	LRESULT WindowEventInternal::HandleInputRawEvent(MSG msg, WindowDisplay* window) {
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

	void WindowEventInternal::ParseRawInputData(MSG msg, WindowDisplay* window, PRAWINPUT pRawInput, UINT size) {
	}

	void WindowEventInternal::UpdateRawInputData(MSG msg, WindowDisplay* window, RAWINPUT* input) {
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

				float32 value_concret = ((value - 127.0) / 127.0f);
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
					bButtonStates[button] = GInputState::PrepareToPressed;
				}

				free(usages);

				for (GenericInput::Button j = GenericInput::Button0_ev; j < GenericInput::ButtonMAX_ev; j++) {
					if (bButtonStates[j] == GInputState::PrepareToPressed) {
						bButtonStates[j] = GInputState::Pressed;
						AnalyzeButtonRawInput(msg, window, deviceInfo.hid, name, j, true);
					}
					else if (bButtonStates[j] == GInputState::Pressed) {
						bButtonStates[j] = GInputState::Released;
						AnalyzeButtonRawInput(msg, window, deviceInfo.hid, name, j, false);
					}
				}
			}
			free(buttonCaps);
		}
		free(data);
	}

	void WindowEventInternal::AnalyzeAxisRawInput(MSG msg, WindowDisplay* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value) {
		if (axis >= GenericInput::AxisMax_ev) return;

		//if ((devicecInfoHid.dwProductId == 0x09CC || devicecInfoHid.dwProductId == 0x05C4) && devicecInfoHid.dwVendorId == 0x054C) // PS4

		// ps3
		if (devicecInfoHid.dwProductId == 0x0268 && devicecInfoHid.dwVendorId == 0x054C) {
			SetPS3GamepadAxis(msg, window, devicecInfoHid, name, axis, transmit_value);
		}
		else {
			GenericInputInfos ginfos(devicecInfoHid.dwVendorId, devicecInfoHid.dwProductId, devicecInfoHid.dwVersionNumber, devicecInfoHid.usUsage, devicecInfoHid.usUsagePage, name);
			FinalizePushEvent(new GenericInputAxisEvent(window->windowSuper->ID(), ginfos, axis, transmit_value), 0, msg, window);
		}
	}

	void WindowEventInternal::AnalyzeButtonRawInput(MSG msg, WindowDisplay* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed) {
		if (button >= GenericInput::ButtonMAX_ev) return;

		if (devicecInfoHid.dwProductId == 0x0268 && devicecInfoHid.dwVendorId == 0x054C) {
			SetPS3GamepadButton(msg, window, devicecInfoHid, name, button, isPressed);
		}
		else {
			GenericInputInfos ginfos(devicecInfoHid.dwVendorId, devicecInfoHid.dwProductId, devicecInfoHid.dwVersionNumber, devicecInfoHid.usUsage, devicecInfoHid.usUsagePage, name);
			if (isPressed) {
				FinalizePushEvent(new GenericInputButtonPressedEvent(window->windowSuper->ID(), ginfos, button), 0, msg, window);
			}
			else {
				FinalizePushEvent(new GenericInputButtonReleasedEvent(window->windowSuper->ID(), ginfos, button), 0, msg, window);
			}
		}
	}

	LRESULT WindowEventInternal::HandleInputDeviceChangeEvent(MSG msg, WindowDisplay* window) {
		return 0;
	}

	LRESULT WindowEventInternal::HandleDropFilesEvent(MSG msg, WindowDisplay* window) {
		return 0;
	}

	void WindowEventInternal::SetPS3GamepadAxis(MSG msg, WindowDisplay* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value) {
	}

	void WindowEventInternal::SetPS3GamepadButton(MSG msg, WindowDisplay* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed) {
	}
}    // namespace nkentseu

#endif
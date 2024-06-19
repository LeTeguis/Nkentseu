//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:03:58 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "MouseEvent.h"
#include <Logger/Formatter.h>

namespace nkentseu {
	using namespace maths;

	MouseInputEvent::MouseInputEvent(uint64 win, ButtonState::Code buttonState, ModifierState modifierState, Mouse::Button button, bool isDoubleClicked, const Vector2i& position, const Vector2i& globalPosition) : Event(win), m_Button(button), m_ButtonState(buttonState), m_Position(position), m_GlobalPosition(globalPosition), m_ModifierState(modifierState), m_IsDoubleClicked(isDoubleClicked)
	{
	}
	MouseInputEvent::MouseInputEvent(const MouseInputEvent& e) : Event(e.GetWindow()), m_Button(e.m_Button), m_ButtonState(e.m_ButtonState), m_Position(e.m_Position), m_GlobalPosition(e.m_GlobalPosition), m_ModifierState(e.m_ModifierState), m_IsDoubleClicked(e.m_IsDoubleClicked)
	{
	}
	std::string MouseInputEvent::ToString() const
	{
		return FORMATTER.Format("MouseInputEvent {Window ID : {0}, Button : {1}, State : {2}, Position : {3}, GlobalPosition : {4}, ModifierState : {5}, DoubleClicked : {6}}", m_WindowID, Mouse::Buttons::ToString(m_Button), ButtonState::ToString(m_ButtonState), m_Position, m_GlobalPosition, m_ModifierState, STR_BOOL(m_IsDoubleClicked));
	}
	Mouse::Button MouseInputEvent::GetButton() const
	{
		return this->m_Button;
	}
	ButtonState::Code MouseInputEvent::GetState() const
	{
		return this->m_ButtonState;
	}
	ModifierState MouseInputEvent::GetModifierState() const
	{
		return this->m_ModifierState;
	}
	bool MouseInputEvent::IsDoubleCliked() const
	{
		return this->m_IsDoubleClicked;
	}
	Vector2i MouseInputEvent::GetPosition() const
	{
		return this->m_Position;
	}
	Vector2i MouseInputEvent::GetGlobalPosition() const
	{
		return this->m_GlobalPosition;
	}

	//-----------------------

	MouseMovedEvent::MouseMovedEvent(uint64 win, const Vector2i& position, const Vector2i& globalPosition, const Vector2i& speed) : Event(win), m_Speed(speed), m_Position(position), m_GlobalPosition(globalPosition)
	{
	}

	MouseMovedEvent::MouseMovedEvent(const MouseMovedEvent& e) : Event(e.GetWindow()), m_Speed(e.m_Speed), m_Position(e.m_Position),
		m_GlobalPosition(e.m_GlobalPosition)
	{
	}

	std::string MouseMovedEvent::ToString() const
	{
		return FORMATTER.Format("MouseMovedEvent {Window ID : {0}, Speed : {1}, Position : {2}, GlobalPosition : {3}}", m_WindowID, m_Speed, m_Position, m_GlobalPosition);
	}

	AxisState::Code MouseMovedEvent::GetVerticalAxisState() const
	{
		if (m_Speed.y == 0) return AxisState::Neutral;
		if (m_Speed.y > 0) return AxisState::Positive;
		return AxisState::Negative;
	}

	AxisState::Code MouseMovedEvent::GetHorizontalAxisState() const
	{
		if (m_Speed.x == 0) return AxisState::Neutral;
		if (m_Speed.x > 0) return AxisState::Positive;
		return AxisState::Negative;
	}

	AxisDirection::Code MouseMovedEvent::GetAxisDirection() const
	{
		if (m_Speed.x == 0) return AxisDirection::Vertical;
		if (m_Speed.y == 0) return AxisDirection::Horizontal;
		return AxisDirection::VerticalHorizontal;
	}

	Vector2i MouseMovedEvent::GetPosition() const
	{
		return this->m_Position;
	}

	Vector2i MouseMovedEvent::GetGlobalPosition() const
	{
		return this->m_GlobalPosition;
	}

	Vector2i MouseMovedEvent::GetSpeed() const
	{
		return this->m_Speed;
	}

	//-----------------------------------

	MouseWheelEvent::MouseWheelEvent(uint64 win, Mouse::Wheel wheel, float32 delta, ModifierState modifierState, const Vector2i& position) : Event(win), m_Delta(delta), m_Position(position), m_Wheel(wheel), m_ModifierState(modifierState)
	{
	}

	MouseWheelEvent::MouseWheelEvent(const MouseWheelEvent& e) : Event(e.GetWindow()), m_Delta(e.m_Delta), m_Position(e.m_Position), m_Wheel(e.m_Wheel), m_ModifierState(e.m_ModifierState)
	{
	}

	std::string MouseWheelEvent::ToString() const
	{
		return FORMATTER.Format("MouseWheelEvent {Window ID : {0}, Wheel : {3}, Delta : {1}, Position : {2}, ModifierState : {4}}", m_WindowID, m_Delta, m_Position, Mouse::Wheels::ToString(m_Wheel), m_ModifierState);
	}

	ModifierState MouseWheelEvent::GetModifierState() const
	{
		return this->m_ModifierState;
	}

	Vector2i MouseWheelEvent::GetPosition() const
	{
		return this->m_Position;
	}

	float32 MouseWheelEvent::GetDelta() const
	{
		return this->m_Delta;
	}

	Mouse::Wheel MouseWheelEvent::GetWheels() const
	{
		return this->m_Wheel;
	}

	//-----------------------------------

	MouseWindowEvent::MouseWindowEvent(uint64 win, RegionState::Code regionState, const Vector2i& position) : Event(win), m_Position(position), m_State(regionState)
	{
	}

	MouseWindowEvent::MouseWindowEvent(const MouseWindowEvent& e) : Event(e.GetWindow()), m_Position(e.m_Position), m_State(e.m_State)
	{
	}

	std::string MouseWindowEvent::ToString() const
	{
		return FORMATTER.Format("MouseWindowEvent {Window ID : {0}, RegionState : {1}, Position : {2}}", m_WindowID, RegionState::ToString(m_State), m_Position);
	}

	Vector2i MouseWindowEvent::GetPosition() const
	{
		return this->m_Position;
	}

	RegionState::Code MouseWindowEvent::GetRegionState() const
	{
		return this->m_State;
	}

}    // namespace nkentseu
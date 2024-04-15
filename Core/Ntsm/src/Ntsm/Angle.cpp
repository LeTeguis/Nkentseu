//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/11/2024 at 10:11:58 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Angle.h"
#include "Utils.h"

namespace nkentseu {

	Angle::Angle() : m_Degree(0.0) {}
	Angle::Angle(float32 degree) : m_Degree(degree){}

	Angle::Angle(const Angle& angle) : m_Degree(angle.m_Degree)
	{
	}

	float32 Angle::Deg() const {
		return m_Degree;
	}

	float32 Angle::Rad() const {
		static float32 radiant = (maths::Pi() * m_Degree) / 180.0f;
		static float32 degree = m_Degree;

		if (m_Degree != degree) {
			degree = m_Degree;
			radiant = (maths::Pi() * m_Degree) / 180.0f;
		}

		return radiant;
	}

	void Angle::Deg(float32 deg) {
		m_Degree = deg;
	}

	void Angle::Rad(float32 rad) {
		m_Degree = 180.0f * rad / maths::Pi();
	}

	Angle::operator float32 () {
		return m_Degree;
	}

	Angle& Angle::operator = (const Angle& a) {
		this->m_Degree = a.m_Degree;
		return *this;
	}

	Angle& Angle::operator = (const float32& a) {
		this->m_Degree = a;
		return *this;
	}

	std::string Angle::ToString() const {
		std::stringstream ss;
		ss << m_Degree << "_deg";
		return ss.str();
	}

	Angle Angle::operator-() const {
		return Angle(-m_Degree);
	}

	Angle& Angle::operator+=(const Angle& other) {
		this->m_Degree += other.m_Degree;
		return *this;
	}

	Angle& Angle::operator-=(const Angle& other) {
		this->m_Degree -= other.m_Degree;
		return *this;
	}

	Angle& Angle::operator*=(const Angle& other) {
		this->m_Degree *= other.m_Degree;
		return *this;
	}

	Angle& Angle::operator/=(const Angle& other) {
		this->m_Degree /= other.m_Degree;
		return *this;
	}

	Angle Angle::operator + (const Angle& other) {
		return Angle(other.m_Degree + this->m_Degree);
	}

	Angle Angle::operator - (const Angle& other) {
		return Angle(this->m_Degree - other.m_Degree);
	}

	Angle Angle::operator / (const Angle& other) {
		return Angle(this->m_Degree / other.m_Degree);
	}

	Angle Angle::FromRadian(float32 rad) {
		return Angle(180.0f * rad / maths::Pi());
	}

}    // namespace nkentseu
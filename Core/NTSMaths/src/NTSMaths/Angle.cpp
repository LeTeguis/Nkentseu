//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/11/2024 at 10:11:58 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Angle.h"
#include "Utils.h"

namespace nkentseu {

	namespace maths
	{
		Angle::Angle() : m_Degree(0.0) {}
		Angle::Angle(float32 degree) : m_Degree(Clamp(degree)) {}

		Angle::Angle(const Angle& angle) : m_Degree(Clamp(angle.m_Degree))
		{
		}

		float32 Angle::Deg() const {
			return m_Degree;
		}

		float32 Angle::Rad() const {
			static float32 radiant = m_Degree * DEG2RAD;
			static float32 degree = m_Degree;

			if (m_Degree != degree) {
				degree = m_Degree;
				radiant = m_Degree * DEG2RAD;
			}

			return radiant;
		}

		void Angle::Deg(float32 deg) {
			m_Degree = Clamp(deg);
		}

		void Angle::Rad(float32 rad) {
			m_Degree = Clamp(rad * RAD2DEG);
		}

		Angle::operator float32 () {
			return m_Degree;
		}

		Angle& Angle::operator = (const Angle& a) {
			this->m_Degree = a.m_Degree;
			return *this;
		}

		Angle& Angle::operator = (const float32& a) {
			this->m_Degree = Clamp(a);
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
			this->m_Degree = Clamp(this->m_Degree + other.m_Degree);
			return *this;
		}

		Angle& Angle::operator-=(const Angle& other) {
			this->m_Degree = Clamp(this->m_Degree - other.m_Degree);
			return *this;
		}

		Angle& Angle::operator*=(const Angle& other) {
			this->m_Degree = Clamp(this->m_Degree * other.m_Degree);
			return *this;
		}

		Angle& Angle::operator/=(const Angle& other) {
			this->m_Degree = Clamp(this->m_Degree / other.m_Degree);
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
			return Angle(180.0f * rad / maths::Pi);
		}

		float32 Angle::Clamp(float32 deg)
		{
			while (deg < -180.0f) {
				deg += 360.0f;
			}

			while (deg >= 180.0f) {
				deg -= 360.0f;
			}

			if (deg == 180.0f) deg = -180.0f;

			return deg;
		}

		bool operator==(const Angle& l, const Angle& r) {
			return maths::Abs(l.m_Degree - r.m_Degree) <= maths::Epsilon;
		}
}
}    // namespace nkentseu
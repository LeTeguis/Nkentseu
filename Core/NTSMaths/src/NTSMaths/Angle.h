//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/11/2024 at 10:11:58 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_ANGLE_H__
#define __NKENTSEU_ANGLE_H__

#pragma once

#include "NTSCore/System.h"

namespace nkentseu {
	namespace maths
	{
		class NKENTSEU_API Angle
		{
		public:
			Angle();
			Angle(float32 degree);
			Angle(const Angle& angle);

			float32 Deg() const;

			float32 Rad() const;

			void Deg(float32 deg);

			void Rad(float32 rad);

			operator float32 ();

			Angle& operator = (const Angle& a);

			Angle& operator = (const float32& a);

			friend std::ostream& operator<<(std::ostream& os, const Angle& e) {
				return os << e.ToString();
			}

			std::string ToString() const;

			friend std::string ToString(const Angle& angle) {
				return angle.ToString();
			}

			friend bool operator==(const Angle& l, const Angle& r);

			friend bool operator!=(const Angle& l, const Angle& r) {
				return !(l == r);
			}

			// Pre-increment and decrement (optional)
			Angle& operator++() { // Pre-increment
				m_Degree = Clamp(this->m_Degree + 1);
				return *this;
			}
			Angle& operator--() { // Pre-decrement
				m_Degree = Clamp(this->m_Degree - 1);
				return *this;
			}

			// Post-increment and decrement
			Angle operator++(int) { // Post-increment
				Angle temp = *this;
				m_Degree = Clamp(this->m_Degree + 1);
				return temp;
			}
			Angle operator--(int) { // Post-decrement
				Angle temp = *this;
				m_Degree = Clamp(this->m_Degree - 1);
				return temp;
			}

			Angle operator-() const;

			Angle& operator+=(const Angle& other);

			Angle& operator-=(const Angle& other);

			Angle& operator*=(const Angle& other);

			Angle& operator/=(const Angle& other);

			Angle operator + (const Angle& other);

			friend Angle operator + (const Angle& other1, const Angle& other2) {
				return Angle(other1.m_Degree + other2.m_Degree);
			}

			Angle operator - (const Angle& other);

			friend Angle operator - (const Angle& other1, const Angle& other2) {
				return Angle(other1.m_Degree - other2.m_Degree);
			}

			Angle& operator *=(float32 deg) {
				this->m_Degree = Clamp(this->m_Degree * deg);
				return *this;
			}

			Angle& operator /=(float32 deg) {
				this->m_Degree = Clamp(this->m_Degree / deg);
				return *this;
			}

			Angle& operator +=(float32 deg) {
				this->m_Degree = Clamp(this->m_Degree + deg);
				return *this;
			}

			Angle& operator -=(float32 deg) {
				this->m_Degree = Clamp(this->m_Degree - deg);
				return *this;
			}

			friend Angle operator * (const Angle& other1, const Angle& other2) {
				return Angle(other1.m_Degree * other2.m_Degree);
			}

			friend Angle operator * (const Angle& other1, float32 scale) {
				return Angle(other1.m_Degree * scale);
			}

			friend Angle operator * (float32 scale, const Angle& other1) {
				return Angle(other1.m_Degree * scale);
			}

			Angle operator / (const Angle& other);

			friend Angle operator / (const Angle& other1, const Angle& other2) {
				return Angle(other1.m_Degree / other2.m_Degree);
			}

			friend Angle operator / (float32 other1, const Angle& other2) {
				return Angle(other1 / other2.m_Degree);
			}

			friend Angle operator / (const Angle& other1, float32 other2) {
				return Angle(other1.m_Degree / other2);
			}

			friend Angle operator + (float32 other1, const Angle& other2) {
				return Angle(other1 + other2.m_Degree);
			}

			friend Angle operator + (const Angle& other1, float32 other2) {
				return Angle(other1.m_Degree + other2);
			}

			friend Angle operator - (float32 other1, const Angle& other2) {
				return Angle(other1 - other2.m_Degree);
			}

			friend Angle operator - (const Angle& other1, float32 other2) {
				return Angle(other1.m_Degree - other2);
			}

			static Angle FromRadian(float32 rad);
		private:
			float32 m_Degree;
			float32 Clamp(float32 deg);
		};
	}
} // namespace nkentseu

#endif    // __NKENTSEU_ANGLE_H__
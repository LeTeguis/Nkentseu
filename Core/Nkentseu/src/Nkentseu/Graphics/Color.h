//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 2:55:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_COLOR_H__
#define __NKENTSEU_COLOR_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include <Ntsm/Range.h>

namespace nkentseu {
	struct NKENTSEU_API HSV {
		float32 hue;
		float32 saturation;
		float32 value;

		HSV(float32 hue, float32 saturation, float32 value) : hue(RangeFloat(0.0f, 360.0f).Clamp(hue)), saturation(RangeFloat(0.0f, 100.0f).Clamp(saturation)), value(RangeFloat(0.0f, 100.0f).Clamp(value)) {}
		HSV() : hue(0), saturation(0), value(0) {}
		HSV(const HSV& hsv) : hue(RangeFloat(0.0f, 360.0f).Clamp(hsv.hue)), saturation(RangeFloat(0.0f, 100.0f).Clamp(hsv.saturation)), value(RangeFloat(0.0f, 100.0f).Clamp(hsv.value)) {}
		HSV& operator=(const HSV& hsv) {
			hue = (RangeFloat(0.0f, 360.0f).Clamp(hsv.hue));
			saturation = (RangeFloat(0.0f, 100.0f).Clamp(hsv.saturation));
			value = (RangeFloat(0.0f, 100.0f).Clamp(hsv.value));
			return *this;
		}

		friend HSV operator+(const HSV& hsv1, const HSV& hsv2) {
			return { hsv1.hue + hsv2.hue, hsv1.saturation + hsv2.saturation, hsv1.value + hsv2.value };
		}

		friend HSV operator-(const HSV& hsv1, const HSV& hsv2) {
			return { hsv1.hue - hsv2.hue, hsv1.saturation - hsv2.saturation, hsv1.value - hsv2.value };
		}
	};

	class NKENTSEU_API Color {
	private:
		uint8 m_Red = 0;
		uint8 m_Green = 0;
		uint8 m_Blue = 0;
		uint8 m_Alpha = 255;

		static uint8 ClampValue(float32 value);
	public:
		Color();

		Color(uint8 r, uint8 g, uint8 b);

		Color(uint8 r, uint8 g, uint8 b, uint8 a);

		Color(const std::string& str_color);

		Color(uint32 color);

		Color(const Color& color);

		static Color RGBf(float32 r, float32 g, float32 b);

		static Color RGBAf(float32 r, float32 g, float32 b, float32 a);

		static Color RandomRGB();
		static Color RandomRGBA();
		// HSV to RGB conversion
		static Color FromHSV(HSV hsv);

		Color ToGrayscale() const;
		Color ToGrayscaleWithAlpha() const;

		// RGB to HSV conversion
		HSV ToHSV() const;

		uint8 R() const;
		uint8 G() const;
		uint8 B() const;
		uint8 A() const;

		uint32 ToUint32A();

		uint32 ToUint32();

		void R(uint8 r);
		void G(uint8 g);
		void B(uint8 b);
		void A(uint8 a);

		float32 Rf() const;
		float32 Gf() const;
		float32 Bf() const;
		float32 Af() const;

		void Rf(float32 r);
		void Gf(float32 g);
		void Bf(float32 b);
		void Af(float32 a);

		std::string ToString() const;

		friend std::ostream& operator<<(std::ostream& os, const Color& e) {
			return os << e.ToString();
		}

		friend std::string ToString(const Color& color) {
			return color.ToString();
		}

		void SetHue(float32 hue);
		void SetSaturation(float32 saturation);
		void SetValue(float32 value);
		void IncreaseHue(float32 hue);
		void DecreaseHue(float32 hue);
		void IncreaseSaturation(float32 saturation);
		void DecreaseSaturation(float32 saturation);
		void IncreaseValue(float32 value);
		void DecreaseValue(float32 value);

		Color Darken(float32 amount) const;
		Color Lighten(float32 amount) const;

		bool IsEqual(const Color& color) const {
			return m_Red == color.m_Red && m_Green == color.m_Green && m_Blue == color.m_Blue;
		}
		bool IsEqualWithTransparency(const Color& color) const {
			return m_Red == color.m_Red && m_Green == color.m_Green && m_Blue == color.m_Blue && m_Alpha == color.m_Alpha;
		}

		friend bool operator==(const Color& color1, const Color& color2) {
			return color1.m_Red == color2.m_Red && color1.m_Green == color2.m_Green && color1.m_Blue == color2.m_Blue && color1.m_Alpha == color2.m_Alpha;
		}

		friend bool operator!=(const Color& color1, const Color& color2) { return !(color1 == color2); }

		// Opérations sur les couleurs
		Color operator+(const Color& other) const {
			return Color(m_Red + other.m_Red, m_Green + other.m_Green, m_Blue + other.m_Blue, m_Alpha + other.m_Alpha);
		}

		Color operator*(const Color& other) const {
			return Color(m_Red * other.m_Red, m_Green * other.m_Green, m_Blue * other.m_Blue, m_Alpha * other.m_Alpha);
		}

		Color operator-(const Color& other) const {
			return Color(m_Red - other.m_Red, m_Green - other.m_Green, m_Blue - other.m_Blue, m_Alpha - other.m_Alpha);
		}

		Color operator/(const Color& other) const {
			return Color((other.m_Red != 0) ? m_Red / other.m_Red : m_Red,
				(other.m_Green != 0) ? m_Green / other.m_Green : m_Green,
				(other.m_Blue != 0) ? m_Blue / other.m_Blue : m_Blue,
				(other.m_Alpha != 0) ? m_Alpha / other.m_Alpha : m_Alpha);
		}

		Color operator%(const Color& other) const {
			return Color((other.m_Red != 0) ? m_Red % other.m_Red : m_Red,
				(other.m_Green != 0) ? m_Green % other.m_Green : m_Green,
				(other.m_Blue != 0) ? m_Blue % other.m_Blue : m_Blue,
				(other.m_Alpha != 0) ? m_Alpha % other.m_Alpha : m_Alpha);
		}

		Color operator&(const Color& other) const {
			return Color(m_Red & other.m_Red, m_Green & other.m_Green, m_Blue & other.m_Blue, m_Alpha & other.m_Alpha);
		}

		Color operator|(const Color& other) const {
			return Color(m_Red | other.m_Red, m_Green | other.m_Green, m_Blue | other.m_Blue, m_Alpha | other.m_Alpha);
		}

		Color operator^(const Color& other) const {
			return Color(m_Red ^ other.m_Red, m_Green ^ other.m_Green, m_Blue ^ other.m_Blue, m_Alpha ^ other.m_Alpha);
		}

		static const Color& Transparent;
		static const Color& Blue;
		static const Color& Green;
		static const Color& Red;
		static const Color& Black;
		static const Color& White;
		static const Color& Yellow;
		static const Color& Purple;
		static const Color& Cyan;
		static const Color& Magenta;
		static const Color& Orange;
		static const Color& Pink;
		static const Color& Gray;
		static const Color& DarkGray;
		static const Color& Lime;
		static const Color& Teal;
		static const Color& Brown;
		static const Color& SaddleBrown;
		static const Color& Olive;
		static const Color& Maroon;
		static const Color& Navy;
		static const Color& Indigo;
		static const Color& Turquoise;
		static const Color& Silver;
		static const Color& Gold;
		static const Color& SkyBlue;
		static const Color& ForestGreen;
		static const Color& SteelBlue;
		static const Color& DarkSlateGray;
		static const Color& Chocolate;
		static const Color& HotPink;
		static const Color& SlateBlue;
		static const Color& RoyalBlue;
		static const Color& Tomato;
		static const Color& MediumSeaGreen;
		static const Color& DarkOrange;
		static const Color& MediumPurple;
		static const Color& CornflowerBlue;
		static const Color& DarkGoldenrod;
		static const Color& DodgerBlue;
		static const Color& MediumVioletRed;
		static const Color& Peru;
		static const Color& MediumAquamarine;
		static const Color& DarkTurquoise;
		static const Color& MediumSlateBlue;
		static const Color& YellowGreen;
		static const Color& LightCoral;
		static const Color& DarkSlateBlue;
		static const Color& DarkOliveGreen;
		static const Color& Firebrick;
		static const Color& MediumOrchid;
		static const Color& RosyBrown;
		static const Color& DarkCyan;
		static const Color& CadetBlue;
		static const Color& PaleVioletRed;
		static const Color& DeepPink;
		static const Color& LawnGreen;
		static const Color& MediumSpringGreen;
		static const Color& MediumTurquoise;
		static const Color& PaleGreen;
		static const Color& DarkKhaki;
		static const Color& MediumBlue;
		static const Color& MidnightBlue;
		static const Color& NavajoWhite;
		static const Color& DarkSalmon;
		static const Color& MediumCoral;
		static const Color& DefaultBackground;
		static const Color& CharcoalBlack;
		static const Color& SlateGray;
		static const Color& SkyBlueRef;
		static const Color& DuckBlue;

		static const Color& FromName(const std::string& name);
	};
} // namespace nkentseu

#endif    // __NKENTSEU_COLOR_H__
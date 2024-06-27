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
#include <Ntsm/Vector/Vector4.h>

namespace nkentseu {
	struct NKENTSEU_API HSV {
		float32 hue;
		float32 saturation;
		float32 value;

		HSV(float32 hue, float32 saturation, float32 value) : hue(maths::RangeFloat(0.0f, 360.0f).Clamp(hue)), saturation(maths::RangeFloat(0.0f, 100.0f).Clamp(saturation)), value(maths::RangeFloat(0.0f, 100.0f).Clamp(value)) {}
		HSV() : hue(0), saturation(0), value(0) {}
		HSV(const HSV& hsv) : hue(maths::RangeFloat(0.0f, 360.0f).Clamp(hsv.hue)), saturation(maths::RangeFloat(0.0f, 100.0f).Clamp(hsv.saturation)), value(maths::RangeFloat(0.0f, 100.0f).Clamp(hsv.value)) {}
		HSV& operator=(const HSV& hsv) {
			hue = (maths::RangeFloat(0.0f, 360.0f).Clamp(hsv.hue));
			saturation = (maths::RangeFloat(0.0f, 100.0f).Clamp(hsv.saturation));
			value = (maths::RangeFloat(0.0f, 100.0f).Clamp(hsv.value));
			return *this;
		}

		friend HSV operator+(const HSV& hsv1, const HSV& hsv2) {
			return { hsv1.hue + hsv2.hue, hsv1.saturation + hsv2.saturation, hsv1.value + hsv2.value };
		}

		friend HSV operator-(const HSV& hsv1, const HSV& hsv2) {
			return { hsv1.hue - hsv2.hue, hsv1.saturation - hsv2.saturation, hsv1.value - hsv2.value };
		}
	};

	struct NKENTSEU_API Color {
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

		Color(const maths::Vector4f& color);
		Color(const maths::Vector3f& color);

		operator maths::Vector4f () const {
			return maths::Vector4f(m_Red / 255.0f, m_Green / 255.0f, m_Blue / 255.0f, m_Alpha / 255.0f);
		}

		operator maths::Vector4f () {
			return maths::Vector4f(m_Red / 255.0f, m_Green / 255.0f, m_Blue / 255.0f, m_Alpha / 255.0f);
		}

		operator const maths::Vector3f& () const {
			return maths::Vector3f(m_Red / 255.0f, m_Green / 255.0f, m_Blue / 255.0f);
		}

		operator maths::Vector3f () {
			return maths::Vector3f(m_Red / 255.0f, m_Green / 255.0f, m_Blue / 255.0f);
		}

		const maths::Vector4f& Vec4f() {
			return maths::Vector4f(m_Red / 255.0f, m_Green / 255.0f, m_Blue / 255.0f, m_Alpha / 255.0f);
		}

		const maths::Vector3f& Vec3f() {
			return maths::Vector3f(m_Red / 255.0f, m_Green / 255.0f, m_Blue / 255.0f);
		}

		const maths::Vector4f& Vec4f() const {
			return maths::Vector4f(m_Red / 255.0f, m_Green / 255.0f, m_Blue / 255.0f, m_Alpha / 255.0f);
		}

		const maths::Vector3f& Vec3f() const {
			return maths::Vector3f(m_Red / 255.0f, m_Green / 255.0f, m_Blue / 255.0f);
		}

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

		//static const Color BlueTransparent;

		/*static const Color& Transparent;
		static const Color& Blue;
		static const Color& Green;
		static const Color& Red;*/
		//static const Color Black;
		/*static const Color& White;
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
		static const Color& DuckBlue;*/

		/*#define Transparent RGBAf(0.0f, 0.0f, 0.0f, 1.0f)
		#define Blue RGBf(0.0f, 0.0f, 1.0f)
		#define Green RGBf(0.0f, 1.0f, 0.0f)
		#define Red RGBf(1.0f, 0.0f, 0.0f)
		#define Black RGBf(0.0f, 0.0f, 0.0f)
		#define White RGBf(1.0f, 1.0f, 1.0f)
		#define Yellow RGBf(1.0f, 1.0f, 0.0f)
		#define Purple RGBf(0.5f, 0.0f, 0.5f)
		#define Cyan RGBf(0.0f, 1.0f, 1.0f)
		#define Magenta RGBf(1.0f, 0.0f, 1.0f)
		#define Orange RGBf(1.0f, 0.5f, 0.0f)
		#define Pink RGBf(1.0f, 0.75f, 0.8f)
		#define Gray RGBf(0.5f, 0.5f, 0.5f)
		#define DarkGray RGBf(0.1f, 0.1f, 0.1f)
		#define Lime RGBf(0.75f, 1.0f, 0.0f)
		#define Teal RGBf(0.0f, 0.5f, 0.5f)
		#define Brown RGBf(0.65f, 0.16f, 0.16f)
		#define Olive RGBf(0.5f, 0.5f, 0.0f)
		#define Maroon RGBf(0.5f, 0.0f, 0.0f)
		#define Navy RGBf(0.0f, 0.0f, 0.5f)
		#define Indigo RGBf(0.29f, 0.0f, 0.51f)
		#define Turquoise RGBf(0.25f, 0.88f, 0.82f)
		#define Silver RGBf(0.75f, 0.75f, 0.75f)
		#define Gold RGBf(1.0f, 0.84f, 0.0f)
		#define SkyBlue RGBf(0.53f, 0.81f, 0.98f)
		#define ForestGreen RGBf(0.13f, 0.55f, 0.13f)
		#define SteelBlue RGBf(0.27f, 0.51f, 0.71f)
		#define DarkSlateGray RGBf(0.18f, 0.31f, 0.31f)
		#define Chocolate RGBf(0.82f, 0.41f, 0.12f)
		#define HotPink RGBf(1.0f, 0.41f, 0.71f)
		#define SlateBlue RGBf(0.42f, 0.35f, 0.8f)
		#define RoyalBlue RGBf(0.25f, 0.41f, 0.88f)
		#define Tomato RGBf(1.0f, 0.39f, 0.28f)
		#define MediumSeaGreen RGBf(0.24f, 0.7f, 0.44f)
		#define DarkOrange RGBf(1.0f, 0.55f, 0.0f)
		#define MediumPurple RGBf(0.58f, 0.44f, 0.86f)
		#define CornflowerBlue RGBf(0.39f, 0.58f, 0.93f)
		#define DarkGoldenrod RGBf(0.72f, 0.53f, 0.04f)
		#define DodgerBlue RGBf(0.12f, 0.56f, 1.0f)
		#define MediumVioletRed RGBf(0.78f, 0.08f, 0.52f)
		#define Peru RGBf(0.8f, 0.52f, 0.25f)
		#define MediumAquamarine RGBf(0.4f, 0.8f, 0.67f)
		#define DarkTurquoise RGBf(0.0f, 0.81f, 0.82f)
		#define MediumSlateBlue RGBf(0.48f, 0.41f, 0.93f)
		#define SaddleBrown RGBf(0.55f, 0.27f, 0.07f)
		#define YellowGreen RGBf(0.6f, 0.8f, 0.2f)
		#define LightCoral RGBf(0.94f, 0.5f, 0.5f)
		#define DarkSlateBlue RGBf(0.28f, 0.24f, 0.55f)
		#define DarkOliveGreen RGBf(0.33f, 0.42f, 0.18f)
		#define Firebrick RGBf(0.7f, 0.13f, 0.13f)
		#define MediumOrchid RGBf(0.73f, 0.33f, 0.83f)
		#define RosyBrown RGBf(0.74f, 0.56f, 0.56f)
		#define DarkCyan RGBf(0.0f, 0.55f, 0.55f)
		#define CadetBlue RGBf(0.37f, 0.62f, 0.63f)
		#define PaleVioletRed RGBf(0.86f, 0.44f, 0.58f)
		#define DeepPink RGBf(1.0f, 0.08f, 0.58f)
		#define LawnGreen RGBf(0.49f, 0.99f, 0.0f)
		#define MediumSpringGreen RGBf(0.0f, 0.98f, 0.6f)
		#define MediumTurquoise RGBf(0.28f, 0.82f, 0.8f)
		#define PaleGreen RGBf(0.6f, 0.98f, 0.6f)
		#define DarkKhaki RGBf(0.74f, 0.72f, 0.42f)
		#define MediumBlue RGBf(0.0f, 0.0f, 0.8f)
		#define NavajoWhite RGBf(1.0f, 0.87f, 0.68f)
		#define DarkSalmon RGBf(0.91f, 0.59f, 0.48f)
		#define MediumCoral RGBf(0.81f, 0.36f, 0.36f)
		#define DefaultBackground RGBf(0.0f, 162.0f / 255.0f, 232.0f / 255.0f)
		#define MidnightBlue RGBf(25.0f / 255.0f, 25.0f / 255.0f, 112.0f / 255.0f)
		#define CharcoalBlack RGBf(31.0f / 255.0f, 31.0f / 255.0f, 31.0f / 255.0f)
		#define SlateGray RGBf(46.0f / 255.0f, 46.0f / 255.0f, 46.0f / 255.0f)
		#define SkyBlueRef RGBf(50.0f / 255.0f, 130.0f / 255.0f, 246.0f / 255.0f)
		#define DuckBlue RGBf(0.0f / 255.0f, 162.0f / 255.0f, 232.0f / 255.0f)*/

		static const Color& Transparent();
		static const Color& Blue();
		static const Color& Green();
		static const Color& Red();
		static const Color& Black();
		static const Color& White();
		static const Color& Yellow();
		static const Color& Purple();
		static const Color& Cyan();
		static const Color& Magenta();
		static const Color& Orange();
		static const Color& Pink();
		static const Color& Gray();
		static const Color& DarkGray();
		static const Color& Lime();
		static const Color& Teal();
		static const Color& Brown();
		static const Color& SaddleBrown();
		static const Color& Olive();
		static const Color& Maroon();
		static const Color& Navy();
		static const Color& Indigo();
		static const Color& Turquoise();
		static const Color& Silver();
		static const Color& Gold();
		static const Color& SkyBlue();
		static const Color& ForestGreen();
		static const Color& SteelBlue();
		static const Color& DarkSlateGray();
		static const Color& Chocolate();
		static const Color& HotPink();
		static const Color& SlateBlue();
		static const Color& RoyalBlue();
		static const Color& Tomato();
		static const Color& MediumSeaGreen();
		static const Color& DarkOrange();
		static const Color& MediumPurple();
		static const Color& CornflowerBlue();
		static const Color& DarkGoldenrod();
		static const Color& DodgerBlue();
		static const Color& MediumVioletRed();
		static const Color& Peru();
		static const Color& MediumAquamarine();
		static const Color& DarkTurquoise();
		static const Color& MediumSlateBlue();
		static const Color& YellowGreen();
		static const Color& LightCoral();
		static const Color& DarkSlateBlue();
		static const Color& DarkOliveGreen();
		static const Color& Firebrick();
		static const Color& MediumOrchid();
		static const Color& RosyBrown();
		static const Color& DarkCyan();
		static const Color& CadetBlue();
		static const Color& PaleVioletRed();
		static const Color& DeepPink();
		static const Color& LawnGreen();
		static const Color& MediumSpringGreen();
		static const Color& MediumTurquoise();
		static const Color& PaleGreen();
		static const Color& DarkKhaki();
		static const Color& MediumBlue();
		static const Color& MidnightBlue();
		static const Color& NavajoWhite();
		static const Color& DarkSalmon();
		static const Color& MediumCoral();
		static const Color& DefaultBackground();
		static const Color& CharcoalBlack();
		static const Color& SlateGray();
		static const Color& SkyBlueRef();
		static const Color& DuckBlue();

		static const Color& FromName(const std::string& name);
	};
} // namespace nkentseu

#endif    // __NKENTSEU_COLOR_H__
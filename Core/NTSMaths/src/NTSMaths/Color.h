//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-25 at 09:56:31 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __COLOR_H__
#define __COLOR_H__

#pragma once

#include <NTSCore/System.h>
#include "Vector/Vector3.h"
#include "Vector/Vector4.h"
#include "Range.h"

namespace nkentseu {
    namespace maths {

		class NKENTSEU_API HSV {
		public:
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

		class NKENTSEU_API Color : public Vector4f {
		public:
			#define COLOR_UINT8(value)	(maths::Min(value / 255.0f, 1.0f))

			Color();
			Color(uint8 r, uint8 g, uint8 b);
			Color(uint8 r, uint8 g, uint8 b, uint8 a);
			Color(const std::string& str_color);
			Color(uint32 color);
			Color(const Color& color);
			Color(const Vector4f& color);
			Color(const Vector3f& color);

			static float32 to_float(uint8 value) {
				return maths::Min(value / 255.0f, 1.0f);
			}

			Color& operator=(const Color& color)  {
				this->r = maths::Min(color.r, 1.0f);
				this->g = maths::Min(color.g, 1.0f);
				this->b = maths::Min(color.b, 1.0f);
				this->a = maths::Min(color.a, 1.0f);
				return *this;
			}

			Color& operator=(Color& color)  {
				this->r = maths::Min(color.r, 1.0f);
				this->g = maths::Min(color.g, 1.0f);
				this->b = maths::Min(color.b, 1.0f);
				this->a = maths::Min(color.a, 1.0f);
				return *this;
			}

			Color& operator=(uint32 color) {
				r = maths::Min(static_cast<uint8>(color & 0xFF) / 255.0f, 1.0f);
				g = maths::Min(static_cast<uint8>((color >> 8) & 0xFF) / 255.0f, 1.0f);
				b = maths::Min(static_cast<uint8>((color >> 16) & 0xFF) / 255.0f, 1.0f);
				a = maths::Min(static_cast<uint8>((color >> 24) & 0xFF) / 255.0f, 1.0f);
				return *this;
			}

			operator Vector4f() const {
				return Vector4f(r, g, b, a);
			}

			operator Vector4f() {
				return Vector4f(r, g, b, a);
			}

			operator const Vector3f& () const {
				return Vector3f(r, g, b);
			}

			operator Vector3f() {
				return Vector3f(r, g, b);
			}

			operator uint32() const {
				return ToUint32A();
			}

			operator uint32() {
				return ToUint32A();
			}

			// HSV to RGB conversion
			static const Color& FromHSV(HSV hsv);

			const Color& ToGrayscale() const;
			const Color& ToGrayscaleWithAlpha() const;

			// RGB to HSV conversion
			HSV ToHSV() const;

			uint32 ToUint32A() const;

			uint32 ToUint32() const;

			const Color& Darken(float32 amount) const;
			const Color& Lighten(float32 amount) const;

			Color operator%(const Color& other) const {
				return Color(
								maths::Min(maths::FMod(r, other.r), 1.0f) * 255, maths::Min(maths::FMod(g, other.g), 1.0f) * 255,
								maths::Min(maths::FMod(b, other.b), 1.0f) * 255, maths::Min(maths::FMod(a, other.a), 1.0f) * 255
							);
			}

			Color operator&(const Color& other) const {
				uint32 other_ = other.ToUint32A();
				uint32 this_ = ToUint32A();
				return Color(this_ & other_);
			}

			Color operator|(const Color& other) const {
				uint32 other_ = other.ToUint32A();
				uint32 this_ = ToUint32A();
				return Color(this_ | other_);
			}

			Color operator^(const Color& other) const {
				uint32 other_ = other.ToUint32A();
				uint32 this_ = ToUint32A();
				return Color(this_ ^ other_);
			}

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

			bool IsEqual(const Color& color) const {
				return r == color.r && g == color.g && b == color.b && a == color.a;
			}

			static const Color& RandomRGB();
			static const Color& RandomRGBA();
			static const Color& RGBf(float32 r, float32 g, float32 b);
			static const Color& RGBAf(float32 r, float32 g, float32 b, float32 a);

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
    }

}  //  nkentseu

#endif  // __COLOR_H__!
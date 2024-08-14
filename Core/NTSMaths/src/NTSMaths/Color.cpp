//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-25 at 09:56:31 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Color.h"
#include <NTSLogger/Formatter.h>
#include "Random.h"

namespace nkentseu {

    namespace maths {

		const Color& Color::Transparent() { static const Color color = Color::RGBAf(0.0f, 0.0f, 0.0f, 1.0f); return color; }
		const Color& Color::Blue() { static const Color color = Color::RGBf(0.0f, 0.0f, 1.0f); return color; }
		const Color& Color::Green() { static const Color color = Color::RGBf(0.0f, 1.0f, 0.0f); return color; }
		const Color& Color::Red() { static const Color color = Color::RGBf(1.0f, 0.0f, 0.0f); return color; }
		const Color& Color::Black() { static const Color color = Color::RGBf(0.0f, 0.0f, 0.0f); return color; }
		const Color& Color::White() { static const Color color = Color::RGBf(1.0f, 1.0f, 1.0f); return color; }
		const Color& Color::Yellow() { static const Color color = Color::RGBf(1.0f, 1.0f, 0.0f); return color; }
		const Color& Color::Purple() { static const Color color = Color::RGBf(0.5f, 0.0f, 0.5f); return color; }
		const Color& Color::Cyan() { static const Color color = Color::RGBf(0.0f, 1.0f, 1.0f); return color; }
		const Color& Color::Magenta() { static const Color color = Color::RGBf(1.0f, 0.0f, 1.0f); return color; }
		const Color& Color::Orange() { static const Color color = Color::RGBf(1.0f, 0.5f, 0.0f); return color; }
		const Color& Color::Pink() { static const Color color = Color::RGBf(1.0f, 0.75f, 0.8f); return color; }
		const Color& Color::Gray() { static const Color color = Color::RGBf(0.5f, 0.5f, 0.5f); return color; }
		const Color& Color::DarkGray() { static const Color color = Color::RGBf(0.1f, 0.1f, 0.1f); return color; }
		const Color& Color::Lime() { static const Color color = Color::RGBf(0.75f, 1.0f, 0.0f); return color; }
		const Color& Color::Teal() { static const Color color = Color::RGBf(0.0f, 0.5f, 0.5f); return color; }
		const Color& Color::Brown() { static const Color color = Color::RGBf(0.65f, 0.16f, 0.16f); return color; }
		const Color& Color::Olive() { static const Color color = Color::RGBf(0.5f, 0.5f, 0.0f); return color; }
		const Color& Color::Maroon() { static const Color color = Color::RGBf(0.5f, 0.0f, 0.0f); return color; }
		const Color& Color::Navy() { static const Color color = Color::RGBf(0.0f, 0.0f, 0.5f); return color; }
		const Color& Color::Indigo() { static const Color color = Color::RGBf(0.29f, 0.0f, 0.51f); return color; }
		const Color& Color::Turquoise() { static const Color color = Color::RGBf(0.25f, 0.88f, 0.82f); return color; }
		const Color& Color::Silver() { static const Color color = Color::RGBf(0.75f, 0.75f, 0.75f); return color; }
		const Color& Color::Gold() { static const Color color = Color::RGBf(1.0f, 0.84f, 0.0f); return color; }
		const Color& Color::SkyBlue() { static const Color color = Color::RGBf(0.53f, 0.81f, 0.98f); return color; }
		const Color& Color::ForestGreen() { static const Color color = Color::RGBf(0.13f, 0.55f, 0.13f); return color; }
		const Color& Color::SteelBlue() { static const Color color = Color::RGBf(0.27f, 0.51f, 0.71f); return color; }
		const Color& Color::DarkSlateGray() { static const Color color = Color::RGBf(0.18f, 0.31f, 0.31f); return color; }
		const Color& Color::Chocolate() { static const Color color = Color::RGBf(0.82f, 0.41f, 0.12f); return color; }
		const Color& Color::HotPink() { static const Color color = Color::RGBf(1.0f, 0.41f, 0.71f); return color; }
		const Color& Color::SlateBlue() { static const Color color = Color::RGBf(0.42f, 0.35f, 0.8f); return color; }
		const Color& Color::RoyalBlue() { static const Color color = Color::RGBf(0.25f, 0.41f, 0.88f); return color; }
		const Color& Color::Tomato() { static const Color color = Color::RGBf(1.0f, 0.39f, 0.28f); return color; }
		const Color& Color::MediumSeaGreen() { static const Color color = Color::RGBf(0.24f, 0.7f, 0.44f); return color; }
		const Color& Color::DarkOrange() { static const Color color = Color::RGBf(1.0f, 0.55f, 0.0f); return color; }
		const Color& Color::MediumPurple() { static const Color color = Color::RGBf(0.58f, 0.44f, 0.86f); return color; }
		const Color& Color::CornflowerBlue() { static const Color color = Color::RGBf(0.39f, 0.58f, 0.93f); return color; }
		const Color& Color::DarkGoldenrod() { static const Color color = Color::RGBf(0.72f, 0.53f, 0.04f); return color; }
		const Color& Color::DodgerBlue() { static const Color color = Color::RGBf(0.12f, 0.56f, 1.0f); return color; }
		const Color& Color::MediumVioletRed() { static const Color color = Color::RGBf(0.78f, 0.08f, 0.52f); return color; }
		const Color& Color::Peru() { static const Color color = Color::RGBf(0.8f, 0.52f, 0.25f); return color; }
		const Color& Color::MediumAquamarine() { static const Color color = Color::RGBf(0.4f, 0.8f, 0.67f); return color; }
		const Color& Color::DarkTurquoise() { static const Color color = Color::RGBf(0.0f, 0.81f, 0.82f); return color; }
		const Color& Color::MediumSlateBlue() { static const Color color = Color::RGBf(0.48f, 0.41f, 0.93f); return color; }
		const Color& Color::SaddleBrown() { static const Color color = Color::RGBf(0.55f, 0.27f, 0.07f); return color; }
		const Color& Color::YellowGreen() { static const Color color = Color::RGBf(0.6f, 0.8f, 0.2f); return color; }
		const Color& Color::LightCoral() { static const Color color = Color::RGBf(0.94f, 0.5f, 0.5f); return color; }
		const Color& Color::DarkSlateBlue() { static const Color color = Color::RGBf(0.28f, 0.24f, 0.55f); return color; }
		const Color& Color::DarkOliveGreen() { static const Color color = Color::RGBf(0.33f, 0.42f, 0.18f); return color; }
		const Color& Color::Firebrick() { static const Color color = Color::RGBf(0.7f, 0.13f, 0.13f); return color; }
		const Color& Color::MediumOrchid() { static const Color color = Color::RGBf(0.73f, 0.33f, 0.83f); return color; }
		const Color& Color::RosyBrown() { static const Color color = Color::RGBf(0.74f, 0.56f, 0.56f); return color; }
		const Color& Color::DarkCyan() { static const Color color = Color::RGBf(0.0f, 0.55f, 0.55f); return color; }
		const Color& Color::CadetBlue() { static const Color color = Color::RGBf(0.37f, 0.62f, 0.63f); return color; }
		const Color& Color::PaleVioletRed() { static const Color color = Color::RGBf(0.86f, 0.44f, 0.58f); return color; }
		const Color& Color::DeepPink() { static const Color color = Color::RGBf(1.0f, 0.08f, 0.58f); return color; }
		const Color& Color::LawnGreen() { static const Color color = Color::RGBf(0.49f, 0.99f, 0.0f); return color; }
		const Color& Color::MediumSpringGreen() { static const Color color = Color::RGBf(0.0f, 0.98f, 0.6f); return color; }
		const Color& Color::MediumTurquoise() { static const Color color = Color::RGBf(0.28f, 0.82f, 0.8f); return color; }
		const Color& Color::PaleGreen() { static const Color color = Color::RGBf(0.6f, 0.98f, 0.6f); return color; }
		const Color& Color::DarkKhaki() { static const Color color = Color::RGBf(0.74f, 0.72f, 0.42f); return color; }
		const Color& Color::MediumBlue() { static const Color color = Color::RGBf(0.0f, 0.0f, 0.8f); return color; }
		const Color& Color::NavajoWhite() { static const Color color = Color::RGBf(1.0f, 0.87f, 0.68f); return color; }
		const Color& Color::DarkSalmon() { static const Color color = Color::RGBf(0.91f, 0.59f, 0.48f); return color; }
		const Color& Color::MediumCoral() { static const Color color = Color::RGBf(0.81f, 0.36f, 0.36f); return color; }
		const Color& Color::DefaultBackground() { static const Color color = Color(0, 162, 232); return color; }
		const Color& Color::MidnightBlue() { static const Color color = Color(25, 25, 112); return color; }
		const Color& Color::CharcoalBlack() { static const Color color = Color(31, 31, 31); return color; }
		const Color& Color::SlateGray() { static const Color color = Color(46, 46, 46); return color; }
		const Color& Color::SkyBlueRef() { static const Color color = Color(50, 130, 246); return color; }
		const Color& Color::DuckBlue() { static const Color color = Color(0, 162, 232); return color; }

		Color::Color() : Vector4f() {
		}

		Color::Color(uint8 r, uint8 g, uint8 b) : Vector4f((r % 256) / 255.0f, (g % 256) / 255.0f, (b % 256) / 255.0f, (1.0f)) {}

		Color::Color(uint8 r, uint8 g, uint8 b, uint8 a) : Vector4f((r % 256) / 255.0f, (g % 256) / 255.0f, (b % 256) / 255.0f, (a % 256) / 255.0f) {}

		Color::Color(const std::string& str_color) : Vector4f(){}

		Color::Color(uint32 color) {
			a = maths::Min(static_cast<uint8>(color & 0xFF) / 255.0f, 1.0f);
			b = maths::Min(static_cast<uint8>((color >> 8) & 0xFF) / 255.0f, 1.0f);
			g = maths::Min(static_cast<uint8>((color >> 16) & 0xFF) / 255.0f, 1.0f);
			r = maths::Min(static_cast<uint8>((color >> 24) & 0xFF) / 255.0f, 1.0f);
		}

		Color::Color(const Color& color) : Vector4f(maths::Min(color.r, 1.0f), maths::Min(color.g, 1.0f), maths::Min(color.b, 1.0f), maths::Min(color.a, 1.0f)) {}

		Color::Color(const maths::Vector4f& color) : Vector4f(maths::Min(color.r, 1.0f), maths::Min(color.g, 1.0f), maths::Min(color.b, 1.0f), maths::Min(color.a, 1.0f))
		{
		}

		Color::Color(const maths::Vector3f& color) : Vector4f(maths::Min(color.r, 1.0f), maths::Min(color.g, 1.0f), maths::Min(color.b, 1.0f), 1.0f)
		{
		}

		const Color& Color::ToGrayscale() const {
			// Formule simple de moyenne
			// uint8 gray = (r + g + b) / 3;

			// Formule prenant en compte la perception humaine
			uint8 gray = static_cast<uint8>(0.299 * r + 0.587 * g + 0.114 * b);
			return Color(gray, gray, gray, 255);
		}

		const Color& Color::FromHSV(HSV hsv) {
			// Assuming HSV values are in range [0, 360], [0, 100], [0, 100]
			float32 h = hsv.hue / 360.0f;
			float32 s = hsv.saturation / 100.0f;
			float32 v = hsv.value / 100.0f;

			int32 i = static_cast<int32>(h * 6.0f);
			float32 f = h * 6.0f - i;
			float32 p = v * (1.0f - s);
			float32 q = v * (1.0f - f * s);
			float32 t = v * (1.0f - (1.0f - f) * s);

			float32 r = 0.0f, g = 0.0f, b = 0.0f;
			switch (i % 6) {
			case 0:
				r = v;
				g = t;
				b = p;
				break;
			case 1:
				r = q;
				g = v;
				b = p;
				break;
			case 2:
				r = p;
				g = v;
				b = t;
				break;
			case 3:
				r = p;
				g = q;
				b = v;
				break;
			case 4:
				r = t;
				g = p;
				b = v;
				break;
			case 5:
				r = v;
				g = p;
				b = q;
				break;
			}

			// Return a new Color object with RGB values
			return Color(r * 255.0f, g * 255.0f, b * 255.0f);
		}

		const Color& Color::ToGrayscaleWithAlpha() const {
			// Formule simple de moyenne
			// uint8 gray = (r + g + b) / 3;

			// Formule prenant en compte la perception humaine
			uint8 gray = static_cast<uint8>(0.299 * r + 0.587 * g + 0.114 * b);
			return Color(gray, gray, gray, a);
		}

		void Color::SetHue(float32 hue) {
			HSV hsv = ToHSV();
			hsv.hue = RangeFloat(0.0f, 360.0f).Clamp(hue);

			*this = FromHSV(hsv);
		}

		HSV Color::ToHSV() const {
			float32 max = maths::Max<float32>({ r, g, b });
			float32 min = maths::Min<float32>({ r, g, b });

			float32 h, s, v;
			v = max; // Value (brightness)

			if (max == 0.0f) {
				// Achromatic (black, gray, white)
				h = 0.0f; // Undefined hue
				s = 0.0f; // No saturation
			}
			else {
				float32 d = max - min;
				s = d / max; // Saturation

				if (max == r) {
					h = (g - b) / d + (g < b ? 6.0f : 0.0f);
				}
				else if (max == g) {
					h = (b - r) / d + 2.0f;
				}
				else {
					h = (r - g) / d + 4.0f;
				}

				h /= 6.0f; // Hue in range [0, 1]
			}

			// Return a new Color object with HSV values
			return { h * 360.0f, s * 100.0f, v * 100.0f };
		}

		const Color& Color::Darken(float32 amount) const {
			float32 r = this->r * (1.0f - amount);
			float32 g = this->g * (1.0f - amount);
			float32 b = this->b * (1.0f - amount);
			return Color(r * 255, g * 255, b * 255, a * 255);
		}

		const Color& Color::Lighten(float32 amount) const {
			float32 r = maths::Min(this->r + amount * (1.0f - this->r), 1.0f);
			float32 g = maths::Min(this->g + amount * (1.0f - this->g), 1.0f);
			float32 b = maths::Min(this->b + amount * (1.0f - this->b), 1.0f);
			return Color(r * 255, g * 255, b * 255, a * 255);
		}

		uint32 Color::ToUint32A() const {
			uint32 r_ = r * 255;
			uint32 g_ = g * 255;
			uint32 b_ = b * 255;
			uint32 a_ = a * 255;
			return ((a_) | ((b_) << 8) | ((g_) << 16) | ((r_) << 24));
		}

		uint32 Color::ToUint32() const {
			uint32 r_ = r * 255;
			uint32 g_ = g * 255;
			uint32 b_ = b * 255;
			uint32 a_ = 255;
			return ((a_) | ((b_) << 8) | ((g_) << 16) | ((r_) << 24));
		}

		void Color::SetSaturation(float32 saturation) {
			HSV hsv = ToHSV();
			hsv.saturation = RangeFloat(0.0f, 100.0f).Clamp(saturation);

			*this = FromHSV(hsv);
		}

		void Color::SetValue(float32 value) {
			HSV hsv = ToHSV();
			hsv.value = RangeFloat(0.0f, 100.0f).Clamp(value);

			*this = FromHSV(hsv);
		}

		void Color::IncreaseHue(float32 hue) {
			HSV hsv = ToHSV();
			hsv.hue = RangeFloat(0.0f, 360.0f).Clamp(hue + hsv.hue);

			*this = FromHSV(hsv);
		}

		void Color::DecreaseHue(float32 hue) {
			HSV hsv = ToHSV();
			hsv.hue = RangeFloat(0.0f, 360.0f).Clamp(-hue + hsv.hue);

			*this = FromHSV(hsv);
		}

		void Color::IncreaseSaturation(float32 saturation) {
			HSV hsv = ToHSV();
			hsv.saturation = RangeFloat(0.0f, 100.0f).Clamp(hsv.saturation + saturation);

			*this = FromHSV(hsv);
		}

		void Color::DecreaseSaturation(float32 saturation) {
			HSV hsv = ToHSV();
			hsv.saturation = RangeFloat(0.0f, 100.0f).Clamp(hsv.saturation - saturation);

			*this = FromHSV(hsv);
		}

		void Color::IncreaseValue(float32 value) {
			HSV hsv = ToHSV();
			hsv.value = RangeFloat(0.0f, 100.0f).Clamp(hsv.value + value);

			*this = FromHSV(hsv);
		}

		void Color::DecreaseValue(float32 value) {
			HSV hsv = ToHSV();
			hsv.value = RangeFloat(0.0f, 100.0f).Clamp(hsv.value - value);

			*this = FromHSV(hsv);
		}

		const Color& Color::RandomRGB() {
			return Color(Random.NextUInt32(256), Random.NextUInt32(256), Random.NextUInt32(256));
		}

		const Color& Color::RandomRGBA() {
			return Color(Random.NextUInt32(256), Random.NextUInt32(256), Random.NextUInt32(256), Random.NextUInt32(256));
		}

		const Color& Color::RGBf(float32 r, float32 g, float32 b) {
			return Color(r * 255, g * 255, b * 255, 255);
		}

		const Color& Color::RGBAf(float32 r, float32 g, float32 b, float32 a) {
			return Color(r * 255, g * 255, b * 255, a * 255);
		}

		std::string Color::ToString() const {
			return FORMATTER.Format("({0}, {1}, {2}, {3})", (int32)(r * 255.0f), (int32)(g * 255.0f), (int32)(b * 255.0f), (int32)(a * 255.0f));
		}

		const Color& Color::FromName(const std::string& name) {
			std::string name_traiter(name);

			if (name_traiter == "Transparent") return Transparent();
			if (name_traiter == "Blue") return Blue();
			if (name_traiter == "Green") return Green();
			if (name_traiter == "Red") return Red();
			if (name_traiter == "Black") return Black();
			if (name_traiter == "White") return White();
			if (name_traiter == "Yellow") return Yellow();
			if (name_traiter == "Purple") return Purple();
			if (name_traiter == "Cyan") return Cyan();
			if (name_traiter == "Magenta") return Magenta();
			if (name_traiter == "Orange") return Orange();
			if (name_traiter == "Pink") return Pink();
			if (name_traiter == "Gray") return Gray();
			if (name_traiter == "DarkGray") return DarkGray();
			if (name_traiter == "Lime") return Lime();
			if (name_traiter == "Teal") return Teal();
			if (name_traiter == "Brown") return Brown();
			if (name_traiter == "SaddleBrown") return SaddleBrown();
			if (name_traiter == "Olive") return Olive();
			if (name_traiter == "Maroon") return Maroon();
			if (name_traiter == "Navy") return Navy();
			if (name_traiter == "Indigo") return Indigo();
			if (name_traiter == "Turquoise") return Turquoise();
			if (name_traiter == "Silver") return Silver();
			if (name_traiter == "Gold") return Gold();
			if (name_traiter == "SkyBlue") return SkyBlue();
			if (name_traiter == "ForestGreen") return ForestGreen();
			if (name_traiter == "SteelBlue") return SteelBlue();
			if (name_traiter == "DarkSlateGray") return DarkSlateGray();
			if (name_traiter == "Chocolate") return Chocolate();
			if (name_traiter == "HotPink") return HotPink();
			if (name_traiter == "SlateBlue") return SlateBlue();
			if (name_traiter == "RoyalBlue") return RoyalBlue();
			if (name_traiter == "Tomato") return Tomato();
			if (name_traiter == "MediumSeaGreen") return MediumSeaGreen();
			if (name_traiter == "DarkOrange") return DarkOrange();
			if (name_traiter == "MediumPurple") return MediumPurple();
			if (name_traiter == "CornflowerBlue") return CornflowerBlue();
			if (name_traiter == "DarkGoldenrod") return DarkGoldenrod();
			if (name_traiter == "DodgerBlue") return DodgerBlue();
			if (name_traiter == "Peru") return Peru();
			if (name_traiter == "MediumAquamarine") return MediumAquamarine();
			if (name_traiter == "DarkTurquoise") return DarkTurquoise();
			if (name_traiter == "MediumSlateBlue") return MediumSlateBlue();
			if (name_traiter == "YellowGreen") return YellowGreen();
			if (name_traiter == "LightCoral") return LightCoral();
			if (name_traiter == "DarkSlateBlue") return DarkSlateBlue();
			if (name_traiter == "DarkOliveGreen") return DarkOliveGreen();
			if (name_traiter == "Firebrick") return Firebrick();
			if (name_traiter == "MediumOrchid") return MediumOrchid();
			if (name_traiter == "RosyBrown") return RosyBrown();
			if (name_traiter == "DarkCyan") return DarkCyan();
			if (name_traiter == "CadetBlue") return CadetBlue();
			if (name_traiter == "PaleVioletRed") return PaleVioletRed();
			if (name_traiter == "DeepPink") return DeepPink();
			if (name_traiter == "LawnGreen") return LawnGreen();
			if (name_traiter == "MediumSpringGreen") return MediumSpringGreen();
			if (name_traiter == "MediumTurquoise") return MediumTurquoise();
			if (name_traiter == "PaleGreen") return PaleGreen();
			if (name_traiter == "DarkKhaki") return DarkKhaki();
			if (name_traiter == "MediumBlue") return MediumBlue();
			if (name_traiter == "MediumVioletRed") return MediumVioletRed();
			if (name_traiter == "MidnightBlue") return MidnightBlue();
			if (name_traiter == "NavajoWhite") return NavajoWhite();
			if (name_traiter == "DarkSalmon") return DarkSalmon();
			if (name_traiter == "MediumCoral") return MediumCoral();
			if (name_traiter == "DefaultBackground") return DefaultBackground();
			if (name_traiter == "CharcoalBlack") return CharcoalBlack();
			if (name_traiter == "SlateGray") return SlateGray();
			if (name_traiter == "SkyBlueRef") return SkyBlueRef();
			if (name_traiter == "DuckBlue") return DuckBlue();
			static const Color& default_value = Color();
			return default_value;
		}
    }

}  //  nkentseu
//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 2:55:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Color.h"

#include <Logger/Formatter.h>
#include <Ntsm/Random.h>

namespace nkentseu {
	const Color Color::Transparent = Color::RGBAf(0.0f, 0.0f, 0.0f, 1.0f);
	const Color Color::Blue = Color::RGBf(0.0f, 0.0f, 1.0f);
	const Color Color::Green = Color::RGBf(0.0f, 1.0f, 0.0f);
	const Color Color::Red = Color::RGBf(1.0f, 0.0f, 0.0f);
	const Color Color::Black = Color::RGBf(0.0f, 0.0f, 0.0f);
	const Color Color::White = Color::RGBf(1.0f, 1.0f, 1.0f);
	const Color Color::Yellow = Color::RGBf(1.0f, 1.0f, 0.0f);
	const Color Color::Purple = Color::RGBf(0.5f, 0.0f, 0.5f);
	const Color Color::Cyan = Color::RGBf(0.0f, 1.0f, 1.0f);
	const Color Color::Magenta = Color::RGBf(1.0f, 0.0f, 1.0f);
	const Color Color::Orange = Color::RGBf(1.0f, 0.5f, 0.0f);
	const Color Color::Pink = Color::RGBf(1.0f, 0.75f, 0.8f);
	const Color Color::Gray = Color::RGBf(0.5f, 0.5f, 0.5f);
	const Color Color::DarkGray = Color::RGBf(0.1f, 0.1f, 0.1f);
	const Color Color::Lime = Color::RGBf(0.75f, 1.0f, 0.0f);
	const Color Color::Teal = Color::RGBf(0.0f, 0.5f, 0.5f);
	const Color Color::Brown = Color::RGBf(0.65f, 0.16f, 0.16f);
	const Color Color::Olive = Color::RGBf(0.5f, 0.5f, 0.0f);
	const Color Color::Maroon = Color::RGBf(0.5f, 0.0f, 0.0f);
	const Color Color::Navy = Color::RGBf(0.0f, 0.0f, 0.5f);
	const Color Color::Indigo = Color::RGBf(0.29f, 0.0f, 0.51f);
	const Color Color::Turquoise = Color::RGBf(0.25f, 0.88f, 0.82f);
	const Color Color::Silver = Color::RGBf(0.75f, 0.75f, 0.75f);
	const Color Color::Gold = Color::RGBf(1.0f, 0.84f, 0.0f);
	const Color Color::SkyBlue = Color::RGBf(0.53f, 0.81f, 0.98f);
	const Color Color::ForestGreen = Color::RGBf(0.13f, 0.55f, 0.13f);
	const Color Color::SteelBlue = Color::RGBf(0.27f, 0.51f, 0.71f);
	const Color Color::DarkSlateGray = Color::RGBf(0.18f, 0.31f, 0.31f);
	const Color Color::Chocolate = Color::RGBf(0.82f, 0.41f, 0.12f);
	const Color Color::HotPink = Color::RGBf(1.0f, 0.41f, 0.71f);
	const Color Color::SlateBlue = Color::RGBf(0.42f, 0.35f, 0.8f);
	const Color Color::RoyalBlue = Color::RGBf(0.25f, 0.41f, 0.88f);
	const Color Color::Tomato = Color::RGBf(1.0f, 0.39f, 0.28f);
	const Color Color::MediumSeaGreen = Color::RGBf(0.24f, 0.7f, 0.44f);
	const Color Color::DarkOrange = Color::RGBf(1.0f, 0.55f, 0.0f);
	const Color Color::MediumPurple = Color::RGBf(0.58f, 0.44f, 0.86f);
	const Color Color::CornflowerBlue = Color::RGBf(0.39f, 0.58f, 0.93f);
	const Color Color::DarkGoldenrod = Color::RGBf(0.72f, 0.53f, 0.04f);
	const Color Color::DodgerBlue = Color::RGBf(0.12f, 0.56f, 1.0f);
	const Color Color::MediumVioletRed = Color::RGBf(0.78f, 0.08f, 0.52f);
	const Color Color::Peru = Color::RGBf(0.8f, 0.52f, 0.25f);
	const Color Color::MediumAquamarine = Color::RGBf(0.4f, 0.8f, 0.67f);
	const Color Color::DarkTurquoise = Color::RGBf(0.0f, 0.81f, 0.82f);
	const Color Color::MediumSlateBlue = Color::RGBf(0.48f, 0.41f, 0.93f);
	const Color Color::SaddleBrown = Color::RGBf(0.55f, 0.27f, 0.07f);
	const Color Color::YellowGreen = Color::RGBf(0.6f, 0.8f, 0.2f);
	const Color Color::LightCoral = Color::RGBf(0.94f, 0.5f, 0.5f);
	const Color Color::DarkSlateBlue = Color::RGBf(0.28f, 0.24f, 0.55f);
	const Color Color::DarkOliveGreen = Color::RGBf(0.33f, 0.42f, 0.18f);
	const Color Color::Firebrick = Color::RGBf(0.7f, 0.13f, 0.13f);
	const Color Color::MediumOrchid = Color::RGBf(0.73f, 0.33f, 0.83f);
	const Color Color::RosyBrown = Color::RGBf(0.74f, 0.56f, 0.56f);
	const Color Color::DarkCyan = Color::RGBf(0.0f, 0.55f, 0.55f);
	const Color Color::CadetBlue = Color::RGBf(0.37f, 0.62f, 0.63f);
	const Color Color::PaleVioletRed = Color::RGBf(0.86f, 0.44f, 0.58f);
	const Color Color::DeepPink = Color::RGBf(1.0f, 0.08f, 0.58f);
	const Color Color::LawnGreen = Color::RGBf(0.49f, 0.99f, 0.0f);
	const Color Color::MediumSpringGreen = Color::RGBf(0.0f, 0.98f, 0.6f);
	const Color Color::MediumTurquoise = Color::RGBf(0.28f, 0.82f, 0.8f);
	const Color Color::PaleGreen = Color::RGBf(0.6f, 0.98f, 0.6f);
	const Color Color::DarkKhaki = Color::RGBf(0.74f, 0.72f, 0.42f);
	const Color Color::MediumBlue = Color::RGBf(0.0f, 0.0f, 0.8f);
	const Color Color::NavajoWhite = Color::RGBf(1.0f, 0.87f, 0.68f);
	const Color Color::DarkSalmon = Color::RGBf(0.91f, 0.59f, 0.48f);
	const Color Color::MediumCoral = Color::RGBf(0.81f, 0.36f, 0.36f);
	//const Color Color::MidnightBlue;

	uint8 Color::ClampValue(float32 value) {
		if (value < 0) return 0;
		else if (value > 1) return 255;
		return (uint8)(value * 255);
	}

	Color::Color() : m_Red(0), m_Green(0), m_Blue(0), m_Alpha(1) {}

	Color::Color(uint8 r, uint8 g, uint8 b) : m_Red(r % 256), m_Green(g % 256), m_Blue(b % 256), m_Alpha(255) {}

	Color::Color(uint8 r, uint8 g, uint8 b, uint8 a) : m_Red(r % 256), m_Green(g % 256), m_Blue(b % 256), m_Alpha(a % 256) {}

	Color::Color(const std::string& str_color) {}

	Color::Color(uint32 color) {
		m_Red = static_cast<uint8>(color & 0xFF);
		m_Green = static_cast<uint8>((color >> 8) & 0xFF);
		m_Blue = static_cast<uint8>((color >> 16) & 0xFF);
		m_Alpha = static_cast<uint8>((color >> 24) & 0xFF);
	}

	Color::Color(const Color& color) : m_Red(color.m_Red), m_Green(color.m_Green), m_Blue(color.m_Blue), m_Alpha(color.m_Alpha) {}

	Color Color::RGBf(float32 r, float32 g, float32 b) {
		return Color(ClampValue(r), ClampValue(g), ClampValue(b), 255);
	}

	Color Color::RGBAf(float32 r, float32 g, float32 b, float32 a) {
		return Color(ClampValue(r), ClampValue(g), ClampValue(b), ClampValue(a));
	}

	Color Color::RandomRGB() {
		return Color(Random.NextUInt32(256), Random.NextUInt32(256), Random.NextUInt32(256));
	}

	Color Color::RandomRGBA() {
		return Color(Random.NextUInt32(256), Random.NextUInt32(256), Random.NextUInt32(256), Random.NextUInt32(256));
	}

	Color Color::ToGrayscale() const {
		// Formule simple de moyenne
		// uint8 gray = (r + g + b) / 3;

		// Formule prenant en compte la perception humaine
		uint8 gray = static_cast<uint8>(0.299 * m_Red + 0.587 * m_Green + 0.114 * m_Blue);
		return Color(gray, gray, gray, 255);
	}

	Color Color::FromHSV(HSV hsv) {
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

	Color Color::ToGrayscaleWithAlpha() const {
		// Formule simple de moyenne
		// uint8 gray = (r + g + b) / 3;

		// Formule prenant en compte la perception humaine
		uint8 gray = static_cast<uint8>(0.299 * m_Red + 0.587 * m_Green + 0.114 * m_Blue);
		return Color(gray, gray, gray, m_Alpha);
	}

	void Color::SetHue(float32 hue) {
		HSV hsv = ToHSV();
		hsv.hue = RangeFloat(0.0f, 360.0f).Clamp(hue);

		*this = FromHSV(hsv);
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

	HSV Color::ToHSV() const {
		float32 r = Rf();
		float32 g = Gf();
		float32 b = Bf();

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

	uint8 Color::R() const { return m_Red; }
	uint8 Color::G() const { return m_Green; }
	uint8 Color::B() const { return m_Blue; }
	uint8 Color::A() const { return m_Alpha; }

	uint32 Color::ToUint32A() {
		return ((m_Red) | ((m_Green) << 8) | ((m_Blue) << 16) | ((m_Alpha) << 24));
	}

	uint32 Color::ToUint32() {
		uint8 a = 255;
		return ((m_Red) | ((m_Green) << 8) | ((m_Blue) << 16) | ((a) << 24));
	}

	void Color::R(uint8 r) { m_Red = r % 256; }
	void Color::G(uint8 g) { m_Green = g % 256; }
	void Color::B(uint8 b) { m_Blue = b % 256; }
	void Color::A(uint8 a) { m_Alpha = a % 256; }

	float32 Color::Rf() const { return m_Red / 255.0f; }
	float32 Color::Gf() const { return m_Green / 255.0f; }
	float32 Color::Bf() const { return m_Blue / 255.0f; }
	float32 Color::Af() const { return m_Alpha / 255.0f; }

	void Color::Rf(float32 r) { m_Red = ClampValue(r); }
	void Color::Gf(float32 g) { m_Green = ClampValue(g); }
	void Color::Bf(float32 b) { m_Blue = ClampValue(b); }
	void Color::Af(float32 a) { m_Alpha = ClampValue(a); }

	std::string Color::ToString() const {
		return FORMATTER.Format("({0}, {1}, {2}, {3})", (int)m_Red, (int)m_Green, (int)m_Blue, (int)m_Alpha);
	}

	Color Color::Darken(float32 amount) const {
		float32 r = Rf() * (1.0f - amount);
		float32 g = Gf() * (1.0f - amount);
		float32 b = Bf() * (1.0f - amount);
		return RGBAf(ClampValue(r), ClampValue(g), ClampValue(b), Af());
	}

	Color Color::Lighten(float32 amount) const {
		float32 r = maths::Min(Rf() + amount * (255.0f - Rf()), 255.0f);
		float32 g = maths::Min(Gf() + amount * (255.0f - Gf()), 255.0f);
		float32 b = maths::Min(Bf() + amount * (255.0f - Bf()), 255.0f);
		return RGBAf(ClampValue(r), ClampValue(g), ClampValue(b), Af());
	}

	const Color& Color::FromName(const std::string& name) {
		std::string name_traiter(name);

		if (name_traiter == "Transparent") return Transparent;
		if (name_traiter == "Blue") return Blue;
		if (name_traiter == "Green") return Green;
		if (name_traiter == "Red") return Red;
		if (name_traiter == "Black") return Black;
		if (name_traiter == "White") return White;
		if (name_traiter == "Yellow") return Yellow;
		if (name_traiter == "Purple") return Purple;
		if (name_traiter == "Cyan") return Cyan;
		if (name_traiter == "Magenta") return Magenta;
		if (name_traiter == "Orange") return Orange;
		if (name_traiter == "Pink") return Pink;
		if (name_traiter == "Gray") return Gray;
		if (name_traiter == "DarkGray") return DarkGray;
		if (name_traiter == "Lime") return Lime;
		if (name_traiter == "Teal") return Teal;
		if (name_traiter == "Brown") return Brown;
		if (name_traiter == "SaddleBrown") return SaddleBrown;
		if (name_traiter == "Olive") return Olive;
		if (name_traiter == "Maroon") return Maroon;
		if (name_traiter == "Navy") return Navy;
		if (name_traiter == "Indigo") return Indigo;
		if (name_traiter == "Turquoise") return Turquoise;
		if (name_traiter == "Silver") return Silver;
		if (name_traiter == "Gold") return Gold;
		if (name_traiter == "SkyBlue") return SkyBlue;
		if (name_traiter == "ForestGreen") return ForestGreen;
		if (name_traiter == "SteelBlue") return SteelBlue;
		if (name_traiter == "DarkSlateGray") return DarkSlateGray;
		if (name_traiter == "Chocolate") return Chocolate;
		if (name_traiter == "HotPink") return HotPink;
		if (name_traiter == "SlateBlue") return SlateBlue;
		if (name_traiter == "RoyalBlue") return RoyalBlue;
		if (name_traiter == "Tomato") return Tomato;
		if (name_traiter == "MediumSeaGreen") return MediumSeaGreen;
		if (name_traiter == "DarkOrange") return DarkOrange;
		if (name_traiter == "MediumPurple") return MediumPurple;
		if (name_traiter == "CornflowerBlue") return CornflowerBlue;
		if (name_traiter == "DarkGoldenrod") return DarkGoldenrod;
		if (name_traiter == "DodgerBlue") return DodgerBlue;
		if (name_traiter == "Peru") return Peru;
		if (name_traiter == "MediumAquamarine") return MediumAquamarine;
		if (name_traiter == "DarkTurquoise") return DarkTurquoise;
		if (name_traiter == "MediumSlateBlue") return MediumSlateBlue;
		if (name_traiter == "YellowGreen") return YellowGreen;
		if (name_traiter == "LightCoral") return LightCoral;
		if (name_traiter == "DarkSlateBlue") return DarkSlateBlue;
		if (name_traiter == "DarkOliveGreen") return DarkOliveGreen;
		if (name_traiter == "Firebrick") return Firebrick;
		if (name_traiter == "MediumOrchid") return MediumOrchid;
		if (name_traiter == "RosyBrown") return RosyBrown;
		if (name_traiter == "DarkCyan") return DarkCyan;
		if (name_traiter == "CadetBlue") return CadetBlue;
		if (name_traiter == "PaleVioletRed") return PaleVioletRed;
		if (name_traiter == "DeepPink") return DeepPink;
		if (name_traiter == "LawnGreen") return LawnGreen;
		if (name_traiter == "MediumSpringGreen") return MediumSpringGreen;
		if (name_traiter == "MediumTurquoise") return MediumTurquoise;
		if (name_traiter == "PaleGreen") return PaleGreen;
		if (name_traiter == "DarkKhaki") return DarkKhaki;
		if (name_traiter == "MediumBlue") return MediumBlue;
		if (name_traiter == "MediumVioletRed") return MediumVioletRed;
		//if (name_traiter == "MidnightBlue") return MidnightBlue;
		if (name_traiter == "NavajoWhite") return NavajoWhite;
		if (name_traiter == "DarkSalmon") return DarkSalmon;
		if (name_traiter == "MediumCoral") return MediumCoral;
		static const Color default_value;
		return default_value;
	}

}    // namespace nkentseu
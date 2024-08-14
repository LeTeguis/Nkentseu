//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:16:36 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_RECTANGLE_H__
#define __NKENTSEU_RECTANGLE_H__

#pragma once

#include "NTSCore/System.h" // Inclure le fichier d'en-tête System/System.h
#include <NTSMaths/Vector/Vector2.h> // Inclure le fichier d'en-tête Vector2f.h depuis le répertoire Ntsm/Vector
#include <NTSMaths/Shapes/Segment.h>

namespace nkentseu {

	namespace maths
	{
		// Définition de la classe Rectangle
		class NKENTSEU_API Rectangle {
		public:

			Vector2f corner{}; // Coin supérieur gauche du rectangle
			Vector2f size{}; // Taille du rectangle

			// Constructeurs
			Rectangle();
			Rectangle(const Vector2f& c, const Vector2f& r);
			Rectangle(float32 x, float32 y, float32 w, float32 h);
			Rectangle(const Vector2f& c, float32 w, float32 h);
			Rectangle(float32 x, float32 y, const Vector2f& r);
			// Surcharge de l'opérateur d'assignation
			Rectangle operator=(const Rectangle& other);

			// Surcharge de l'opérateur de flux de sortie
			friend std::ostream& operator<<(std::ostream& os, const Rectangle& e) { // Surcharge de l'opérateur << pour l'affichage d'un Rectangle
				return os << e.ToString(); // Renvoyer le résultat de l'appel à la méthode ToString() du Rectangle
			}

			// Méthode pour obtenir une représentation sous forme de chaîne de caractères du Rectangle
			std::string ToString() const;

			friend std::string ToString(const Rectangle& r) {
				return r.ToString();
			}

			// Méthodes membres de la classe Rectangle (non implémentées dans ce contexte)
			Vector2f Clamp(const Vector2f& p);
			Vector2f Corner(int nr);
			bool SeparatingAxis(const Segment& axis);
			Rectangle Enlarge(const Vector2f& p);
			Rectangle Enlarge(const Rectangle& extender);
			static Rectangle AABB(const Rectangle* rectangles, int count);
			Vector2f Center();

		};

		template <typename T>
		class NKENTSEU_API Rect {
		public:
			union {
				struct { union { T x; T left; }; union { T y; T top; }; T width; T height; };
				struct { Vector2<T> position; Vector2<T> size; };
			};

			Rect() : position(T(0), T(0)), size(T(0), T(0)) {}
			Rect(T x, T y, T width, T height) : position(x, y), size(width, height) {}
			Rect(const Vector2<T>& position, const Vector2<T>& size) : position(position), size(size) {}
			Rect(const Rect& rect) : position(rect.position), size(rect.size) {}

			template <typename U>
			Rect(const Vector2<U>& position, const Vector2<U>& size) : position(position), size(size) {}
			template <typename U>
			Rect(const Rect<U>& rect) : position((Vector2<T>)rect.position), size((Vector2<T>)rect.size) {}

			//~RectT() {}

			Rect& operator=(const Rect& rect) {
				this->x = rect.x;
				this->y = rect.y;
				this->width = rect.width;
				this->height = rect.height;

				return *this;
			}

			Vector2<T> GetCenter() const {
				return Vector2(x + width / 2, y + height / 2);
			}

			T GetCenterX() const {
				return x + width / 2;
			}

			T GetCenterY() const {
				return y + height / 2;
			}

			friend bool operator==(const Rect& left, const Rect& right) {
				return left.x == right.x && left.y == right.y && left.width == right.width && left.height == right.height;
			}

			friend bool operator!=(const Rect& left, const Rect& right) {
				return !(left == right);
			}
		};

		using IntRect = Rect<int32>;
		using FloatRect = Rect<float32>;
	}
} // namespace nkentseu

#endif  // __NKENTSEU_RECTANGLE_H__ // Fin de la définition de la classe Rectangle
//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:16:36 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_RECTANGLE_H__
#define __NKENTSEU_RECTANGLE_H__

#pragma once

#include "System/System.h" // Inclure le fichier d'en-tête System/System.h
#include <Ntsm/Vector/Vector2.h> // Inclure le fichier d'en-tête Vector2.h depuis le répertoire Ntsm/Vector
#include <Ntsm/Shapes/Segment.h>

namespace nkentseu {

	namespace maths
	{
		// Définition de la classe Rectangle
		class NKENTSEU_API Rectangle {
		public:

			Vector2 corner{}; // Coin supérieur gauche du rectangle
			Vector2 size{}; // Taille du rectangle

			// Constructeurs
			Rectangle();
			Rectangle(const Vector2& c, const Vector2& r);
			Rectangle(float32 x, float32 y, float32 w, float32 h);
			Rectangle(const Vector2& c, float32 w, float32 h);
			Rectangle(float32 x, float32 y, const Vector2& r);
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
			Vector2 Clamp(const Vector2& p);
			Vector2 Corner(int nr);
			bool SeparatingAxis(const Segment& axis);
			Rectangle Enlarge(const Vector2& p);
			Rectangle Enlarge(const Rectangle& extender);
			static Rectangle AABB(const Rectangle* rectangles, int count);
			Vector2 Center();

		};

		using Rect = Rectangle; // Alias pour la classe Rectangle


		template <typename T>
		class NKENTSEU_API RectT {
		public:
			union {
				struct { T x; T y; T width; T height; };
				struct { Vector2T<T> position; Vector2T<T> size; };
			};

			RectT() : position(T(0), T(0)), size(T(0), T(0)) {}
			RectT(const Vector2T<T>& position, const Vector2T<T>& size) : position(position), size(size) {}
			RectT(const RectT& rect) : position(rect.position), size(rect.size) {}

			template <typename U>
			RectT(const Vector2T<U>& position, const Vector2T<U>& size) : position(position), size(size) {}
			template <typename U>
			RectT(const RectT<U>& rect) : position((Vector2T<T>)rect.position), size((Vector2T<T>)rect.size) {}

			RectT& operator=(const RectT& rect) {
				this->x = rect.x;
				this->y = rect.y;
				this->width = rect.width;
				this->height = rect.height;

				return *this;
			}

			Vector2T<T> GetCenter() const {
				return Vector2T(x + width / 2, y + height / 2);
			}

			T GetCenterX() const {
				return x + width / 2;
			}

			T GetCenterY() const {
				return y + height / 2;
			}
		};

		using IntRect = RectT<int32>;
		using FloatRect = RectT<float32>;
	}
} // namespace nkentseu

#endif  // __NKENTSEU_RECTANGLE_H__ // Fin de la définition de la classe Rectangle
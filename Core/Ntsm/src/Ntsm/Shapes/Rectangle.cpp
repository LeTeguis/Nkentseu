//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:16:36 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Rectangle.h"

namespace nkentseu {

	namespace maths
	{
		// Constructeurs
		Rectangle::Rectangle() : corner(0.0f), size(1) {} // Constructeur par défaut initialisant le coin supérieur gauche à l'origine (0, 0) et la taille à (1, 1)
		Rectangle::Rectangle(const Vector2f& c, const Vector2f& r) : corner(c), size(r) {} // Constructeur initialisant le coin supérieur gauche et la taille avec des vecteurs donnés
		Rectangle::Rectangle(float32 x, float32 y, float32 w, float32 h) : corner(x, y), size(w, h) {} // Constructeur initialisant le coin supérieur gauche avec des coordonnées et la taille avec des dimensions données
		Rectangle::Rectangle(const Vector2f& c, float32 w, float32 h) : corner(c), size(w, h) {} // Constructeur initialisant le coin supérieur gauche avec un vecteur et la taille avec des dimensions données
		Rectangle::Rectangle(float32 x, float32 y, const Vector2f& r) : corner(x, y), size(r) {} // Constructeur initialisant le coin supérieur gauche avec des coordonnées et la taille avec un vecteur donné

		// Surcharge de l'opérateur d'assignation
		Rectangle Rectangle::operator=(const Rectangle& other) { // Opérateur d'assignation pour affecter un Rectangle à un autre Rectangle
			if (this->corner != other.corner && this->size != other.size) { // Vérifier si les coins supérieurs gauches et les tailles des deux rectangles sont différents
				this->corner = other.corner; // Affecter le coin supérieur gauche du rectangle 'other' à celui-ci
				this->size = other.size; // Affecter la taille du rectangle 'other' à celui-ci
			}
			return *this; // Retourner le rectangle après l'assignation
		}

		std::string Rectangle::ToString() const { // Méthode ToString() pour obtenir une représentation sous forme de chaîne de caractères du Rectangle
			std::stringstream ss; // Créer un flux de chaîne de caractères
			ss << "Rect[pos(" << corner.x << ", " << corner.y << "); size(" << size << ")]"; // Ajouter les coordonnées du coin supérieur gauche et la taille du rectangle au flux
			return ss.str(); // Renvoyer la chaîne de caractères résultante
		}

		Vector2f Rectangle::Clamp(const Vector2f& p) {
			Vector2f clamp;
			clamp.x = RangeFloat(corner.x, corner.x + size.x).Clamp(p.x);
			clamp.y = RangeFloat(corner.y, corner.y + size.y).Clamp(p.y);
			return clamp;
		}

		Vector2f Rectangle::Corner(int nr) {
			Vector2f corner_i = corner;
			switch (nr % 4) {
			case 0:
				corner_i.x += size.x;
				break;
			case 1:
				corner_i = (corner + size);
				break;
			case 2:
				corner_i.y += size.y;
			break; default:
				/* corner = r.corner */
				break;
			}
			return corner_i;
		}

		bool Rectangle::SeparatingAxis(const Segment& axis) {
			Segment rEdgeA, rEdgeB;
			RangeFloat axisRange, rEdgeARange, rEdgeBRange, rProjection;
			Vector2f n = (axis.points[0] - axis.points[1]);
			rEdgeA.points[0] = Corner(0);
			rEdgeA.points[1] = Corner(1);
			rEdgeB.points[0] = Corner(2);
			rEdgeB.points[1] = Corner(3);
			rEdgeARange = rEdgeA.Project(n);
			rEdgeBRange = rEdgeB.Project(n);
			rProjection = RangeFloat::Hull(rEdgeARange, rEdgeBRange);
			axisRange = Segment(axis).Project(n);
			return !axisRange.Overlaps(rProjection);
		}

		Rectangle Rectangle::Enlarge(const Vector2f& p) {
			Rectangle enlarged;
			enlarged.corner.x = maths::Min(corner.x, p.x);
			enlarged.corner.y = maths::Min(corner.y, p.y);
			enlarged.size.x = maths::Max(corner.x + size.x, p.x);
			enlarged.size.y = maths::Max(corner.y + size.y, p.y);
			enlarged.size = (enlarged.size - enlarged.corner);
			return enlarged;
		}

		Rectangle Rectangle::Enlarge(const Rectangle& extender) {
			Vector2f maxCorner = (extender.corner + extender.size);
			Rectangle enlarged = Enlarge(maxCorner);

			return enlarged.Enlarge(extender.corner);
		}

		Rectangle Rectangle::AABB(const Rectangle* rectangles, int count) {
			int i; Rectangle h = { {0, 0}, {0, 0} };

			if (0 == count) return h;

			h = rectangles[0];

			for (i = 1; i < count; i++)
				h = h.Enlarge(rectangles[i]);

			return h;
		}

		Vector2f Rectangle::Center() {
			Vector2f halfSize = (size / 2);
			return (corner + halfSize);
		}
	}
}    // namespace nkentseu
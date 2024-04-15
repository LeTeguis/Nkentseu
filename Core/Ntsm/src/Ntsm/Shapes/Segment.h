//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:22:06 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_SEGMENT_H__
#define __NKENTSEU_SEGMENT_H__

#pragma once

#include "System/System.h" // Inclure le fichier d'en-tête System/System.h
#include <Ntsm/Range.h> // Inclure le fichier d'en-tête Range.h depuis le répertoire Ntsm
#include <Ntsm/Vector/Vector2.h>

namespace nkentseu {
	// Définition de la classe Segment
	class NKENTSEU_API Segment {
	public:
		union {
			Vector2 points[2];
			struct {
				float32 x1; float32 y1; // Coordonnées x et y du point A
				float32 x2; float32 y2; // Coordonnées x et y du point B
			};
			float32 ptr[4]; // Tableau de pointeurs vers les coordonnées des points A et B
		};

		// Constructeurs
		Segment() : points{ Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f) } {} // Constructeur par défaut initialisant les points A et B à l'origine (0, 0)
		Segment(const Vector2& point1, const Vector2& point2) : points{ point1, point2 } {} // Constructeur initialisant les points A et B avec des vecteurs donnés
		Segment(float32 x1, float32 y1, float32 x2, float32 y2) : points{ Vector2(x1, y1), Vector2(x2, y2) } {} // Constructeur initialisant les points A et B avec des coordonnées données
		Segment(const Segment& segment) : points{ segment.points[0], segment.points[1] } {} // Constructeur de copie initialisant les points A et B avec ceux d'un autre segment

		// Surcharge de l'opérateur d'assignation
		Segment& operator=(const Segment& other) { // Opérateur d'assignation pour affecter un Segment à un autre Segment
			if (this != &other) { // Vérifier si l'objet est différent de celui à affecter
				points[0] = other.points[0]; // Affecter le point A du segment 'other' à celui-ci
				points[1] = other.points[1]; // Affecter le point B du segment 'other' à celui-ci
			}
			return *this; // Retourner le segment après l'assignation
		}

		// Surcharge de l'opérateur de flux de sortie
		friend std::ostream& operator<<(std::ostream& os, const Segment& e) { // Surcharge de l'opérateur << pour l'affichage d'un Segment
			return os << e.ToString(); // Renvoyer le résultat de l'appel à la méthode ToString() du Segment
		}

		// Méthode pour obtenir une représentation sous forme de chaîne de caractères du Segment
		std::string ToString() const { // Méthode ToString() pour obtenir une représentation sous forme de chaîne de caractères du Segment
			return "Segment[A(" + std::to_string(points[0].x) + ", " + std::to_string(points[0].y) + "); B(" + std::to_string(points[1].x) + ", " + std::to_string(points[1].y) + ")]";
		}

		// Méthodes membres de la classe Segment (non implémentées dans ce contexte)
		RangeFloat Project(const Vector2& onto);
		float32 Len();
		bool Equivalent(const Segment& b);

		// Surcharge des opérateurs de comparaison
		friend bool operator==(const Segment& l, const Segment& r) { // Surcharge de l'opérateur ==
			return (l.points[0] == r.points[0] && l.points[1] == r.points[1]); // Renvoyer vrai si les points A et B des deux segments sont égaux
		}

		friend bool operator!=(const Segment& l, const Segment& r) { // Surcharge de l'opérateur !=
			return !(l == r); // Renvoyer faux si les segments sont égaux, vrai sinon
		}
	};
} // namespace nkentseu

#endif    // __NKENTSEU_SEGMENT_H__ // Fin de la définition de la classe Segment

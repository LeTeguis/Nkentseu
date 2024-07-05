//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-04 at 06:21:01 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __EULER_ANGLE_H__
#define __EULER_ANGLE_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include "Angle.h"

namespace nkentseu {
    
    namespace maths {
        struct NKENTSEU_API EulerAngle {
            Angle pitch;
            Angle yaw;
            Angle roll;

            // Constructeurs
            EulerAngle();
            EulerAngle(const Angle& pitch, const Angle& yaw, const Angle& roll);
            EulerAngle(const EulerAngle& other);

            // Opérateur d'affectation
            EulerAngle& operator=(const EulerAngle& other);

            // Opérateurs d'égalité
            bool operator==(const EulerAngle& other) const;
            bool operator!=(const EulerAngle& other) const;

            // Opérateurs arithmétiques
            EulerAngle operator+(const EulerAngle& other) const;
            EulerAngle operator-(const EulerAngle& other) const;
            EulerAngle& operator+=(const EulerAngle& other);
            EulerAngle& operator-=(const EulerAngle& other);

            // Opérateurs d'incrémentation et de décrémentation
            EulerAngle& operator++();    // Pré-incrémentation
            EulerAngle operator++(int);  // Post-incrémentation
            EulerAngle& operator--();    // Pré-décrémentation
            EulerAngle operator--(int);  // Post-décrémentation

            // Méthode ToString
            std::string ToString() const;

            // Opérateur de flux
            friend std::ostream& operator<<(std::ostream& os, const EulerAngle& e);

            // Méthode ToString friend
            friend std::string ToString(const EulerAngle& v) {
                return v.ToString();
            }
        };
    }

}  //  nkentseu

#endif  // __EULER_ANGLE_H__!
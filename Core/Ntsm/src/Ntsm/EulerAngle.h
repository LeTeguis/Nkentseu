//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-07-04 at 06:21:01 PM AM.
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

            // Op�rateur d'affectation
            EulerAngle& operator=(const EulerAngle& other);

            // Op�rateurs d'�galit�
            bool operator==(const EulerAngle& other) const;
            bool operator!=(const EulerAngle& other) const;

            // Op�rateurs arithm�tiques
            EulerAngle operator+(const EulerAngle& other) const;
            EulerAngle operator-(const EulerAngle& other) const;
            EulerAngle& operator+=(const EulerAngle& other);
            EulerAngle& operator-=(const EulerAngle& other);

            // Op�rateurs d'incr�mentation et de d�cr�mentation
            EulerAngle& operator++();    // Pr�-incr�mentation
            EulerAngle operator++(int);  // Post-incr�mentation
            EulerAngle& operator--();    // Pr�-d�cr�mentation
            EulerAngle operator--(int);  // Post-d�cr�mentation

            // M�thode ToString
            std::string ToString() const;

            // Op�rateur de flux
            friend std::ostream& operator<<(std::ostream& os, const EulerAngle& e);

            // M�thode ToString friend
            friend std::string ToString(const EulerAngle& v) {
                return v.ToString();
            }
        };
    }

}  //  nkentseu

#endif  // __EULER_ANGLE_H__!
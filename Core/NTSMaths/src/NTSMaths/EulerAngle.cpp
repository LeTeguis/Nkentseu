//
// Created by TEUGUIA TADJUIDJE Rodolf Sederis on 2024-07-04 at 06:21:01 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "EulerAngle.h"
#include <NTSLogger/Formatter.h>

namespace nkentseu {
	namespace maths {
        // Constructeurs
        EulerAngle::EulerAngle() : pitch(0.0f), yaw(0.0f), roll(0.0f) {}

        EulerAngle::EulerAngle(const Angle& pitch, const Angle& yaw, const Angle& roll)
            : pitch(pitch), yaw(yaw), roll(roll) {}

        EulerAngle::EulerAngle(const EulerAngle& other)
            : pitch(other.pitch), yaw(other.yaw), roll(other.roll) {}

        // Operateur d'affectation
        EulerAngle& EulerAngle::operator=(const EulerAngle& other) {
            if (this != &other) {
                pitch = other.pitch;
                yaw = other.yaw;
                roll = other.roll;
            }
            return *this;
        }

        // Operateurs d'egalite
        bool EulerAngle::operator==(const EulerAngle& other) const {
            return (pitch == other.pitch) && (yaw == other.yaw) && (roll == other.roll);
        }

        bool EulerAngle::operator!=(const EulerAngle& other) const {
            return !(*this == other);
        }

        // Operateurs arithmetiques
        EulerAngle EulerAngle::operator+(const EulerAngle& other) const {
            return EulerAngle(pitch + other.pitch, yaw + other.yaw, roll + other.roll);
        }

        EulerAngle EulerAngle::operator-(const EulerAngle& other) const {
            return EulerAngle(pitch - other.pitch, yaw - other.yaw, roll - other.roll);
        }

        EulerAngle& EulerAngle::operator+=(const EulerAngle& other) {
            pitch += other.pitch;
            yaw += other.yaw;
            roll += other.roll;
            return *this;
        }

        EulerAngle& EulerAngle::operator-=(const EulerAngle& other) {
            pitch -= other.pitch;
            yaw -= other.yaw;
            roll -= other.roll;
            return *this;
        }

        EulerAngle EulerAngle::operator*(float32 scalar) const
        {
            return EulerAngle(pitch * scalar, yaw * scalar, roll * scalar);
        }

        EulerAngle& EulerAngle::operator*=(float32 scalar)
        {
            pitch *= scalar;
            yaw *= scalar;
            roll *= scalar;
            return *this;
        }

        // Operateurs d'incrementation et de decrementation
        EulerAngle& EulerAngle::operator++() {  // Pre-incrementation
            ++pitch;
            ++yaw;
            ++roll;
            return *this;
        }

        EulerAngle EulerAngle::operator++(int) {  // Post-incrementation
            EulerAngle temp = *this;
            ++(*this);
            return temp;
        }

        EulerAngle& EulerAngle::operator--() {  // Pre-decrementation
            --pitch;
            --yaw;
            --roll;
            return *this;
        }

        EulerAngle EulerAngle::operator--(int) {  // Post-decrementation
            EulerAngle temp = *this;
            --(*this);
            return temp;
        }

        // Methode ToString
        std::string EulerAngle::ToString() const {
            std::stringstream ss;
            ss << "euler(" << pitch.Deg() << ", " << yaw.Deg() << ", " << roll.Deg() << ")";
            return ss.str();
        }

        EulerAngle operator*(float32 scalar, const EulerAngle& other)
        {
            return EulerAngle();
        }

        // Operateur de flux
        std::ostream& operator<<(std::ostream& os, const EulerAngle& e) {
            return os << e.ToString();
        }

	}
}  //  nkentseu
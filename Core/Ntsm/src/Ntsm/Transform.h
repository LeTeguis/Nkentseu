//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 1:35:32 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_TRANSFORM_H__
#define __NKENTSEU_TRANSFORM_H__

#pragma once

#include "System/System.h"
#include <Ntsm/Quaternion/Quaternion.h>
#include <Ntsm/Matrix/Matrix4.h>

namespace nkentseu {

    namespace maths
    {
        class NKENTSEU_API Transform {
        public:
            Vector3f position;
            Quaternion rotation;
            Vector3f scale;

            Transform(const Vector3f& p, const Quaternion& r, const Vector3f& s);
            Transform();
            Transform(const Transform& transform);
            Transform& operator = (const Transform& transform);

            Transform Combine(const Transform& a, const Transform& b);

            Transform Inverse();

            Transform Mix(const Transform& b, float32 t);

            Matrix4f ToMat4();
            Matrix4f mat4();
            static Transform mat4(const Matrix4f& m);

            static Transform FromMat4(const Matrix4f& m);

            Vector3f FormPoint(const Vector3f& b);

            Vector3f FormVector(const Vector3f& b);
        };
    }
} // namespace nkentseu

#endif    // __NKENTSEU_TRANSFORM_H__
//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-14 at 08:48:57 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "View.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    using namespace maths;
    
    // Constructor
    View::View() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    View::~View() {
        // Ajoutez votre code de destructeur ici
    }

    void View::LookAt(const Vector3f& target)
    {
        Vector3f eye = target - m_Position;

        if (eye != Vector3f()) {
            float32 dot = eye.Dot(m_Up);

            if (maths::Abs(dot - 1) < 0.000000001f) {
                m_Up = -m_Direction;
            }
            else if (maths::Abs(dot + 1) < 0.000000001f) {
                m_Up = m_Direction;
            }
            m_Direction = eye;
            NormalizeUp();
        }
    }

    void View::LookAt(float32 xTarget, float32 yTarget, float32 zTarget)
    {
        LookAt(Vector3f(xTarget, yTarget, zTarget));
    }

    void View::NormalizeUp()
    {
        m_Up = m_Direction.Cross(m_Up);
        m_Up = m_Up.Cross(m_Direction).Normalized();
    }

}  //  nkentseu
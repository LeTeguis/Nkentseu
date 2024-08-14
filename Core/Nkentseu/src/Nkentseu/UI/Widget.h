//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-20 at 08:00:41 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __WIDGET_H__
#define __WIDGET_H__

#pragma once

#include <System/System.h>

#include "UIElement.h"
#include "UIAnimation.h"

namespace nkentseu {
    namespace ui {

        // Widget base class
        class NKENTSEU_API Widget : public UIElement {
        public:
            Widget();
            virtual ~Widget();

            // Common properties for all widgets
            void SetEnabled(bool enabled);
            bool IsEnabled() const;

        protected:
            bool enabled;
        };

    }

}  //  nkentseu

#endif  // __WIDGET_H__!
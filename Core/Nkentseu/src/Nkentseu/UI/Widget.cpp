//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-20 at 08:00:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Widget.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    
    namespace ui {
        Widget::Widget() : UIElement(), enabled(true)
        {
        }

        Widget::~Widget()
        {
        }

        void Widget::SetEnabled(bool enabled)
        {
            this->enabled = enabled;
        }

        bool Widget::IsEnabled() const
        {
            return enabled;
        }

    }

}  //  nkentseu
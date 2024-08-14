//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-21 at 05:19:40 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Themes.h"
#include <Logger/Formatter.h>

namespace nkentseu {
	namespace ui {

		UIThemeProperty::UIThemeProperty(UITheme* theme, const std::string& name) : theme(theme), name(name)
		{
		}

		std::string UIThemeProperty::GetName()
		{
			return name;
		}

		UITheme* UIThemeProperty::GetOwner()
		{
			return theme;
		}

		UISkinProperty::UISkinProperty(UISkin* theme, const std::string& name) : theme(theme), name(name)
		{
		}

		std::string UISkinProperty::GetName()
		{
			return name;
		}

		UISkin* UISkinProperty::GetOwner()
		{
			return theme;
		}

	}
}  //  nkentseu
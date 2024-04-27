//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/25/2024 at 8:50:07 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_EVENT_STATE_H__
#define __NKENTSEU_EVENT_STATE_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>

#include <string>

namespace nkentseu {
    // Enum for WindowState
    class NKENTSEU_API WindowState {
    public:
        using Code = uint32;
        
        enum : Code {
            NotDefine = 0,
            Created,
            Closed
        };

        static std::string ToString(const WindowState::Code& e) {
            switch (e) {
            case WindowState::Created: return "Created";
            case WindowState::Closed: return "Closed";
            default: return "Not Defined";
            }
        }
    };

    class NKENTSEU_API RegionState {
    public:
        using Code = uint32;
        
        enum : Code {
            NotDefine = 0,
            Entered,
            Exited
        };

        static std::string ToString(const RegionState::Code& e) {
            switch (e) {
            case RegionState::Entered: return "Entered";
            case RegionState::Exited: return "Exited";
            default: return "Not Defined";
            }
        }
    };

    // Enum for ConnectionState
    class NKENTSEU_API ConnectionState {
    public:
        using Code = uint32;

        enum : Code {
            NotDefined = 0,
            Established,
            Terminated
        };

        static std::string ToString(const ConnectionState::Code& e) {
            switch (e) {
            case ConnectionState::Established: return "Established";
            case ConnectionState::Terminated: return "Terminated";
            default: return "Not Defined";
            }
        }
    };


    // Enum for RisizeState
    class NKENTSEU_API ResizeState {
    public:
        using Code = uint32;
        
        enum : Code {
            NotDefine = 0,
            Expanded,
            Reduced,
            NotChange
        };

        static std::string ToString(const ResizeState::Code& e) {
            switch (e) {
            case ResizeState::Expanded: return "Expanded";
            case ResizeState::Reduced: return "Reduced";
            case ResizeState::NotChange: return "Not Change";
            default: return "Not Defined";
            }
        }
    };

    // Enum for ButtonState
    class NKENTSEU_API ButtonState {
    public:
        using Code = uint32;
        
        enum : Code {
            NotDefine = 0,
            Pressed,
            Released
        };

        static std::string ToString(const ButtonState::Code& e) {
            switch (e) {
            case ButtonState::Pressed: return "Pressed";
            case ButtonState::Released: return "Released";
            default: return "Not Defined";
            }
        }
    };

    // Enum for AxisState
    class NKENTSEU_API AxisState {
    public:
        using Code = uint32;

        enum : Code {
            NotDefine = 0,
            Neutral,
            Positive,
            Negative
        };

        static std::string ToString(const AxisState::Code& e) {
            switch (e) {
            case AxisState::Neutral: return "Neutral";
            case AxisState::Positive: return "Positive";
            case AxisState::Negative: return "Negative";
            default: return "Not Defined";
            }
        }
    };

    // Enum for AxisDirection
    class NKENTSEU_API AxisDirection {
    public:
        using Code = uint32;

        enum : Code {
            NotDefined = 0,
            Vertical,
            Horizontal,
            Depth,
            VerticalHorizontal,
            VerticalDepth,
            HorizontalDepth,
            AllDirection
        };

        static std::string ToString(const AxisDirection::Code& e) {
            switch (e) {
            case AxisDirection::Vertical: return "Vertical";
            case AxisDirection::Horizontal: return "Horizontal";
            case AxisDirection::Depth: return "Depth";
            case AxisDirection::VerticalHorizontal: return "VerticalHorizontal";
            case AxisDirection::VerticalDepth: return "VerticalDepth";
            case AxisDirection::HorizontalDepth: return "HorizontalDepth";
            case AxisDirection::AllDirection: return "AllDirection";
            default: return "Not Defined";
            }
        }
    };

    // Enum for StatusState
    class NKENTSEU_API StatusState {
    public:
        using Code = uint32;

        enum : Code {
            NotDefine = 0,
            Connected,
            Disconnected,
            Undefined
        };

        static std::string ToString(const StatusState::Code& e) {
            switch (e) {
            case StatusState::Connected: return "Connected";
            case StatusState::Disconnected: return "Disconnected";
            case StatusState::Undefined: return "Undefined";
            default: return "Not Defined";
            }
        }
    };

    // Enum for DraggedState
    class NKENTSEU_API DraggedState {
    public:
        using Code = uint32;

        enum : Code {
            NotDefine = 0,
            Dragged,
            Dropped,
            Canceled
        };

        static std::string ToString(const DraggedState::Code& e) {
            switch (e) {
            case DraggedState::Dragged: return "Dragged";
            case DraggedState::Dropped: return "Dropped";
            case DraggedState::Canceled: return "Canceled";
            default: return "Not Defined";
            }
        }
    };

    // Enum for FocusState
    class NKENTSEU_API FocusState {
    public:
        using Code = uint32;

        enum : Code {
            NotDefine = 0,
            Focused,
            Unfocused
        };

        static std::string ToString(const FocusState::Code& e) {
            switch (e) {
            case FocusState::Focused: return "Focused";
            case FocusState::Unfocused: return "Unfocused";
            default: return "Not Defined";
            }
        }
    };

    class NKENTSEU_API ModifierState {
    public:
        bool control;
        bool alt;
        bool shift;
        bool super;

        friend std::ostream& operator<<(std::ostream& os, const ModifierState& e) {
            return os << e.ToString();
        }

        friend std::string ToString(const ModifierState& e) {
            return e.ToString();
        }

        // Constructeur par défaut
        ModifierState();

        // Constructeur avec initialisation
        ModifierState(bool control, bool alt, bool shift, bool meta);

        // Méthode ToString pour convertir l'état en chaîne de caractères
        std::string ToString() const;

        friend bool operator==(const ModifierState& modifierState1, const ModifierState& modifierState2) {
            return  modifierState1.alt == modifierState2.alt && modifierState1.control == modifierState2.control && 
                    modifierState1.super == modifierState2.super && modifierState1.shift == modifierState2.shift;
        }

        friend bool operator!=(const ModifierState& modifierState1, const ModifierState& modifierState2) {
            return !(modifierState1 == modifierState2);
        }
    };
} // namespace nkentseu

#endif    // __NKENTSEU_EVENT_STATE_H__
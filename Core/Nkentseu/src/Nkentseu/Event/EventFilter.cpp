//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:02:35 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "EventFilter.h"


#ifdef NKENTSEU_PLATFORM_WINDOWS
#include "Nkentseu/Platform/Window/Windows/WindowEventInternal.h"
#elif defined(NKENTSEU_PLATFORM_ANDROID)
#include "Nkentseu/Platform/Window/Android/WindowEventInternal.h"
#elif defined(NKENTSEU_PLATFORM_LINUX)
    #ifdef NKENTSEU_LINUX_WIN_API_XCB
    #include "Nkentseu/Platform/Window/Linux/XCB/WindowEventInternal.h"
    #elif defined(NKENTSEU_LINUX_WIN_API_XLIB)
    #include "Nkentseu/Platform/Window/Linux/XLIB/WindowEventInternal.h"
    #endif
#elif defined(NKENTSEU_PLATFORM_IOS)
#include "Nkentseu/Platform/Window/iOS/WindowEventInternal.h"
#elif defined(NKENTSEU_PLATFORM_MACOS)
#include "Nkentseu/Platform/Window/Macos/WindowEventInternal.h"
#elif defined(NKENTSEU_PLATFORM_NDS)
#include "Nkentseu/Platform/Window/NDS/WindowEventInternal.h"
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported"
#endif

#include "Nkentseu/Core/NkentseuLogger.h"

namespace nkentseu {
    EventFilter& EventFilter::Instance() {
        static EventFilter EventFilter;
        WindowEventInternal::GetInstance();
        return EventFilter;
    }

    void EventFilter::Pick() {
        EventInternal.isQueueLocked = true;
        EventInternal.Update();
    }

    bool EventFilter::Pick(Event** event) {
        static bool update_it = true;

        if (update_it) {
            EventInternal.isQueueLocked = false;
            EventInternal.Update();
            update_it = false;
            m_IsPick = false;
        }

        if (event == nullptr) {
            do {
                EventInternal.Pop();
            } while (!EventInternal.Empty());

            update_it = true;
            return false;
        }

        if (m_IsPick) {
            EventInternal.Pop();
            m_IsPick = false;
        }

        if (EventInternal.Empty()) {
            update_it = true;
            return false;
        }

        if (event != nullptr) {
            do {
                *event = EventInternal.Front();
                if (*event == nullptr) {
                    EventInternal.Pop();

                    if (EventInternal.Empty()) {
                        update_it = true;
                        return false;
                    }
                }
            } while (*event == nullptr);
        }

        m_IsPick = true;

        return true;
    }

    /*bool EventFilter::Pick(Event* event) {
        static bool update_it = true;

        if (update_it) {
            EventInternal.isQueueLocked = false;
            EventInternal.Update();
            update_it = false;
            m_IsPick = false;
        }

        if (event == nullptr) {
            do {
                EventInternal.Pop();
            } while (!EventInternal.Empty());

            update_it = true;
            return false;
        }

        if (m_IsPick) {
            EventInternal.Pop();
            m_IsPick = false;
        }

        if (EventInternal.Empty()) {
            update_it = true;
            return false;
        }

        if (event != nullptr) {
            do {
                event = &EventInternal.FrontReference();
                if (event == nullptr) {
                    EventInternal.Pop();

                    if (EventInternal.Empty()) {
                        update_it = true;
                        return false;
                    }
                }
            } while (event == nullptr);
        }

        m_IsPick = true;

        return true;
    }*/

    Event* EventFilter::Picks() {
        static bool update_it = true;

        if (update_it) {
            EventInternal.isQueueLocked = false;
            EventInternal.Update();
            update_it = false;
            m_IsPick = false;
        }

        if (m_IsPick) {
            EventInternal.Pop();
            m_IsPick = false;
        }

        if (EventInternal.Empty()) {
            update_it = true;
            return nullptr;
        }

        Event *event = nullptr;

        do {
            event = EventInternal.Front();
            if (event == nullptr) {
                EventInternal.Pop();

                if (EventInternal.Empty()) {
                    update_it = true;
                    return nullptr;
                }
            } else{
                m_IsPick = true;
                return event;
            }
        } while (event == nullptr);

        return nullptr;
    }

    void EventFilter::TriggerEvent(Event* event)
    {
        if (event != nullptr) {
            EventInternal.TriggerEvent(event);
        }
    }

    void EventFilter::AddObserver(EventObserver observer) {
        if (observer == 0) return;

        auto itr = std::find_if(EventInternal.eventObservers.begin(), EventInternal.eventObservers.end(), [&observer](const EventObserver& c) {
            return &c == &observer;
            });

        if (itr == EventInternal.eventObservers.end()) {
            EventInternal.eventObservers.push_back(observer);
            return;
        }
    }

    void EventFilter::RemoveObserver(const EventObserver& observer) {
        if (observer == 0) return;

        auto itr = std::find_if(EventInternal.eventObservers.begin(), EventInternal.eventObservers.end(), [&observer](const EventObserver& c) {
            return &c == &observer;
            });

        if (itr != EventInternal.eventObservers.end()) {
            EventInternal.eventObservers.erase(itr);
            return;
        }
    }

    Vector2i EventFilter::GetMousePosition() const {
        return EventInternal.mousePosition;
    }

    Vector2i EventFilter::GetMousePositionGlobal() const {
        return EventInternal.globalMousePosition;
    }

    void EventFilter::SetMousePosition(int32 x, int32 y) {
        //SetCursorPos(x, y);
    }

    void EventFilter::SetMousePosition(const Vector2i& pos) {
        SetMousePosition(pos.x, pos.y);
    }
    
}    // namespace nkentseu
//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:02:35 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "EventObservable.h"


#ifdef NKENTSEU_PLATFORM_WINDOWS
#include "Nkentseu/Platform/Window/Windows/WindowEventInternal.h"
#elif defined(NKENTSEU_PLATFORM_ANDROID)
#include "Nkentseu/Platform/Window/Android/WindowEventInternal.h"
#elif defined(NKENTSEU_PLATFORM_LINUX)
#include "Nkentseu/Platform/Window/Linux/WindowEventInternal.h"
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

    std::shared_ptr<EventObservable> EventObservable::Instance() {
        static std::shared_ptr<EventObservable> eventObservable = std::make_shared<EventObservable>();
        WindowEventInternal::GetInstance();
        return eventObservable;
    }

    void EventObservable::Pick() {
        EventInternal->isQueueLocked = true;
        EventInternal->Update();
    }

    bool EventObservable::Pick(Event** event) {
        static bool update_it = true;

        if (update_it) {
            EventInternal->isQueueLocked = false;
            EventInternal->Update();
            update_it = false;
            m_IsPick = false;
        }

        if (event == nullptr) {
            do {
                EventInternal->Pop();
            } while (!EventInternal->Empty());

            update_it = true;
            return false;
        }

        if (m_IsPick) {
            EventInternal->Pop();
            m_IsPick = false;
        }

        if (EventInternal->Empty()) {
            update_it = true;
            return false;
        }

        if (event != nullptr) {
            do {
                *event = EventInternal->Front();
                if (*event == nullptr) {
                    EventInternal->Pop();

                    if (EventInternal->Empty()) {
                        update_it = true;
                        return false;
                    }
                }
            } while (*event == nullptr);
        }

        m_IsPick = true;

        return true;
    }

    void EventObservable::AddObserver(EventObserver observer) {
        if (observer == 0) return;

        auto itr = std::find_if(EventInternal->eventObservers.begin(), EventInternal->eventObservers.end(), [&observer](const EventObserver& c) {
            return &c == &observer;
            });

        if (itr == EventInternal->eventObservers.end()) {
            EventInternal->eventObservers.push_back(observer);
            return;
        }
    }

    void EventObservable::RemoveObserver(const EventObserver& observer) {
        if (observer == 0) return;

        auto itr = std::find_if(EventInternal->eventObservers.begin(), EventInternal->eventObservers.end(), [&observer](const EventObserver& c) {
            return &c == &observer;
            });

        if (itr != EventInternal->eventObservers.end()) {
            EventInternal->eventObservers.erase(itr);
            return;
        }
    }

    Vector2i EventObservable::GetMousePosition() const {
        return EventInternal->mousePosition;
    }

    Vector2i EventObservable::GetMousePositionGlobal() const {
        return EventInternal->globalMousePosition;
    }

    void EventObservable::SetMousePosition(int32 x, int32 y) {
        SetCursorPos(x, y);
    }

    void EventObservable::SetMousePosition(const Vector2i& pos) {
        SetMousePosition(pos.x, pos.y);
    }

}    // namespace nkentseu
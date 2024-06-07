#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>

class Event {
   public:
    Event() = default;
    void wait() {
        std::unique_lock<std::mutex> lock(mutex);
        while (!event_triggered) {
            this_event.wait(lock);
        }
    }

    void notify_all() {
        std::lock_guard<std::mutex> lock(mutex);
        event_triggered = true;
        this_event.notify_all();
    }

   private:
    std::mutex mutex;
    std::condition_variable this_event;
    bool event_triggered = false;  // Este bool solo pasa de false->true.
    // Esta para que si el evento se triggereo antes de esperarlo,
    // el wait no espere al proximo notify_all.

    // No quiero que se copie ni mueva
    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    Event(Event&&) = delete;
    Event& operator=(Event&&) = delete;
};

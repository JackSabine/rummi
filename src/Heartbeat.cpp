#include "Heartbeat.h"

Heartbeat::Heartbeat(std::function<void(void)> func, std::chrono::milliseconds interval)
: func(func), interval(interval) {
    enabled.store(false);
}

Heartbeat::~Heartbeat() {
    stop();
}

void Heartbeat::start() {
    start_stop_lock.lock();

    if (enabled.load()) return;

    enabled.store(true);

    std::thread([this]() {
        while (true) {
            auto x = std::chrono::steady_clock::now() + interval;
            if (!enabled.load()) break;
            func();
            std::this_thread::sleep_until(x);
        }
    }).detach();

    start_stop_lock.unlock();
}

void Heartbeat::stop() {
    start_stop_lock.lock();
    enabled.store(false);
    start_stop_lock.unlock();
}

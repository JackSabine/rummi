#include <functional>
#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>

class Heartbeat {
private:
    std::atomic_bool enabled;
    std::mutex start_stop_lock;

    const std::function<void(void)> func;
    const std::chrono::milliseconds interval;
public:
    Heartbeat(std::function<void(void)> func, std::chrono::milliseconds interval);
    ~Heartbeat();

    void start();
    void stop();
};

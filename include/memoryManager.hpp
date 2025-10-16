#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>

class MemoryManager {
private:
    std::atomic<bool> running{false};
    std::thread monitor_thread;
    size_t limit_mb;
    std::mutex log_mutex;
    std::string log_file = "log/memory.txt";

    size_t get_current_memory_mb();
    void log(const std::string& msg);
    void monitor();

public:
    explicit MemoryManager(size_t limit);
    ~MemoryManager();

    void start();
    void stop();
    void set_limit(size_t new_limit_mb);
    size_t get_limit() const;
};

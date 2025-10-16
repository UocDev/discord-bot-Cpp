#include "memoryManager.hpp"
#include <fstream>
#include <filesystem>
#include <sys/resource.h>
#include <unistd.h>
#include <cstdlib>

MemoryManager::MemoryManager(size_t limit) : limit_mb(limit) {}

MemoryManager::~MemoryManager() {
    stop();
}

size_t MemoryManager::get_current_memory_mb() {
    long rss = 0L;
    FILE* fp = fopen("/proc/self/statm", "r");
    if (fp) {
        if (fscanf(fp, "%*s%ld", &rss) != 1) rss = 0L;
        fclose(fp);
    }
    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
    return (rss * page_size_kb) / 1024;
}

void MemoryManager::log(const std::string& msg) {
    std::lock_guard<std::mutex> lock(log_mutex);

    std::filesystem::create_directories("log");

    std::ofstream out(log_file, std::ios::app);
    if (out) {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        out << "[" << std::ctime(&now);
        out.seekp(-1, std::ios_base::end);
        out << "] " << msg << std::endl;
    }
}

void MemoryManager::monitor() {
    while (running) {
        size_t current = get_current_memory_mb();
        if (current >= limit_mb) {
            log("[memory manager] Memory limit reached: " + std::to_string(current) + " MB (Limit: " + std::to_string(limit_mb) + " MB)");

            limit_mb = static_cast<size_t>(limit_mb * 1.25);
            log("[memory manager] Auto-increased limit to " + std::to_string(limit_mb) + " MB");

            if (current >= limit_mb * 1.2) {
                log("[memory manager] Process exceeded new limit (" + std::to_string(current) + " MB). Sending SIGKILL...");
                kill(getpid(), SIGKILL);
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void MemoryManager::start() {
    if (running) return;
    running = true;
    monitor_thread = std::thread(&MemoryManager::monitor, this);
    log("[memory manager] Memory Manager started. Limit: " + std::to_string(limit_mb) + " MB");
}

void MemoryManager::stop() {
    if (!running) return;
    running = false;
    if (monitor_thread.joinable()) monitor_thread.join();
    log("[memory manager] Memory Manager stopped.");
}

void MemoryManager::set_limit(size_t new_limit_mb) {
    limit_mb = new_limit_mb;
    log("[memory manager] Limit updated to " + std::to_string(limit_mb) + " MB");
}

size_t MemoryManager::get_limit() const {
    return limit_mb;
}

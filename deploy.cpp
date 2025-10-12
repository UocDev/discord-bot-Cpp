// deploy.cpp
// small program that connects, registers all commands (via register_all_commands),
// then exits. Use it if you want an explicit deploy step.

#include "include/utils.hpp"
#include "command.hpp"
#include <dpp/dpp.h>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    auto token = read_token_file("token.txt");
    if (token.empty()) {
        std::cerr << "[deploy] token.txt not found or empty. Please create token.txt with your bot token.\n";
        return 1;
    }

    dpp::cluster bot(token);

    bot.on_log(dpp::utility::cout_logger());

    // Once ready, register all commands then shutdown.
    bot.on_ready([&bot](const dpp::ready_t&){
        std::cout << "[deploy] Bot ready. Registering commands...\n";
        register_all_commands(bot);
        // Wait a short moment to ensure REST calls finish
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "[deploy] Done. Shutting down.\n";
        bot.shutdown();
    });

    bot.start(dpp::st_wait); // blocking
    std::cout << "[Deploy] Clean exit check. \n";
    return 0;
}

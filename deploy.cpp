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

    bot.on_ready([&bot](const dpp::ready_t&) {
        std::cout << "[deploy] Bot ready. Cleaning up old commands...\n";

        bot.global_commands_get([&bot](const dpp::confirmation_callback_t& callback) {
            if (callback.is_error()) {
                std::cerr << "[deploy] Failed to fetch existing commands.\n";
                return;
            }

            auto commands = std::get<dpp::slashcommand_map>(callback.value);

            for (auto& [id, cmd] : commands) {
                bot.global_command_delete(id, [cmd](const dpp::confirmation_callback_t& del_callback) {
                    if (del_callback.is_error()) {
                        std::cerr << "[deploy] Failed to delete command: " << cmd.name << "\n";
                    } else {
                        std::cout << "[deploy] Deleted old command: " << cmd.name << "\n";
                    }
                });
            }

            std::this_thread::sleep_for(std::chrono::seconds(20));

            std::cout << "[deploy] Registering all new commands...\n";
            register_all_commands(bot);

            std::cout << "[deploy] Finalizing registration...\n";
            std::this_thread::sleep_for(std::chrono::seconds(20));

            std::cout << "[deploy] Done. Shutting down.\n";
            bot.shutdown();
        });
    });

    bot.start(dpp::st_wait); // blocking
    std::cout << "[deploy] Clean exit check.\n";
    return 0;
}

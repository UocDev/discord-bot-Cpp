// main.cpp
#include "utils.hpp"
#include "command.hpp"
#include <dpp/dpp.h>
#include <iostream>

int main() {
    auto token = read_token_file("token.txt");
    if (token.empty()) {
        std::cerr << "token.txt not found or empty. Put your bot token in token.txt (single line).\n";
        return 1;
    }

    dpp::cluster bot(token);

    bot.on_log(dpp::utility::cout_logger());

    // Register handlers & create commands once bot is ready
    bot.on_ready([&bot](const dpp::ready_t&){
        std::cout << "[main] Bot ready. Registering commands & handlers...\n";
        register_all_commands(bot);
    });

    // Basic message logging
    bot.on_message_create([&bot](const dpp::message_create_t& event){
        // optional: simple prefix command handling to test
        if (event.msg.content == "!ping") {
            bot.message_create(dpp::message(event.msg.channel_id, "Pong (prefix)!"));
        }
    });

    std::cout << "[main] Starting bot...\n";
    bot.start(dpp::st_wait); // blocking, non-daemon
    return 0;
}

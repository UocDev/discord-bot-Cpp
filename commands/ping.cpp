#include "command.hpp"
#include <dpp/dpp.h>

// Static registrar: when register_all_commands(bot) called, this lambda runs
static bool _ping_registered = []() {
    register_command([](dpp::cluster& bot){
        // Create slash command (global)
        dpp::slashcommand ping_cmd("ping", "Check bot latency");
        // Register command to Discord (global)
        bot.global_command_create(ping_cmd);

        // Register handler
        bot.on_slashcommand([](const dpp::slashcommand_t& event){
            if (event.command.get_name() == "ping") {
                // reply with latency info (simple)
                event.reply("Pong!");
            }
        });
    });
    return true;
}();

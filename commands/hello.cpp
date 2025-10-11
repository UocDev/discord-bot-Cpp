#include "command.hpp"
#include <dpp/dpp.h>
#include <iostream>

void register_hello_command(dpp::cluster& bot) {
    // Ambil application ID dari bot
    dpp::snowflake app_id = bot.me.id;

    // Buat slash command
    dpp::slashcommand hello_cmd("hello", "Say hello", app_id);

    // Tambahkan optional argument
    hello_cmd.add_option(
        dpp::command_option(dpp::co_string, "name", "Your name", false)
    );

    // Register ke Discord
    bot.global_command_create(hello_cmd);

    // Event handler untuk respon command
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "hello") {
            std::string name = "there";

            auto param = event.get_parameter("name");
            if (std::holds_alternative<std::string>(param)) {
                name = std::get<std::string>(param);
            }

            event.reply("Hello, " + name + "!");
        }
    });
}

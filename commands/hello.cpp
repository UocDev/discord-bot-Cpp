#include "command.hpp"
#include <dpp/dpp.h>
#include <iostream>

void register_hello_command(dpp::cluster& bot) {
    dpp::snowflake app_id = bot.me.id;

    dpp::slashcommand hello_cmd("hello", "Say hello", app_id);
    hello_cmd.add_option(
        dpp::command_option(dpp::co_string, "name", "Your name", false)
    );

    bot.global_command_create(hello_cmd);

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

static bool reg_hello = ([]{
    register_command(register_hello_command);
    return true;
})();

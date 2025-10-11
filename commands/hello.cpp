#include "command.hpp"
#include <dpp/dpp.h>

// Example command with one string option
static bool _hello_registered = []() {
    register_command([](dpp::cluster& bot){
        dpp::command_option name_opt(dpp::co_string, "name", "Your name", true);
        dpp::slashcommand hello_cmd("hello", "Say hello", { name_opt });

        bot.global_command_create(hello_cmd);

        bot.on_slashcommand([](const dpp::slashcommand_t& event){
            if (event.command.get_name() == "hello") {
                std::string name = "there";
                if (!event.get_parameter("name").empty()) {
                    name = event.get_parameter("name");
                }
                event.reply("Hello, " + name + "!");
            }
        });
    });
    return true;
}();

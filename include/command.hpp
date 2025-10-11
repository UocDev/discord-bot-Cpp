#pragma once
#include <dpp/dpp.h>
#include <functional>

// Register function type: receives dpp::cluster& so command can create slash command and attach handlers.
using CommandRegistrar = std::function<void(dpp::cluster&)>;

// Called by each commands/*.cpp to add its registrar
void register_command(const CommandRegistrar& r);

// Called by main/deploy to register all commands (should be called once bot instance exists)
void register_all_commands(dpp::cluster& bot);

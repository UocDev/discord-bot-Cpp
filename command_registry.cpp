#include "command.hpp"
#include <vector>
#include <mutex>

static std::vector<CommandRegistrar>& registry() {
    static std::vector<CommandRegistrar> r;
    return r;
}

void register_command(const CommandRegistrar& r) {
    registry().push_back(r);
}

void register_all_commands(dpp::cluster& bot) {
    for (auto &r : registry()) {
        r(bot);
    }
}

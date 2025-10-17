#include "command.hpp"
#include <dpp/dpp.h>
#include <iomanip>
#include <sstream>

std::string format_latency(double ms) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(3) << ms << "ms";
    return out.str();
}

void register_status_command(dpp::cluster& bot) {
    dpp::slashcommand cmd("ping", "Check bot connection latency", bot.me.id);
    bot.global_command_create(cmd);

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            auto e = std::make_shared<dpp::slashcommand_t>(event);

            double gateway_ping = bot.get_shard(0)->websocket_ping;

            e->thinking();

            auto start = std::chrono::steady_clock::now();

            bot.current_user_get([e, start, gateway_ping](const dpp::confirmation_callback_t& cb) {
                auto end = std::chrono::steady_clock::now();
                std::chrono::duration<double, std::milli> diff = end - start;
                double http_ping = diff.count();

                if (cb.is_error()) {
                    e->edit_response(dpp::message("Failed to fetching bot API datas."));
                    return;
                }

                std::ostringstream msg;
                msg << "HTTP API: " << format_latency(http_ping) << "\n"
                    << "Gateway: " << format_latency(gateway_ping);

                e->edit_response(dpp::message(msg.str()));
            });
        }
    });
}

static bool reg_status = ([] {
    register_command(register_ping_command);
    return true;
})();

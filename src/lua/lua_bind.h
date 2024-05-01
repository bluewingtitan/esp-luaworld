#pragma once

#include "sol/sol.hpp"

namespace lw
{
    void register_http_server(sol::state &lua);
    void execute_http_demo(sol::state &lua);

    void register_io(sol::state &lua);

    void add_imports(sol::state &lua);
    void overwrite_defaults(sol::state &lua);
} // namespace netes

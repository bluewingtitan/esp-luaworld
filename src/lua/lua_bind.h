#pragma once

#include "sol/sol.hpp"

namespace espn::lua
{
    void register_http_server(sol::state &lua);
    void execute_http_demo(sol::state &lua);

    void add_imports(sol::state &lua);
} // namespace netes

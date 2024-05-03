#pragma once
#include "sol/sol.hpp"
#include "service/control.h"

namespace lw
{
    void register_http_server(sol::state &lua);
    void execute_http_demo(sol::state &lua);

    void register_io(sol::state &lua);

    void register_sqlite(sol::state &lua);

    void register_sessionkit(sol::state &lua);

    void add_imports(sol::state &lua);
    void overwrite_defaults(sol::state &lua);
    void add_control(sol::state &lua, lw::control_plane *cp);
} // namespace netes

#include "lua_bind.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <SD.h>

String strip_lua_ending(String str)
{
    auto last_slash = str.lastIndexOf(".lua");
    return str.substring(0, last_slash);
}

String find_lua_file(File dir, String lua_base_path, String searched_lua)
{
    if (lua_base_path.startsWith("."))
    {
        lua_base_path = lua_base_path.substring(1);
    }
    Serial.println("search lua module: " + searched_lua);

    while (File entry = dir.openNextFile())
    {
        String name = entry.name();

        if (entry.isDirectory())
        {
            if (searched_lua.indexOf(name) >= 0)
            {
                Serial.println("search dir: " + name);
                auto ret = find_lua_file(entry, lua_base_path + "." + name, searched_lua);

                if (ret.length() > 0)
                {
                    entry.close();
                    return ret;
                }
            }
            else
            {
                Serial.println("skip dir: " + name);
                entry.close();
                continue;
            }
        }

        if (!name.endsWith(".lua"))
        {
            Serial.println("skip file: " + name);
            entry.close();
            continue;
        }

        String stripped = strip_lua_ending(name);
        Serial.println("check: " + stripped + " in " + lua_base_path);
        if ((lua_base_path + "." + stripped) == searched_lua || stripped == searched_lua)
        {
            // found it!
            auto val = entry.readString();
            entry.close();
            return val;
        }
    }

    return "";
}

int load_file_require(sol::state &state, fs::File &dir, const char *req)
{
    if (dir)
    {
        if (dir.isDirectory())
        {
            auto ret = find_lua_file(dir, "", req);

            if (ret.length() > 0)
            {
                state.load(ret.c_str(), req);
                return 0;
            }
        }
        dir.close();
    }

    Serial.println("Couldn't find module.");

    return 1;
}

int load_file_spiffs(sol::state &state, const char *req)
{
    auto dir = SPIFFS.open("/");
    return load_file_require(state, dir, req);
}

int load_file_sd(sol::state &state, const char *req)
{
    auto dir = SD.open("/lua/");
    return load_file_require(state, dir, req);
}

void use(sol::state &lua, std::string lib)
{
    if (lib == "webserver")
    {
        lw::register_http_server(lua);
        return;
    }

    if (lib == "io")
    {
        lw::register_io(lua);
        return;
    }

    if (lib == "sqlite3")
    {
        lw::register_sqlite(lua);
        return;
    }

    if (lib == "table")
    {
        lua.open_libraries(sol::lib::table);
        return;
    }

    if (lib == "string")
    {
        lua.open_libraries(sol::lib::string);
        return;
    }

    if (lib == "math")
    {
        lua.open_libraries(sol::lib::math);
        return;
    }

    if (lib == "package")
    {
        lua.open_libraries(sol::lib::package);
        return;
    }

    if (lib == "utf8")
    {
        lua.open_libraries(sol::lib::utf8);
        return;
    }

    if (load_file_spiffs(lua, lib.c_str()) == 0)
    {
        return;
    }

    if (load_file_sd(lua, lib.c_str()) == 0)
    {
        return;
    }
}

void lw::add_imports(sol::state &lua)
{
    lua["require"] = [&lua](std::string pck)
    {
        use(lua, pck);
    };

    lua["use"] = lua["require"];
}

void lw::overwrite_defaults(sol::state &lua)
{
    // not needed yet.
}

void lw::add_control(sol::state &lua, lw::control_plane *cp)
{
    auto cp_table = lua.create_table();
    cp_table["start_control_plane"] = [cp]()
    {
        cp->start();
    };

    cp_table["stop_control_plane"] = [cp]()
    {
        cp->stop();
    };

    lua["EspLw"] = cp_table;
}

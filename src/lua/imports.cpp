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

int load_file_spiffs(lua_State *L)
{
    auto dir = SPIFFS.open("/");
    return load_file_require(L, dir);
}

int load_file_sd(lua_State *L)
{
    auto dir = SD.open("/lua/");
    return load_file_require(L, dir);
}

int load_file_require(lua_State *L, fs::File &dir)
{
    std::string path = sol::stack::get<std::string>(L);

    if (dir)
    {
        if (dir.isDirectory())
        {
            auto ret = find_lua_file(dir, "", path.c_str());

            if (ret.length() > 0)
            {
                luaL_loadbuffer(L, ret.begin(), ret.length(), path.c_str());
                return 1;
            }
        }
        dir.close();
    }

    Serial.println("Couldn't find module.");

    sol::stack::push(L, "Sorry dev, but your module is in another castle.");
    return 1;
}

void use(sol::state &lua, std::string lib)
{
    if (lib == "webserver")
    {
        espn::lua::register_http_server(lua);
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
}

void espn::lua::add_imports(sol::state &lua)
{
    lua["use"] = [&lua](std::string pck)
    {
        use(lua, pck);
    };

    lua["package"]["searchers"] = lua.create_table_with(
        1, load_file_spiffs, //
        2, load_file_sd      //
    );
}
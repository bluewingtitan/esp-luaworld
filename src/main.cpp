#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFiManager.h>
#include <sol/sol.hpp>
#include "lua/lua_bind.h"

sol::state lua;

void setup()
{
  SPIFFS.begin();
  Serial.begin(115200);

  WiFiManager manager{};
  manager.autoConnect("espnetes");

  lua.open_libraries(sol::lib::base);
  lw::add_imports(lua);
  lw::overwrite_defaults(lua);
  lw::execute_http_demo(lua);
}

void loop()
{
}

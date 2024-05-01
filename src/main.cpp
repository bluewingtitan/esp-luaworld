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
  espn::lua::add_imports(lua);
  espn::lua::execute_http_demo(lua);
}

void loop()
{
}

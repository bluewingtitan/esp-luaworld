#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFiManager.h>
#include <sol/sol.hpp>
#include "lua/lua_bind.h"

#include "service/control.h"

sol::state lua;
lw::control_plane cp{80};

int execute(FS fs)
{
  if (fs.exists("/app.lua"))
  {
    auto script = fs.open("/app.lua");
    auto str = script.readString();
    lua.safe_script(str.c_str());
    return 0;
  }
  else
  {
    Serial.println("Couldn't find entrypoint script 'app.lua'");
    return 1;
  }
}

void setup()
{
  SPIFFS.begin();
  Serial.begin(115200);

  if (true)
  {
    WiFiManager manager{};
    manager.autoConnect("esp-lw-wifi");
    Serial.println(WiFi.localIP());
  }
  else
  {
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP("esp-lw-wifi", "development");
    Serial.println(WiFi.softAPIP());
    Serial.println("Wifi: esp-lw-wifi\nPassword: development");
  }

  lua.open_libraries(sol::lib::base);
  lw::add_imports(lua);
  lw::overwrite_defaults(lua);
  lw::add_control(lua, &cp);

  execute(SPIFFS);
}

void loop()
{
  cp.loop();
}

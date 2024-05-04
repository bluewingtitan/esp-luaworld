#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFiManager.h>
#include <sol/sol.hpp>
#include <RNG.h>

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

  // init RNG from the two best sources we got.
  auto rand = esp_random(); // uses noise from wifi
  RNG.stir(static_cast<uint8_t *>(static_cast<void *>(&rand)), sizeof(uint32_t), sizeof(uint32_t) * 2);
  rand = millis(); // unpredictable, as wifi-connection time will vary A LOT.
  RNG.stir(static_cast<uint8_t *>(static_cast<void *>(&rand)), sizeof(uint32_t), sizeof(uint32_t) * 1);

  lua.open_libraries(sol::lib::base);
  lw::add_imports(lua);
  lw::overwrite_defaults(lua);
  lw::add_control(lua, &cp);

  execute(SPIFFS);
}

int i = 0;

void loop()
{
  i++;
  cp.loop();

  if (i % 10000)
  {
    // housekeeping that has to be done now and then, but not often to keep the system responsive, as housekeeping tends to add up.
    RNG.loop();
  }
}

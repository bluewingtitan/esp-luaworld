#include "lua_bind.h"
#include <Arduino.h>

void lw::register_io(sol::state &lua)
{
    auto io = lua.create_table();

    io["pin_mode"] = pinMode;
    io["digital_read"] = digitalRead;
    io["digital_write"] = digitalWrite;
    io["analog_read"] = analogRead;
    io["analog_write"] = analogWrite;
    io["shift_out"] = shiftOut;

    lua["io"] = io;

    // pin modes
    lua["INPUT"] = INPUT;
    lua["INPUT_PULLUP"] = INPUT_PULLUP;
    lua["INPUT_PULLDOWN"] = INPUT_PULLDOWN;
    lua["OUTPUT"] = OUTPUT;
    lua["OUTPUT_OPEN_DRAIN"] = OUTPUT_OPEN_DRAIN;
    lua["PULLUP"] = PULLUP;
    lua["PULLDOWN"] = PULLDOWN;
    lua["ANALOG"] = ANALOG;

    // alt
    lua["HIGH"] = HIGH;
    lua["LOW"] = LOW;

    // bit order
    lua["MSBFIRST"] = MSBFIRST;
    lua["LSBFIRST"] = LSBFIRST;
}
-- api for the http-server, no actual implementation,
-- use for intellisense.

-- io
---@type lw_io
_G.io = nil

-- digital pin states
---@type pin_state
_G.HIGH = nil
---@type pin_state
_G.LOW = nil

-- pin modes
---@type pin_mode
_G.INPUT = nil
---@type pin_mode
_G.INPUT_PULLUP = nil
---@type pin_mode
_G.INPUT_PULLDOWN = nil
---@type pin_mode
_G.OUTPUT = nil
---@type pin_mode
_G.OUTPUT_OPEN_DRAIN = nil
---@type pin_mode
_G.PULLUP = nil
---@type pin_mode
_G.PULLDOWN = nil
---@type pin_mode
_G.ANALOG = nil


-- bit orders
---@type bit_order
_G.MSBFIRST = nil
---@type bit_order
_G.LSBFIRST = nil


-- class definitions

--import with use("io")
---@class lw_io
---@field digital_read fun(pin:integer):pin_state
---@field digital_write fun(pin:integer, val:pin_state)
---@field analog_read fun(pin:integer): integer
---@field analog_write fun(pin:integer, val:integer)
---@field pin_mode fun(pin:integer, mode:pin_mode)
---@field shift_out fun(data_pin:integer, clock_pin:integer, bit_order:bit_order, val:integer)

--import with use("io")
---@class pin_mode

--import with use("io")
---@class pin_state

--import with use("io")
---@class bit_order

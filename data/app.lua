-- lua http server demo script

require("webserver")
require("io")
require("string")

local LED = 2
io.pin_mode(2, OUTPUT)

---@type fun(req: http_request)
local function demo_handler(req)
    print("GET: /")
    req:send(200, "text/plain", "Hello, World!")
end

---@type fun(req: http_request)
local function not_found_handler(req)
    print("GET: 404")
    req:redirect("/")
end

local on = HIGH

---@type fun(req: http_request)
local function blink(req)
    req:code(200)

    if on == HIGH then
        on = LOW
    else
        on = HIGH
    end

    print("GET: /blink")
    print(tostring(on))

    io.digital_write(2, on)
end

local server = webserver.new(8080)
server:on("/", HTTP_GET, demo_handler)
server:on("/blink", HTTP_GET, blink)
server:on_not_found(not_found_handler)
server:begin()

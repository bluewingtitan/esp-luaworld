-- lua http server demo script

use("webserver")

local function demo_handler(req)
    req:send(200, "text/plain", "Hello, World!")
end

local server = webserver.new(8080)
server:on("/", HTTP_GET, demo_handler)
server:begin()

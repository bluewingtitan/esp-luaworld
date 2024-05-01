-- lua http server demo script

use("webserver")

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

local server = webserver.new(8080)
server:on("/", HTTP_GET, demo_handler)
server:on_not_found(not_found_handler)
server:begin()

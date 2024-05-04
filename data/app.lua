require("sqlite3")
require("lustache.renderer")
EspLw:start_control_plane();


local view_model = {
    title = "Joe",
    calc = function()
        return 2 + 4
    end
}
local lus = LustacheRenderer:new()
local out = lus:render("{{title}} spends {{calc}}", view_model)
print(out)

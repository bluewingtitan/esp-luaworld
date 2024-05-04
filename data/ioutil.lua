require("io")

---@type fun(file: string): string | nil
function ReadFile(file)
    local f = io.open("/spiffs/migrations.sql")
    if f == nil then
        return nil
    end

    local contents = f:read("a")
    f:close()
    return contents
end

require("sqlite3")
require("ioutil")

---@class offgrid_db
local offgrid_db = {
}

---@type sqlite3
offgrid_db.db = nil

---@type fun(db: sqlite3)
local function run_migrations(db)
    local migrations = ReadFile("/spiffs/migrations.sql")

    if migrations == nil then
        print("failed to open migrations.sql")
        return
    end
    print("applying migrations...")

    local err, errmsg = db:execute(migrations)

    if err == SQLITE_OK then
        print("successfully ran migrations.")
        return
    else
        print("error running migrations:")
        print(errmsg)
        return
    end
end

function offgrid_db:setup()
    self.db = sqlite3.new("offgrid")
    run_migrations(self.db)
end

OffGridDb = offgrid_db

require("sqlite3")
require("ioutil")

---@class offgrid_db
local offgrid_db = {
}


local log = Log.sub("db")

---@type sqlite3
offgrid_db.db = nil

---@type fun(db: sqlite3)
local function run_migrations(db)
    local migrations = ReadFile("/spiffs/migrations.sql")

    if migrations == nil then
        log:err("failed to open migrations.sql")
        return
    end

    if log:enabled(LogLevel.trace) then
        log:trace(migrations)
    end

    log:debug("applying migrations...")

    local err, errmsg = db:execute(migrations)

    if err == SQLITE_OK then
        log:debug("successfully ran migrations.")
        return
    else
        log:err("error running migrations:")
        log:err(errmsg)
        return
    end
end

function offgrid_db:setup()
    log:debug("initialize db...")
    self.db = sqlite3.new("offgrid")
    run_migrations(self.db)
    log:info("fully initialized db.")
end

OffGridDb = offgrid_db

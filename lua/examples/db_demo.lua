-- simple db demo, demonstrating basic opening and usage of a sqlite3 db

require("sqlite3")


local function dump(o)
    if type(o) == 'table' then
        local s = '{ '
        for k, v in pairs(o) do
            if type(k) ~= 'number' then k = '"' .. k .. '"' end
            s = s .. '[' .. k .. '] = ' .. dump(v) .. ','
        end
        return s .. '} '
    else
        return tostring(o)
    end
end

---@type fun(db: sqlite3)
local function db_demo(db)
    local err, msg, data

    print("Clean up tables...")

    err, msg = db:execute(
        "DROP TABLE IF EXISTS user;")

    if not err == SQLITE_OK then
        print("Error executing sql:" .. msg)
        return
    end

    print("Create tables...")

    err, msg = db:execute(
        "CREATE TABLE IF NOT EXISTS user(id INTEGER PRIMARY KEY, name TEXT NOT NULL, pass TEXT NOT NULL);")

    if not err == SQLITE_OK then
        print("Error executing sql:" .. msg)
        return
    end

    print("Insert data...")

    err, msg = db:execute("INSERT INTO user VALUES (1, 'bwt', 'helloworld'), (2, 'bnb', 'helloworld');")
    if not err == SQLITE_OK then
        print("Error executing sql:" .. msg)
        return
    end

    print("Get data...")
    err, msg, data = db:execute_out("SELECT * FROM user;")
    if not err == SQLITE_OK then
        print("Error executing sql:" .. msg)
        return
    end

    print(dump(data))
end

print("running db demo...")

local db = sqlite3.new("demo.db")

if not db:available() then
    print("Couldn't initialize demo.db")
else
    db_demo(db)
end

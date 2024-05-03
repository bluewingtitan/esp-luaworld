-- api for the http-server, no actual implementation,
-- use for intellisense.


_G.sqlite3 = {

    ---@type fun(dbname:string): sqlite3
    new = function(dbname)
        return {}
    end
}

-- errorcodes

-- Successful result
---@type sqlite3_err
_G.SQLITE_OK = nil
-- Generic error
---@type sqlite3_err
_G.SQLITE_ERROR = nil
-- Internal logic error in SQLite
---@type sqlite3_err
_G.SQLITE_INTERNAL = nil
-- Access permission denied
---@type sqlite3_err
_G.SQLITE_PERM = nil
-- Callback routine requested an abort
---@type sqlite3_err
_G.SQLITE_ABORT = nil
-- The database file is locked
---@type sqlite3_err
_G.SQLITE_BUSY = nil
-- A table in the database is locked
---@type sqlite3_err
_G.SQLITE_LOCKED = nil
-- A malloc() failed
---@type sqlite3_err
_G.SQLITE_NOMEM = nil
-- Attempt to write a readonly database
---@type sqlite3_err
_G.SQLITE_READONLY = nil
-- Operation terminated by sqlite3_interrupt()*/
---@type sqlite3_err
_G.SQLITE_INTERRUPT = nil
-- Some kind of disk I/O error occurred
---@type sqlite3_err
_G.SQLITE_IOERR = nil
-- The database disk image is malformed
---@type sqlite3_err
_G.SQLITE_CORRUPT = nil
-- Unknown opcode in sqlite3_file_control()
---@type sqlite3_err
_G.SQLITE_NOTFOUND = nil
-- Insertion failed because database is full
---@type sqlite3_err
_G.SQLITE_FULL = nil
-- Unable to open the database file
---@type sqlite3_err
_G.SQLITE_CANTOPEN = nil
-- Database lock protocol error
---@type sqlite3_err
_G.SQLITE_PROTOCOL = nil
-- Internal use only
---@type sqlite3_err
_G.SQLITE_EMPTY = nil
-- The database schema changed
---@type sqlite3_err
_G.SQLITE_SCHEMA = nil
-- String or BLOB exceeds size limit
---@type sqlite3_err
_G.SQLITE_TOOBIG = nil
-- Abort due to constraint violation
---@type sqlite3_err
_G.SQLITE_CONSTRAINT = nil
-- Data type mismatch
---@type sqlite3_err
_G.SQLITE_MISMATCH = nil
-- Library used incorrectly
---@type sqlite3_err
_G.SQLITE_MISUSE = nil
-- Uses OS features not supported on host
---@type sqlite3_err
_G.SQLITE_NOLFS = nil
-- Authorization denied
---@type sqlite3_err
_G.SQLITE_AUTH = nil
-- Not used
---@type sqlite3_err
_G.SQLITE_FORMAT = nil
-- 2nd parameter to sqlite3_bind out of range
---@type sqlite3_err
_G.SQLITE_RANGE = nil
-- File opened that is not a database file
---@type sqlite3_err
_G.SQLITE_NOTADB = nil
-- Notifications from sqlite3_log()
---@type sqlite3_err
_G.SQLITE_NOTICE = nil
-- Warnings from sqlite3_log()
---@type sqlite3_err
_G.SQLITE_WARNING = nil
-- sqlite3_step() has another row ready
---@type sqlite3_err
_G.SQLITE_ROW = nil
-- sqlite3_step() has finished executing
---@type sqlite3_err
_G.SQLITE_DONE = nil


---@class sqlite3
---@field available fun(this: sqlite3): boolean
---@field execute fun(this: sqlite3, sql:string): (err:sqlite3_err, errmsg: string)
-- execute sql, retrieve potential error and data as an array
---@field execute_out fun(this: sqlite3, sql:string): (err:sqlite3_err, errmsg: string, data:table|nil)

---@class sqlite3_err

#include "lua_bind.h"
#include <sqlite3.h>
#include <tuple>

int callback(void *data, int argc, char **argv, char **azColName)
{
    return 0;
}

int db_open(const char *filename, sqlite3 **db)
{
    int rc = sqlite3_open(filename, db);
    return rc;
}

const char *data = "";
char *zErrMsg = 0;
int db_exec(sqlite3 *db, const char *sql, std::string &errstr)
{
    int rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        errstr = std::string{zErrMsg};
        sqlite3_free(zErrMsg);
    }
}

class sqlite3_wrapper
{
public:
    sqlite3_wrapper(std::string name) : db_{nullptr}
    {
        sqlite3 *db;

        name = "/spiffs/" + name;

        if (db_open(name.c_str(), &db))
        {
        }
    }

    bool available()
    {
        return db_;
    }

    std::tuple<int, std::string> execute(const char *sql)
    {
        std::string errmsg{};
        if (!available())
        {
            return std::make_tuple(1, "DB not running");
        }

        int rc = db_exec(db_, sql, errmsg) == 0;

        return std::make_tuple(rc, errmsg);
    }

private:
    sqlite3 *db_;
};

void lw::register_sqlite(sol::state &lua)
{
    sqlite3_initialize();

    lua.new_usertype<sqlite3_wrapper>(
        "sqlite3", sol::constructors<sqlite3_wrapper(std::string dbname)>(),
        "available", &sqlite3_wrapper::available,
        "execute", &sqlite3_wrapper::execute

    );

    // add error codes
    lua["SQLITE_OK"] = SQLITE_OK;
    lua["SQLITE_ERROR"] = SQLITE_ERROR;
    lua["SQLITE_INTERNAL"] = SQLITE_INTERNAL;
    lua["SQLITE_PERM"] = SQLITE_PERM;
    lua["SQLITE_ABORT"] = SQLITE_ABORT;
    lua["SQLITE_BUSY"] = SQLITE_BUSY;
    lua["SQLITE_LOCKED"] = SQLITE_LOCKED;
    lua["SQLITE_NOMEM"] = SQLITE_NOMEM;
    lua["SQLITE_READONLY"] = SQLITE_READONLY;
    lua["SQLITE_INTERRUPT"] = SQLITE_INTERRUPT;
    lua["SQLITE_IOERR"] = SQLITE_IOERR;
    lua["SQLITE_CORRUPT"] = SQLITE_CORRUPT;
    lua["SQLITE_NOTFOUND"] = SQLITE_NOTFOUND;
    lua["SQLITE_FULL"] = SQLITE_FULL;
    lua["SQLITE_CANTOPEN"] = SQLITE_CANTOPEN;
    lua["SQLITE_PROTOCOL"] = SQLITE_PROTOCOL;
    lua["SQLITE_EMPTY"] = SQLITE_EMPTY;
    lua["SQLITE_SCHEMA"] = SQLITE_SCHEMA;
    lua["SQLITE_TOOBIG"] = SQLITE_TOOBIG;
    lua["SQLITE_CONSTRAINT"] = SQLITE_CONSTRAINT;
    lua["SQLITE_MISMATCH"] = SQLITE_MISMATCH;
    lua["SQLITE_MISUSE"] = SQLITE_MISUSE;
    lua["SQLITE_NOLFS"] = SQLITE_NOLFS;
    lua["SQLITE_AUTH"] = SQLITE_AUTH;
    lua["SQLITE_FORMAT"] = SQLITE_FORMAT;
    lua["SQLITE_RANGE"] = SQLITE_RANGE;
    lua["SQLITE_NOTADB"] = SQLITE_NOTADB;
    lua["SQLITE_NOTICE"] = SQLITE_NOTICE;
    lua["SQLITE_WARNING"] = SQLITE_WARNING;
    lua["SQLITE_ROW"] = SQLITE_ROW;
    lua["SQLITE_DONE"] = SQLITE_DONE;
}

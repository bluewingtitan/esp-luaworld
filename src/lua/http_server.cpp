#include "lua_bind.h"

#include <string>
#include <ESPAsyncWebServer.h>

class http_response
{
public:
    http_response(AsyncWebServerResponse *response, AsyncWebServerRequest *req) : response_{response}, req_{req}
    {
    }

    void add_header(const char *key, const char *value)
    {
        response_->addHeader(key, value);
    }

    void set_code(int code)
    {
        response_->setCode(code);
    }

    void send()
    {
        req_->send(response_);
    }

private:
    AsyncWebServerResponse *response_{};
    AsyncWebServerRequest *req_;
};

class http_response_stream
{
public:
    http_response_stream(AsyncResponseStream *res, AsyncWebServerRequest *req) : res_{res}, req_{req}
    {
    }

    void print(const char *val)
    {
        res_->print(val);
    }

    void send()
    {
        req_->send(res_);
    }

    void add_header(const char *key, const char *value)
    {
        res_->addHeader(key, value);
    }

    void set_code(int code)
    {
        res_->setCode(code);
    }

private:
    AsyncResponseStream *res_;
    AsyncWebServerRequest *req_;
};

class http_request
{
public:
    http_request(AsyncWebServerRequest *req) : req_{req}
    {
    }

    void code(int code)
    {
        req_->send(code);
    }

    void send(int code, const char *type, const char *content)
    {
        req_->send(code, type, content);
    }

    bool has_header(const char *param_name)
    {
        return req_->hasHeader(param_name);
    }

    std::string get_header(const char *param_name)
    {
        return req_->getHeader(param_name)->value().c_str();
    }

    bool has_arg(const char *param_name)
    {
        return req_->hasArg(param_name);
    }

    std::string get_arg(const char *param_name)
    {
        return req_->arg(param_name).c_str();
    }

    bool has_param(const char *param_name)
    {
        return req_->hasParam(param_name);
    }

    std::string get_param(const char *param_name)
    {
        return req_->getParam(param_name)->value().c_str();
    }

    void redirect(const char *target)
    {
        req_->redirect(target);
    }

    http_response begin_response_code(int code)
    {
        return http_response{req_->beginResponse(code), req_};
    }

    http_response begin_response(int code, const char *type, const char *content)
    {
        return http_response{req_->beginResponse(code, type, content), req_};
    }

    http_response_stream begin_response_stream(const char *type)
    {
        return http_response_stream{req_->beginResponseStream(type), req_};
    }

private:
    AsyncWebServerRequest *req_;
};

class http_server
{
public:
    http_server(uint16_t port) : server{port}
    {
    }

    void on(const char *path, byte http_method, sol::function handler)
    {
        server.on(path, http_method, [handler](AsyncWebServerRequest *req)
                  {
                      std::function<void(http_request)> handlerT = handler;
                      auto request = http_request{req};
                      handlerT(request); //
                  });
    }

    void on_not_found(sol::function handler)
    {
        server.onNotFound([handler](AsyncWebServerRequest *req)
                          {
                              std::function<void(http_request)> handlerT = handler;
                              auto request = http_request{req};
                              handlerT(request); //
                          });
    }

    void begin()
    {
        server.begin();
    }

private:
    AsyncWebServer server;
};

void lw::register_http_server(sol::state &lua)
{
    lua.new_usertype<http_response>(
        "http_response", sol::no_constructor,
        "add_header", &http_response::add_header,
        "set_code", &http_response::set_code,
        "send", &http_response::send

    );

    lua.new_usertype<http_response_stream>(
        "http_response_stream", sol::no_constructor,
        "print", &http_response_stream::print,
        "add_header", &http_response_stream::add_header,
        "set_code", &http_response_stream::set_code,
        "send", &http_response_stream::send

    );

    lua.new_usertype<http_request>(
        "http_request", sol::no_constructor,
        "code", &http_request::code,
        "send", &http_request::send,
        "redirect", &http_request::redirect,
        "has_header", &http_request::has_header,
        "get_header", &http_request::get_header,
        "has_arg", &http_request::has_arg,
        "get_arg", &http_request::get_arg,
        "has_param", &http_request::has_param,
        "get_param", &http_request::get_param,

        "begin_response", &http_request::begin_response,
        "begin_response_code", &http_request::begin_response_code,

        "begin_stream", &http_request::begin_response_stream

    );

    lua.new_usertype<http_server>(
        "webserver", sol::constructors<http_server(uint16_t)>(),
        "on", &http_server::on,
        "on_not_found", &http_server::on_not_found,
        "begin", &http_server::begin

    );

    lua["HTTP_GET"] = HTTP_GET;
    lua["HTTP_POST"] = HTTP_POST;
    lua["HTTP_DELETE"] = HTTP_DELETE;
    lua["HTTP_PUT"] = HTTP_PUT;
    lua["HTTP_PATCH"] = HTTP_PATCH;
    lua["HTTP_HEAD"] = HTTP_HEAD;
    lua["HTTP_OPTIONS"] = HTTP_OPTIONS;
    lua["HTTP_ANY"] = HTTP_ANY;
}

void lw::execute_http_demo(sol::state &lua)
{
    std::string demo_script = R"(
-- lua http server demo script

use("webserver")
use("io")
use("string")

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



    )";

    Serial.println("Running http demo...");

    lua.script(demo_script);
}
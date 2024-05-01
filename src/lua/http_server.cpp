#include "lua_bind.h"

#include <string>
#include <ESPAsyncWebServer.h>

class http_request
{
public:
    http_request(AsyncWebServerRequest *req) : req_{req}
    {
    }
    http_request() : req_{nullptr}
    {
    }

    void code(int code)
    {
        req_->send(code);
    }

    void send(int code, std::string type, std::string content)
    {
        req_->send(code, type.c_str(), content.c_str());
    }

    bool has_header(std::string param_name)
    {
        return req_->hasHeader(param_name.c_str());
    }

    std::string get_header(std::string param_name)
    {
        return req_->getHeader(param_name.c_str())->value().c_str();
    }

    bool has_arg(std::string param_name)
    {
        return req_->hasArg(param_name.c_str());
    }

    std::string get_arg(std::string param_name)
    {
        return req_->arg(param_name.c_str()).c_str();
    }

    bool has_param(std::string param_name)
    {
        return req_->hasParam(param_name.c_str());
    }

    std::string get_param(std::string param_name)
    {
        return req_->getParam(param_name.c_str())->value().c_str();
    }

    void redirect(std::string target)
    {
        req_->redirect(target.c_str());
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
    ~http_server()
    {
    }

    void on(std::string path, byte http_method, sol::function handler)
    {
        server.on(path.c_str(), http_method, [handler](AsyncWebServerRequest *req)
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
    lua.new_usertype<http_request>(
        "request",
        sol::constructors<http_request()>(),
        "code",
        &http_request::code,
        "send",
        &http_request::send,
        "redirect",
        &http_request::redirect,
        "has_header",
        &http_request::has_header,
        "get_header",
        &http_request::get_header,
        "has_arg",
        &http_request::has_arg,
        "get_arg",
        &http_request::get_arg,
        "has_param",
        &http_request::has_param,
        "get_param",
        &http_request::get_param //
    );

    lua.new_usertype<http_server>(
        "webserver",
        sol::constructors<http_server(uint16_t)>(),
        "on",
        &http_server::on,
        "on_not_found",
        &http_server::on_not_found,
        "begin",
        &http_server::begin //
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

    )";

    Serial.println("Running http demo...");

    lua.script(demo_script);
}
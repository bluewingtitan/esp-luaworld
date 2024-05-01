-- api for the http-server, no actual implementation,
-- use for intellisense.

-- async webserver with basic routing.
-- theoretically capable of a lot more, currently most functions are not bridged over to lua.
_G.webserver = {
    ---@type fun(port:number): webserver_imp
    new = function(port)
        return {}
    end
}

-- http request types
---@type http_method
_G.HTTP_GET = nil
---@type http_method
_G.HTTP_POST = nil
---@type http_method
_G.HTTP_DELETE = nil
---@type http_method
_G.HTTP_PUT = nil
---@type http_method
_G.HTTP_PATCH = nil
---@type http_method
_G.HTTP_HEAD = nil
---@type http_method
_G.HTTP_OPTIONS = nil
---@type http_method
_G.HTTP_ANY = nil


-- class definitions
---@class webserver_imp
---@field on fun(this:webserver_imp, path:string, http_method:http_method, callback:fun(req: http_request))
---@field on_not_found fun(this:webserver_imp, callback:fun(req: http_request))
---@field begin fun(this:webserver_imp)
local webserver_imp_dummy = {}

---@class http_request
---@field code fun(this:http_request, code:number)
---@field send fun(this:http_request, code:number, type:string, content:string)
---@field has_header fun(this:http_request, name:string)
---@field get_header fun(this:http_request, name:string): string
---@field has_arg fun(this:http_request, name:string)
---@field get_arg fun(this:http_request, name:string): string
---@field has_param fun(this:http_request, name:string)
---@field get_param fun(this:http_request, name:string): string
local http_request_dummy = {}

---@class http_method
local http_method_dummy = {}

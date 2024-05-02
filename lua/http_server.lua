-- api for the http-server, no actual implementation,
-- use for intellisense.


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

--import with use("webserver")
---@class webserver_imp
---@field on fun(this:webserver_imp, path:string, http_method:http_method, callback:fun(req: http_request))
---@field on_not_found fun(this:webserver_imp, callback:fun(req: http_request))
---@field begin fun(this:webserver_imp)

--import with use("webserver")
---@class http_request
---@field code fun(this:http_request, code:integer)
---@field send fun(this:http_request, code:integer, type:string, content:string)
---@field redirect fun(this:http_request, target:string)
---@field has_header fun(this:http_request, name:string)
---@field get_header fun(this:http_request, name:string): string
---@field has_arg fun(this:http_request, name:string)
---@field get_arg fun(this:http_request, name:string): string
---@field has_param fun(this:http_request, name:string)
---@field get_param fun(this:http_request, name:string): string
---@field begin_response fun(this:http_request, code:integer, type:string, content:string): http_response
---@field begin_response_code fun(this:http_request, code:integer): http_response
---@field begin_stream fun(this:http_request, type:string): http_response_stream

--import with use("webserver")
---@class http_method

--import with use("webserver")
---@class http_response
---@field add_header fun(this:http_response, key:string, value:string)
---@field set_code fun(this:http_response, code:integer)
---@field send fun(this:http_response)

--import with use("webserver")
---@class http_response_stream
---@field print fun(this:http_response_stream, val:string)
---@field add_header fun(this:http_response_stream, key:string, value:string)
---@field set_code fun(this:http_response_stream, code:integer)
---@field send fun(this:http_response_stream)

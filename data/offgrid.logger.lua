-- basic logger

-- level to log in
---@enum log_level
local log_level = {
    trace = 0,
    debug = 1,
    information = 2,
    warning = 3,
    error = 4,
    fatal = 5
}


---@class logger
---@field sub fun(self:logger, topic:string): logger
---@field trace fun(self:logger, message:string)
---@field debug fun(self:logger, message:string)
---@field info  fun(self:logger, message:string)
---@field warn  fun(self:logger, message:string)
---@field err   fun(self:logger, message:string)
---@field fatal fun(self:logger, message:string)
---@field set_level fun(self:logger, level: log_level)
---@field enabled fun(self:logger, level: log_level): boolean
---@field sink log_sink
---@field topic string
---@field level log_level


---@class sublogger: logger
---@field parent logger

---@class log_sink
---@field down fun(self:log_sink, level:log_level, message:string)

---@type fun(parent:logger, topic:string): logger
local function create_sublogger(parent, topic)
    ---@type sublogger
    local ret = {
        parent = parent,
        sink = {
            parent = parent,
            down = function(self, level, message)
                ---@diagnostic disable-next-line: undefined-field
                self.parent.sink:down(level, message)
            end
        },
        topic = parent.topic .. "." .. topic,
        level = log_level.information,
        sub = function(self, topic)
            return create_sublogger(self, topic)
        end,
        trace = function(self, message)
            if self:enabled(log_level.trace) then
                self.sink:down(log_level.trace, "(T)[" .. self.topic .. "]\t" .. message)
            end
        end,
        debug = function(self, message)
            if self:enabled(log_level.debug) then
                self.sink:down(log_level.debug, "(D)[" .. self.topic .. "]\t" .. message)
            end
        end,
        info = function(self, message)
            if self:enabled(log_level.information) then
                self.sink:down(log_level.information, "(I)[" .. self.topic .. "]\t" .. message)
            end
        end,
        warn = function(self, message)
            if self:enabled(log_level.warning) then
                self.sink:down(log_level.warning, "(W)[" .. self.topic .. "]\t" .. message)
            end
        end,
        err = function(self, message)
            if self:enabled(log_level.error) then
                self.sink:down(log_level.error, "(E)[" .. self.topic .. "]\t" .. message)
            end
        end,
        fatal = function(self, message)
            if self:enabled(log_level.fatal) then
                self.sink:down(log_level.fatal, "(F)[" .. self.topic .. "]\t" .. message)
            end
        end,
        enabled = function(self, level)
            ---@diagnostic disable-next-line: undefined-field
            return self.parent:enabled(level) and level >= self.level
        end,
        set_level = function(self, level)
            self.level = level
        end
    }
    return ret
end

---@type fun(sink:log_sink, topic:string): logger
local function create_logger(sink, topic)
    ---@type logger
    local ret = {
        sink = sink,
        topic = topic,
        level = log_level.information,
        sub = function(self, topic)
            return create_sublogger(self, topic)
        end,
        trace = function(self, message)
            if self:enabled(log_level.trace) then
                self.sink:down(log_level.trace, "(T)[" .. self.topic .. "]\t" .. message)
            end
        end,
        debug = function(self, message)
            if self:enabled(log_level.debug) then
                self.sink:down(log_level.debug, "(D)[" .. self.topic .. "]\t" .. message)
            end
        end,
        info = function(self, message)
            if self:enabled(log_level.information) then
                self.sink:down(log_level.information, "(I)[" .. self.topic .. "]\t" .. message)
            end
        end,
        warn = function(self, message)
            if self:enabled(log_level.warning) then
                self.sink:down(log_level.warning, "(W)[" .. self.topic .. "]\t" .. message)
            end
        end,
        err = function(self, message)
            if self:enabled(log_level.error) then
                self.sink:down(log_level.error, "(E)[" .. self.topic .. "]\t" .. message)
            end
        end,
        fatal = function(self, message)
            if self:enabled(log_level.fatal) then
                self.sink:down(log_level.fatal, "(F)[" .. self.topic .. "]\t" .. message)
            end
        end,
        enabled = function(self, level)
            return level >= self.level
        end,
        set_level = function(self, level)
            self.level = level
        end
    }
    return ret
end



-- exports:
LogLevel = log_level

---@type log_sink
PrintSink = {
    down = function(self, level, message)
        print(message)
    end
}

CreateLogger = create_logger

Log = {
    ---@type logger
    ---@private
    logger = nil,
    ---@type fun(sink:log_sink, topic: string)
    init_logger = function(sink, topic)
        Log.logger = create_logger(sink, topic)
    end,
    sub = function(topic)
        return Log.logger:sub(topic)
    end,
    trace = function(message)
        return Log.logger:trace(message)
    end,
    debug = function(message)
        return Log.logger:debug(message)
    end,
    info = function(message)
        return Log.logger:info(message)
    end,
    warn = function(message)
        return Log.logger:warn(message)
    end,
    err = function(message)
        return Log.logger:err(message)
    end,
    fatal = function(message)
        return Log.logger:fatal(message)
    end,
}

require("offgrid.logger")
Log.init_logger(PrintSink, "offgrid")
Log.logger:set_level(LogLevel.trace)

require("offgrid.db")

Log.info("Starting up offgrid social...")

OffgridEnv = {
    Db = OffGridDb
}

Log.debug("Initialize modules")
OffgridEnv.Db:setup()
Log.debug("Initialized modules")

Log.info("Offgrid Social ready")

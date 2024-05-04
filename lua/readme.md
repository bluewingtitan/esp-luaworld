# lua scripting in lua-world

## deployment

The code will look for a app.lua in the file system and execute it if found.

## development

Essentially it boils down to:

- install the lua extension in vscode for intellisense (the one by sumneko)
- copy the top level files in the lua folder into your project
- require env.lua (only for development to activate intellisense, comment out for deployment!)

after this, you should have intellisense.

## require() and libraries

Use require to import all available parts of the standard library, the custom library, and also other lua modules.
The lua module search path is first /spiffs/ followed by /sd/lua/ and is searched for recursively (e.g. require("xyz/module") will be looked up under /spiffs/xyz/module.lua and /sd/lua/xyz/module.lua).

Due to limitations of the plattform, require targeting other files works a bit differently, which will break some libraries:

- require always is relative to the base search path, not the module directory (e.g. /xy/z.lua needs to use require("xy.b") to require /xy/b.lua)
- require will not return anything, even if the loaded module returns.
- require will fully execute all top-level code in the loaded module before returning. Use this to setup global variables as needed.

This compromise was chosen carefully to still enable easy porting of most packages.

### lua standard library

From the standard library the following are importable via use:

- table
- string
- math
- utf8

All other stdlibs either did not work, or might now offer the full experience.

For custom libs look into the following sections

### custom libs

#### webserver

> an async webserver with routing.
> `use("webserver")`

Look into examples/webserver.lua for an example.

#### io

> not the default io package
> instead specific for esp32 (basic pin manipulation)
> `use("io")`

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

Because overwriting the require() function comes with compromises am currently not willing to take, lua-world has it's own alternative: `use(...)`

require() can still be used to import other local lua files/modules.
Modules will be searched in spiffs and on a potential sd card.

### lua standard library

From the standard library the following are importable via use:

- table
- string
- math
- package
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

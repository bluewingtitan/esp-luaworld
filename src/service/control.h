#pragma once
#include <cstdint>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

#include "ESP-FTP-Server-Lib.h"
#include "FTPFilesystem.h"

namespace lw
{
    class control_plane
    {
    public:
        control_plane(uint16_t port);
        ~control_plane();

        void start();
        void stop();

        void loop();

    private:
        bool running{false};
        AsyncWebServer serv_;
        FTPServer ftp_;
    };

} // namespace lw

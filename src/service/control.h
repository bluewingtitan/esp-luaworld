#pragma once
#include <cstdint>
#include <ESPAsyncWebServer.h>

namespace lw
{
    class control_plane
    {
    public:
        control_plane(uint16_t port);
        ~control_plane();

        void start();
        void stop();

    private:
        AsyncWebServer serv_;
    };

} // namespace lw

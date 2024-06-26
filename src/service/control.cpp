#include "control.h"
#include <microrender.h>
#include <microrender.simpleajax.h>
#include <microrender.special_characters.h>
#include <microrender.modules.h>

using namespace lw;

MCR(control_home, ESP LuaWorld Control,

    MCR_BASE(
        MCR_HSPACE(20)
            MCR_ROW(
                MCR_ELEMENT(
                    MCR_H1(ESP LuaWorld Control)))
                MCR_HSPACE(20)

                    MCR_ROW(
                        MCRA_ANCHOR_TEXT(memory_free, 0, memory_free) / MCRA_ANCHOR_TEXT(memory_total, 0, memory_total) B free.

                                                                        MCRA_TRIGGER_INTERVAL(10000, memory_free)

                            )

                        )

)

control_plane::control_plane(uint16_t port) : serv_{port}, ftp_{}
{
    serv_.on("/", [](AsyncWebServerRequest *req)
             { req->send(200, "text/html", control_home()); });

    serv_.on("/memory_free", [](AsyncWebServerRequest *req)
             { req->send(200, "text/plain", std::to_string(ESP.getFreeHeap()).c_str()); });

    serv_.on("/memory_total", [](AsyncWebServerRequest *req)
             { req->send(200, "text/plain", std::to_string(ESP.getHeapSize()).c_str()); });

    ftp_.addUser("dev", "dev");
    ftp_.addFilesystem("SPIFFS", &SPIFFS);
}

control_plane::~control_plane()
{
}

void control_plane::start()
{
    if (running)
        return;
    ftp_.begin();
    serv_.begin();
    Serial.println("Control plane started.");
    running = true;
}

void control_plane::stop()
{
    if (!running)
        return;

    serv_.end();
    Serial.println("Control plane stopped.");
    running = false;
}

void control_plane::loop()
{
    if (!running)
        return;

    ftp_.handle();
}

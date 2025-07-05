#pragma once

#include <crow.h>

class ApiServer {
    public:
        static void run(uint16_t port = 8080);
};
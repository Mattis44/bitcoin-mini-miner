#pragma once

#include <crow.h>

inline void register_status_route(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/status")
    ([] {
        crow::json::wvalue response;
        response["status"] = "ok";
        response["message"] = "Blockchain is running.";
        return response;
    });
}

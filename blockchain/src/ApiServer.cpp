#include "ApiServer.h"

#include "routes/status.h"
#include "routes/block.h"
#include "routes/mine.h"


void ApiServer::run(uint16_t port) {
    crow::SimpleApp app;
    
    register_status_route(app);
    register_block_routes(app);
    register_mine_routes(app);

    app.port(port).multithreaded().run();
}
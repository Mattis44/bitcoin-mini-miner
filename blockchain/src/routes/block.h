#pragma once

#include <crow.h>
#include "lib/BlockLoader.h"

inline void register_block_routes(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/block/latest")
    ([] {
        crow::json::wvalue response;
        const auto latest_block = BlockLoader::load_block_template(BlockLoader::get_last_block_id());
        response["id"] = latest_block.get_id();
        response["difficulty"] = latest_block.get_difficulty();
        response["nonce"] = latest_block.get_block().get_nonce().has_value() ? latest_block.get_block().get_nonce().value() : NAN;
        return response;
    });
}
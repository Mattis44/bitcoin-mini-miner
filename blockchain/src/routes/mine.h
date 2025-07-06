#pragma once

#include <crow.h>

#include "lib/BlockLoader.h"
#include "lib/Hash.h"
#include "lib/Generator.h"

inline void register_mine_routes(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/mine")
    ([] {
        crow::json::wvalue res;
        auto latest = BlockLoader::load_block_template(BlockLoader::get_last_block_id());
        if (latest.get_block().get_nonce().has_value()) {
            latest = Generator::generate_next();
        }
        res["id"] = latest.get_id();
        res["difficulty"] = latest.get_difficulty();
        res["hash"] = latest.get_hash().has_value() ? latest.get_hash().value() : "null";

        crow::json::wvalue::list data_array;
        for (const auto& [hash, score] : latest.get_block().get_data()) {
            crow::json::wvalue pair;
            pair[0] = hash;
            pair[1] = score;
            data_array.push_back(std::move(pair));
        }

        res["block"]["nonce"] = latest.get_block().get_nonce().has_value() ? latest.get_block().get_nonce().value() : NAN;
        res["block"]["previous_hash"] = latest.get_block().get_previous_hash();
        res["block"]["timestamp"] = latest.get_block().get_timestamp();
        res["block"]["data"] = std::move(data_array);
        return res;
    });

    CROW_ROUTE(app, "/mine/submit").methods("POST"_method)([](const crow::request &req) {
        auto body = crow::json::load(req.body);
        if (!body || !body["nonce"] || body["nonce"].t() != crow::json::type::Number) {
            return crow::response(400, "Invalid request body. 'nonce' is required and must be an integer.");
        }

        uint64_t nonce = body["nonce"].i();
        auto block_template = BlockLoader::load_block_template(BlockLoader::get_last_block_id());
        auto block = block_template.get_block();
        if (block.get_nonce().has_value()) {
            return crow::response(400, "Block already mined with nonce: " + std::to_string(block.get_nonce().value()));
        }
        block.set_nonce(nonce);
        Hash hasher;
        std::string hash = hasher.compute_block_hash(block);
        if (hasher.check_difficulty(hash, block_template.get_difficulty())) {
            crow::json::wvalue res;
            res["status"] = "success";
            res["hash"] = hash;
            res["nonce"] = nonce;
            res["id"] = block_template.get_id();
            res["difficulty"] = block_template.get_difficulty();
            block_template.set_hash(hash);
            block_template.set_block(block);
            Generator::save_nonce_hash(block_template.get_id(), nonce, hash);
            auto new_block = Generator::generate_next();
            
            return crow::response(200, res);
        }

        return crow::response(400, "Invalid nonce for difficulty: " + std::to_string(block_template.get_difficulty()) + " Hash: " + hash);
    });
}
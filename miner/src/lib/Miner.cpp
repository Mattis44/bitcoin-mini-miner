#include <nlohmann/json.hpp>
#include "Miner.h"
#include <iostream>

using json = nlohmann::json;

std::pair<unsigned int, json> Miner::get_start_full_data(const std::string &entries) {
    try {
        auto j = json::parse(entries);
        unsigned int difficulty = j["difficulty"].get<unsigned int>();

        json block_data;
        block_data["previous_hash"] = j["block"]["previous_hash"].get<std::string>();
        block_data["timestamp"] = j["block"]["timestamp"].get<uint64_t>();
        block_data["data"] = j["block"]["data"];

        return {difficulty, block_data};
    } catch (const std::exception &e) {
        throw std::runtime_error("Failed to parse: " + std::string(e.what()));
    }
}

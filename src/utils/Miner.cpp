#include <nlohmann/json.hpp>
#include "Miner.h"

using json = nlohmann::json;

std::pair<unsigned int, json> Miner::get_start_full_data(const std::string &entries) {
    try {
        auto j = json::parse(entries);
        unsigned int difficulty = j["difficulty"].get<unsigned int>();
        auto block = j["block"]["data"];
        
        return {difficulty, {block}};
    } catch (const std::exception &e) {
        throw std::runtime_error("Failed to parse difficulty: " + std::string(e.what()));
    }
}

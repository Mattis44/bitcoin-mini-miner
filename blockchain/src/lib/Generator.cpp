#include "Generator.h"
#include "BlockLoader.h"
#include "BlockTemplate.h"
#include "Block.h"

#include <random>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <iostream>

using json = nlohmann::json;

std::string Generator::generate_hash() {
    static const char hex_chars[] = "0123456789abcdef";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    std::string hash;
    for (int i = 0; i < 64; ++i) {
        hash += hex_chars[dis(gen)];
    }
    return hash;
}

unsigned int Generator::get_random_difficulty() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> diff_dist(14, 20);
    return diff_dist(gen);
}

BlockTemplate Generator::generate_next() {
    unsigned int next_id = BlockLoader::get_next_block_id();
    if (next_id == 0) {
        throw std::runtime_error("[Generator] Cannot generate the first block, please create a genesis block manually.");
    }
    unsigned int difficulty = get_random_difficulty();
    auto previous_block = BlockLoader::load_block_template(next_id - 1);
    auto previous_hash_opt = previous_block.get_hash();

    if (!previous_hash_opt.has_value()) {
        throw std::runtime_error("[Generator] Previous block hash missing: block was probably not mined correctly.");
    }

    std::string previous_hash = previous_hash_opt.value();
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> score_dist(-100, 100);

    std::vector<std::pair<std::string, int>> data;
    json data_json = json::array();

    for (int i = 0; i < 5; ++i) {
        std::string hash = generate_hash();
        int score = score_dist(gen);
        data.emplace_back(hash, score);
        data_json.push_back({hash, score});
    }

    json j;
    j["difficulty"] = difficulty;
    j["id"] = next_id;
    j["block"]["nonce"] = nullptr;
    j["block"]["data"] = data_json;
    j["block"]["previous_hash"] = previous_hash;
    j["block"]["timestamp"] = timestamp;

    std::ostringstream filename;
    filename << "../blocks/" << std::setw(6) << std::setfill('0') << next_id << ".json";
    std::ofstream file(filename.str());
    file << std::setw(2) << j;

    Block block(data, previous_hash, timestamp, std::nullopt);
    BlockTemplate bt;
    bt.set_id(next_id);
    bt.set_difficulty(difficulty);
    bt.set_block(block);

    return bt;
}

void Generator::save_nonce_hash(unsigned int id, uint64_t nonce, const std::string &hash) {
    std::ostringstream path;
    path << "../blocks/" << std::setw(6) << std::setfill('0') << id << ".json";
    
    std::ifstream input(path.str());
    if (!input) {
        throw std::runtime_error("[Generator] Could not open block file for ID: " + std::to_string(id));
    }

    json j;
    input >> j;
    input.close();

    j["block"]["nonce"] = nonce;
    j["hash"] = hash;

    std::ofstream output(path.str());
    output << std::setw(2) << j;
    output.close();
}

#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <algorithm>
#include <filesystem>
#include <stdexcept>

#include "BlockLoader.h"
#include "BlockTemplate.h"
#include <iostream>

namespace fs = std::filesystem;

std::vector<BlockTemplate> BlockLoader::load_all_blocks_template() {
    std::regex pattern(R"((\d{6})\.json)");
    std::vector<fs::directory_entry> entries;
    std::vector<BlockTemplate> blocks_template;

    for (const auto& entry : fs::directory_iterator("../blocks")) {
        if (!entry.is_regular_file()) continue;

        const std::string filename = entry.path().filename().string();
        std::smatch match;
        if (std::regex_match(filename, match, pattern)) {
            entries.push_back(entry);
        }
    }

    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
        return a.path().filename() < b.path().filename();
    });

    for (const auto& entry : entries) {
        std::smatch match;
        const std::string filename = entry.path().filename().string();
        if (std::regex_match(filename, match, pattern)) {
            unsigned int id = std::stoi(match[1].str());
            auto parsed = parse_block_template_from_file(entry.path(), id);
            if (parsed.has_value()) {
                blocks_template.push_back(parsed.value());
            }
        }
    }

    return blocks_template;
}


BlockTemplate BlockLoader::load_block_template(unsigned int id) {
    std::ostringstream filename;
    filename << "../blocks/" << std::setw(6) << std::setfill('0') << id << ".json";
    auto parsed = parse_block_template_from_file(filename.str(), id);

    if (!parsed.has_value()) {
        throw std::runtime_error("[BlockLoader] Failed to load block template with id " + std::to_string(id));
    }

    return parsed.value();
}

 std::optional<BlockTemplate> BlockLoader::parse_block_template_from_file(const fs::path& path, unsigned int id) {
    std::ifstream file(path);
    if (!file)
        return std::nullopt;

    try {
        json j;
        file >> j;

        if (!check_block_template_data(j))
            return std::nullopt;

        std::vector<std::pair<std::string, int>> parsed_data;
        for (const auto& entry : j["block"]["data"]) {
            parsed_data.emplace_back(entry[0].get<std::string>(), entry[1].get<int>());
        }

        std::string previous_hash = j["block"]["previous_hash"].get<std::string>();
        uint64_t timestamp = j["block"]["timestamp"].get<uint64_t>();
        
        Block block(parsed_data, previous_hash, timestamp, j["block"]["nonce"].is_null() ? std::nullopt : std::make_optional(j["block"]["nonce"].get<unsigned int>()));
        BlockTemplate bt;
        bt.set_id(id);
        bt.set_difficulty(j["difficulty"].get<unsigned int>());
        bt.set_hash(j.contains("hash") ? std::make_optional(j["hash"].get<std::string>()) : std::nullopt);
        bt.set_block(block);

        return bt;
    } catch (...) {
        return std::nullopt;
    }
}


bool BlockLoader::check_block_template_data(json &json_block_template) {
    if (!json_block_template.contains("difficulty") || !json_block_template.contains("block")) {
        return false;
    }
    if (!json_block_template["block"].contains("nonce") || !json_block_template["block"].contains("data")) {
        return false;
    }
    if (!json_block_template["block"]["data"].is_array()) {
        return false;
    }
    for (const auto &tx : json_block_template["block"]["data"]) {
        if (!tx.is_array() || tx.size() != 2 || !tx[0].is_string() || !tx[1].is_number()) {
            return false;
        }
    }
    if (!json_block_template["difficulty"].is_number_unsigned()) {
        return false;
    }
    return true;
}
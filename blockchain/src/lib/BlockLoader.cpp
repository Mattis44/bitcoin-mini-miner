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

    for (const auto &entry : fs::directory_iterator("../blocks")) {
        if (!entry.is_regular_file())
            continue;
        
            const std::string filename = entry.path().filename().string();
            if (std::regex_match(filename, pattern)) {
                entries.push_back(entry);
            }
    }

    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
        return a.path().filename() < b.path().filename();
    });


    for (const auto &entry : entries) {
        BlockTemplate block_template;
        std::ifstream file(entry.path());
        std::smatch match;
        const std::string filename = entry.path().filename().string();
        if (std::regex_match(filename, match, pattern)) {
            if (!file)
                continue;
            
            json j;
            try {
                file >> j;
                std::vector<std::pair<std::string, int>> parsed_data;
                if (!check_block_template_data(j)) {
                    throw std::runtime_error("[BlockLoader] Invalid block data in " + entry.path().string());
                }
                unsigned int id = std::stoi(match[1].str());
                block_template.set_id(id);
                block_template.set_difficulty(j["difficulty"].get<unsigned int>());
                
                for (const auto &entry : j["block"]["data"]) {
                    std::string hash = entry[0].get<std::string>();
                    int score = entry[1].get<int>();
                    parsed_data.emplace_back(hash, score);
                }
    
                Block block(parsed_data);
                block_template.set_block(block);
                blocks_template.push_back(block_template);
            } catch (...) {
                std::runtime_error("[BlockLoader] Error while loading blocks");
            }
        }
    }

    return blocks_template;
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
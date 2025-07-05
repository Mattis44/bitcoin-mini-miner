#pragma once

#include <vector>
#include <nlohmann/json.hpp>

#include "BlockTemplate.h"
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

class BlockLoader {
    private:
        static bool check_block_template_data(json &json_block_template);
        static std::optional<BlockTemplate> parse_block_template_from_file(const fs::path &path, const unsigned int id);
    public:
        static std::vector<BlockTemplate> load_all_blocks_template();
        static BlockTemplate load_block_template(unsigned int id);
        static unsigned int get_last_block_id() {
            auto count = std::distance(fs::directory_iterator("../blocks"), fs::directory_iterator{});
            if (count < 0 || static_cast<uint64_t>(count) > std::numeric_limits<unsigned int>::max()) {
                throw std::runtime_error("[BlockLoader] Too many blocks or invalid block count.");
            }
            return static_cast<unsigned int>(count);
        }

        static unsigned int get_next_block_id() {
            return get_last_block_id() + 1;
        }
};
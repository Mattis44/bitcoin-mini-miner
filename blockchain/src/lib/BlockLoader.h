#pragma once

#include <vector>
#include <nlohmann/json.hpp>

#include "BlockTemplate.h"

using json = nlohmann::json;

class BlockLoader {
    private:
        static bool check_block_template_data(json &json_block_template);
    public:
        static std::vector<BlockTemplate> load_all_blocks_template();
};
#pragma once

#include <string>

#include "Block.h"

class BlockTemplate {
    private:
        unsigned int id;
        /** Number of '0' bits from the result hash found by Miners. */
        unsigned int difficulty;
        /** A block contains all datas from the blockchain such as Txs (data), and the nonce. */
        Block block;
        /** Actual block hash **/
        std::optional<std::string> hash = std::nullopt;

    public:
        BlockTemplate() = default;
        BlockTemplate(unsigned int id, unsigned int difficulty, const Block &block)
            : id(id), difficulty(difficulty), block(block) {}

        unsigned int get_id() const { return id; }
        unsigned int get_difficulty() const { return difficulty; }
        const Block &get_block() const { return block; }
        const std::optional<std::string> &get_hash() const { return hash; }

        void set_id(unsigned int new_id) { id = new_id; }
        void set_difficulty(unsigned int d) { difficulty = d; }
        void set_block(Block &b) { block = b; }
        void set_hash(const std::optional<std::string> &h) { hash = h; }
};
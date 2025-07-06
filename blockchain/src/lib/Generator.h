#pragma once

#include "BlockTemplate.h"

class Generator {
    private:
        static unsigned int get_random_difficulty();
        static std::string generate_hash();
    public:
        static BlockTemplate generate_next();
        static void save_nonce_hash(unsigned int id, uint64_t nonce, const std::string &hash);
};
#pragma once

#include "BlockTemplate.h"

class Generator {
    private:
        static unsigned int get_random_difficulty();
        static std::string generate_hash();
    public:
        static BlockTemplate generate_next();
};
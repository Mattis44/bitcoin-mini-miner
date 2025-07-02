#include <iostream>
#include <iomanip>

#include "lib/BlockLoader.h"

int main(int argc, char const *argv[])
{
    auto blocks = BlockLoader::load_all_blocks_template();
    
    for (const auto &block : blocks) {
        std::cout << "[🧱] Block #" << std::setw(6) << std::setfill('0') << block.get_id() << "\n";
        std::cout << "   [📊] Difficulty : " << block.get_difficulty() << "\n";
        std::cout << "   [📦] Data:\n";
        for (const auto &pair : block.get_block().get_data()) {
            std::cout << "     • 🔗 " << pair.first << " → " << pair.second << "\n";
        }
        std::cout << "--------------------------------------------------\n";
    }
    return 0;
}

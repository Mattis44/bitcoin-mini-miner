#include <iostream>

#include "api/api.h"
#include "lib/Miner.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    // const std::string token = argv[1];
    // std::string entries = API().get("https://hackattic.com/challenges/mini_miner/problem?access_token=" + token);


    std::pair<unsigned int, json> miner_base_data = Miner::get_start_full_data(entries);
    Miner miner(miner_base_data);
    
    while (true) {
        std::array<unsigned char, 32Ui64> current_hash = miner.get_current_hash();
        std::cout << "[🚀] Hash: " << Miner::to_hex(current_hash) << " Nonce: " << miner.get_nonce() << std::endl;
        if (Miner::has_leading_zero_bits(current_hash, miner.get_difficulty())) {
            std::cout << "[✨] Found hash: " << Miner::to_hex(current_hash) << "\n[🔑] Difficulty: " << miner.get_difficulty() << "\n[🏆] Nonce: " << miner.get_nonce() << std::endl;
            auto payload = json{{"nonce", miner.get_nonce()}};
            // API().post("https://hackattic.com/challenges/mini_miner/solve?access_token=" + token, payload);
            return 0;
        }

        miner.up_nonce();
    }

    return 0;
};
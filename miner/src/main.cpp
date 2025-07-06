#include <iostream>

#include "api/api.h"
#include "lib/Miner.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    while (true) {
        std::string entries = API().get("http://localhost:8080/mine");

        try {
            std::pair<unsigned int, json> miner_base_data = Miner::get_start_full_data(entries);
            std::cout << "\n[🔍] Starting mining with difficulty: " << miner_base_data.first << std::endl;

            Miner miner(
                miner_base_data.second["previous_hash"].get<std::string>(),
                miner_base_data.second["timestamp"].get<uint64_t>(),
                {miner_base_data.first, miner_base_data.second["data"]}
            );

            while (true) {
                std::array<unsigned char, 32Ui64> current_hash = miner.get_current_hash();
                std::cout << "[🚀] Hash: " << Miner::to_hex(current_hash)
                          << " | Nonce: " << miner.get_nonce() << std::endl;

                if (Miner::has_leading_zero_bits(current_hash, miner.get_difficulty())) {
                    std::cout << "[✨] Found hash: " << Miner::to_hex(current_hash)
                              << "\n[🔑] Difficulty: " << miner.get_difficulty()
                              << "\n[🏆] Nonce: " << miner.get_nonce() << std::endl;

                    auto payload = json{{"nonce", miner.get_nonce()}};
                    std::string response = API().post("http://localhost:8080/mine/submit", payload);
                    std::cout << "[📩] Submitted: " << response << std::endl;

                    break;
                }

                miner.up_nonce();
            }
        } catch (const std::exception &e) {
            std::cerr << "[❌] Error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    return 0;
}

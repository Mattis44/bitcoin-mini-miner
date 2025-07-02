#pragma once

#include <string>
#include <vector>
#include <variant>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>


using json = nlohmann::json;


class Miner {
    private:
        std::pair<unsigned int, json> data;
        unsigned int nonce = 0;

    public:
        Miner(std::pair<unsigned int, json> init_data) : data(std::move(init_data)) {}

        std::string get_data_txs() {
            return data.second;
        }
        unsigned int get_difficulty() {
            return data.first;
        }
        unsigned int get_nonce() {
            return nonce;
        }
        void up_nonce() {
            nonce += 1;
        }
        json get_current_full_data() {
            return {
                {"data", data.second},
                {"nonce", nonce}
            };
        }

        std::string sha256(const std::string& input) {
            unsigned char hash[SHA256_DIGEST_LENGTH];
            SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

            std::ostringstream oss;
            for (unsigned char byte : hash)
                oss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;

            return oss.str();
        }
        
        std::array<unsigned char, SHA256_DIGEST_LENGTH> get_current_hash() {
            std::string str = get_current_full_data().dump();
            std::array<unsigned char, SHA256_DIGEST_LENGTH> hash;
            SHA256(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), hash.data());
            return hash;
        }
        
        static std::string to_hex(const std::array<unsigned char, SHA256_DIGEST_LENGTH> &hash) {
            std::ostringstream oss;
            for (unsigned char byte : hash)
                oss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
            return oss.str();
        }

        static bool has_leading_zero_bits(const std::array<unsigned char, SHA256_DIGEST_LENGTH>& hash, int bits) {
            int full_bytes = bits / 8;
            int remaining_bits = bits % 8;

            for (int i = 0; i < full_bytes; ++i) {
                if (hash[i] != 0)
                    return false;
            }

            if (remaining_bits > 0) {
                unsigned char mask = 0xFF << (8 - remaining_bits);
                if ((hash[full_bytes] & mask) != 0)
                    return false;
            }

            return true;
        }


        static std::pair<unsigned int, json> get_start_full_data(const std::string &entries);


};

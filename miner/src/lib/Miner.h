#pragma once

#include <string>
#include <vector>
#include <variant>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <iostream>


using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;


class Miner {
    private:
        std::string previous_hash;
        uint64_t timestamp;
        std::pair<unsigned int, json> data;
        uint64_t nonce = 0;

    public:
        Miner(std::string prev_hash, uint64_t timestp, std::pair<unsigned int, json> init_data)
            : previous_hash(std::move(prev_hash)), timestamp(timestp), data(std::move(init_data)) {}

        std::string get_data() {
            return data.second;
        }
        unsigned int get_difficulty() {
            return data.first;
        }
        uint64_t get_nonce() {
            return nonce;
        }
        void up_nonce() {
            nonce += 1;
        }
        ordered_json get_current_full_data() {
            ordered_json j;
            j["previous_hash"] = previous_hash;
            j["timestamp"] = timestamp;
            j["data"] = data.second;
            j["nonce"] = nonce;
            return j;
        }

        std::string sha256(const std::string &input) {
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

        static bool has_leading_zero_bits(const std::array<unsigned char, SHA256_DIGEST_LENGTH> &hash, int bits) {
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

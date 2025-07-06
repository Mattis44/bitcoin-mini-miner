#pragma once

#include <sstream>
#include <string>
#include <iomanip>
#include <openssl/sha.h>
#include <nlohmann/json.hpp>
#include <iostream>

#include "Block.h"

using ordered_json = nlohmann::ordered_json;

class Hash {
    public:
        std::string sha256(const std::string &data) {
            unsigned char hash[SHA256_DIGEST_LENGTH];
            SHA256(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), hash);

            std::stringstream ss;
            for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
            }
            return ss.str();
        }

       std::string compute_block_hash(const Block &block) {
            ordered_json payload;
            payload["previous_hash"] = block.get_previous_hash();
            payload["timestamp"] = block.get_timestamp();
            payload["data"] = ordered_json::array();

            for (const auto &pair : block.get_data()) {
                payload["data"].push_back({pair.first, pair.second});
            }

            if (block.get_nonce().has_value()) {
                payload["nonce"] = block.get_nonce().value();
            }


            std::string dumped = payload.dump();
            return sha256(dumped);
        }

        bool check_difficulty(const std::string &hash, unsigned int difficulty) {
            unsigned int bits_checked = 0;

            for (char c : hash) {
                unsigned int val;
                if (c >= '0' && c <= '9') val = c - '0';
                else if (c >= 'a' && c <= 'f') val = 10 + (c - 'a');
                else if (c >= 'A' && c <= 'F') val = 10 + (c - 'A');
                else return false;

                for (int i = 3; i >= 0; --i) {
                    bool bit = (val >> i) & 1;
                    if (bit == 0) {
                        ++bits_checked;
                        if (bits_checked >= difficulty)
                            return true;
                    } else {
                        return false;
                    }
                }
            }

            return false;
        }
};
#pragma once

#include <optional>
#include <vector>
#include <string>

class Block {
    private:
        /** Contains Txs from users */
        std::vector<std::pair<std::string, int>> data;
        /** A nonce start at null because Miners have to find it. */
        std::optional<unsigned int> nonce = std::nullopt;
        /** Previous block hash, used to link blocks together */
        std::string previous_hash;
        /** Timestamp of the block creation */
        uint64_t timestamp = 0;

    public:
        Block() = default;
        Block(std::vector<std::pair<std::string, int>> &data) : data(data) {}
        Block(std::vector<std::pair<std::string, int>> &data, std::string previous_hash, uint64_t timestamp, std::optional<unsigned int> nonce)
            : data(data), previous_hash(previous_hash), timestamp(timestamp), nonce(nonce) {}

        const std::vector<std::pair<std::string, int>> get_data() const { return data; }
        const std::optional<unsigned int> get_nonce() const { return nonce; }
        const std::string &get_previous_hash() const { return previous_hash; }
        const uint64_t get_timestamp() const { return timestamp; }

        void set_data(const std::vector<std::pair<std::string, int>> &new_data) { data = new_data; }
        void set_nonce(const std::optional<unsigned int> &new_nonce) { nonce = new_nonce; }
        void set_previous_hash(const std::string &new_previous_hash) { previous_hash = new_previous_hash; }
        void set_timestamp(uint64_t new_timestamp) { timestamp = new_timestamp; }
};
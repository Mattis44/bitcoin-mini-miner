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

    public:
        Block() = default;
        Block(std::vector<std::pair<std::string, int>> &data) : data(data) {}
        Block(std::vector<std::pair<std::string, int>> &data, std::optional<unsigned int> &nonce) : data(data), nonce(nonce) {}

        const std::vector<std::pair<std::string, int>> get_data() const { return data; }
        const std::optional<unsigned int> get_nonce() const { return nonce; }

};
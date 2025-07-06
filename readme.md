# Simple Blockchain & Miner - README

## 1. Bitcoin Mining Overview

### SHA-256 and Proof of Work

Bitcoin uses a cryptographic hash function called **SHA-256**. This function maps any input to a fixed 256-bit (32-byte) output. It is deterministic, fast, and irreversible (you can't go backward).

To validate a block in Bitcoin, the system requires a **Proof of Work**:

* A miner must find a **nonce** (a number) that, when added to the block's data and hashed, produces a hash that starts with a certain number of zero bits.
* The number of leading zero bits required is called the **difficulty**.

This system ensures that mining takes computational effort, making it costly to alter the blockchain.

For example, if the difficulty is 14, the hash must start with 14 '0' bits. This means the hash must be less than `2^(256-14)`, or `2^242`, which is a very large number.
This difficulty level is adjusted periodically (every 2016 blocks) to ensure that blocks are mined at a consistent rate (approximately every 10 minutes in Bitcoin).

In real-world Bitcoin, the difficulty is around 75, which means the hash must start with 75 leading zero bits, making it extremely hard to find a valid nonce.

### Example of SHA-256 Hashing
```cpp
#include <iostream>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);
    
    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return oss.str();
}

int main() {
    std::string input = "{\"data\": [[\"abc123\", 1], [\"def456\", -2]], \"previous_hash\": \"1234abcd\", \"timestamp\": 1751792920, \"nonce\": 0}";
    std::string hash = sha256(input);
    std::cout << "SHA-256 Hash: " << hash << std::endl;
    return 0;
}
```

Here the `input` is a JSON string representing a block, including its data, previous hash, timestamp, and nonce. The nonce is initially set to 0 and will be incremented during the mining process until a valid hash is found.

So, the miner will basically make a `while(true)` loop, and add +1 to the nonce until it finds a hash that meets the difficulty requirement.

For example, if you have a block with the following data:

```json
{
  "data": [["abc123", 1], ["def456", -2]],
  "previous_hash": "1234abcd",
  "timestamp": 1751792920,
  "nonce": 0
}
```
The miner will keep incrementing the nonce and hashing the entire block data until it finds a hash that starts with the required number of leading zero bits such as `0000000000000ef0c1b2d3er4...` .

### Block Structure

Each block includes:

* `data`: a list of transactions or arbitrary pairs (e.g., key-value).
* `previous_hash`: the hash of the previous block.
* `timestamp`: when the block was created.
* `nonce`: a number miners must discover.

Miners try different nonce values until they find one that produces a hash with the required difficulty.

## 2. Project Structure

This project implements a simplified blockchain system in C++, including:

* Block generation and validation
* Mining simulation
* HTTP API using [Crow](https://github.com/CrowCpp/Crow)

/!\ The project is designed for educational purposes and does not implement all features of a full blockchain system like Bitcoin. Difficulty levels and datas are just randomly set (`Generator` class).

### Key Directories

```
blockchain/
├── src/
│   ├── lib/               # Core logic (Block, BlockTemplate, BlockLoader, Miner...)
│   ├── routes/            # API routes (status, mine, submit...)
│   ├── api/               # API client used by the miner
│   └── main.cpp           # Entry point
miner/
├── src/                  # Miner logic
├── main.cpp              # Entry point for miner
blocks/                   # JSON files containing block templates (e.g., `000001`)
```

## 3. API Endpoints

### GET `/mine`

Returns the latest block to mine:

```json
{
  "difficulty": 14,
  "block": {
    "previous_hash": "...",
    "timestamp": 1751792920,
    "data": [["abc123", 1], ["def456", -2], ...],
    "nonce": null
  }
}
```

### POST `/mine/submit`

Submits a nonce:

```json
{
  "nonce": 12500
}
```

If valid (the resulting hash meets the difficulty), the block is finalized and a new one is generated.

### GET `/status`

Returns server health and sync status.

## 4. How Mining Works in This Project

### 1. Load a Block Template

You will have a Genesis block (the first block, id: 000001) and subsequent blocks stored as JSON files in the `blocks/` directory. Each block file contains a template with a `difficulty`, `id`, and the block structure.

The miner fetches the current block via `/mine`. This includes the previous hash, timestamp, difficulty, and data.

### 2. Try Nonces

Using `get_current_full_data().dump()`, the miner generates a JSON string with all the info, including a changing nonce. It then hashes this string with SHA-256.

### 3. Check Difficulty

It checks if the hash has the required number of leading zero bits.

### 4. Submit If Valid

If the nonce is valid, the miner sends it to the server. The server verifies it and stores it permanently.

### 5. Start Over

A new block is generated, and the miner starts the process again.

## 5. Example Block JSON File

```
{
  "difficulty": 14,
  "id": 42,
  "block": {
    "nonce": null,
    "previous_hash": "1234abcd...",
    "timestamp": 1751792920,
    "data": [
      ["abc123", 1],
      ["def456", -2]
    ]
  }
}
```

## 6. Building the Project

### Requirements

* C++23
* vcpkg

### Build & Run

Using CMake (CMakeLists.txt is provided):

```bash
cd blockchain
vcpkg install
./blockchain.sh # This script sets up the environment and builds the blockchain project and runs the server

cd miner
vcpkg install
./miner.sh # This script sets up the environment and builds the miner project and runs the miner
```

## 7. Run Instructions

I provide a Miner, you can just run it and you'll see something like this:
```bash
# Here we get the difficulty from the blockchain of the current block.
[🔍] Starting mining with difficulty: 17
...
...
[🚀] Hash: 186115d6c7cba8f0d6670cebacd3074b9bf2f0eac540c1f92ebc55e5ba0966e7 | Nonce: 15079
...
...
[🚀] Hash: 186115d6c7cba8f0d6670cebacd3074b9bf2f0eac540c1f92ebc55e5ba0966e7 | Nonce: 234512
# You found the right nonce (example output)
[✨] Found hash: 000001x2f12...
```

## 8. Conclusion
This project provides a basic implementation of a blockchain and mining system in C++. It demonstrates how blocks are created, validated, and mined using SHA-256 hashing and Proof of Work. The miner continuously tries different nonces until it finds a valid one that meets the difficulty requirement, simulating the mining process in Bitcoin.

## 9. Sources
* [Bitcoin Whitepaper](https://bitcoin.org/bitcoin.pdf)
* [SHA-256 Explained](https://en.wikipedia.org/wiki/SHA-2)
* [OpenSSL SHA-256 Documentation](https://www.openssl.org/docs/man1.1.1/man3/SHA256.html)
* [Crow C++ Web Framework](https://crowcpp.org/)
* [Nlohmann JSON for Modern C++](https://github.com/nlohmann/json)
* [Boost Libraries](https://www.boost.org/)
* [CMake Documentation](https://cmake.org/documentation/)

# Mini Miner (Hackattic challenge)
This is a C++ solution for the Mini Miner challenge on [Hackattic](https://hackattic.com/challenges/mini_miner).

- CMake ≥ 3.14
- vcpkg installed (https://github.com/microsoft/vcpkg)
- `openssl`, `nlohmann-json`, `cpr` installés via vcpkg

## Installation
Clone the repository:
```bash
git clone
cd Miner
```
Install dependencies using vcpkg:
```bash
vcpkg install openssl nlohmann-json cpr
```

## Build
Run the build script with your Hackattic access token:
```bash
./build.sh YOUR_ACCESS_TOKEN
```

## Output
The program will output all tested hashes and will send the result to Hackattic:
```bash
Hash: 00056c28481de0358e0581d19f628776a3abcb387c3ec101babd758b0b843921
Found hash: 00056c28481de0358e0581d19f628776a3abcb387c3ec101babd758b0b843921
Difficulty: 13
```
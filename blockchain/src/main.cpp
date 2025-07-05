#include <iostream>
#include <iomanip>

#include "lib/BlockLoader.h"
#include "lib/Generator.h"
#include "ApiServer.h"

int main(int argc, char const *argv[])
{

    ApiServer::run(8080);

    // auto next_block = Generator::generate_next();
    // std::cout << "Next block generated with ID: " << next_block.get_id() << std::endl;
    // std::cout << "Difficulty: " << next_block.get_difficulty() << std::endl;
    // std::cout << "Previous block hash: " << next_block.get_block().get_previous_hash() << std::endl;
    // if (next_block.get_block().get_timestamp() != 0) {
    //     std::cout << "Timestamp: " << next_block.get_block().get_timestamp() << std::endl;
    // } else {
    //     std::cout << "Timestamp: Not set" << std::endl;
    // }
    // std::cout << "Block data:" << std::endl;
    // for (const auto &data : next_block.get_block().get_data()) {
    //     std::cout << "  Hash: " << data.first << ", Score: " << data.second << std::endl;
    // }

    return 0;
}

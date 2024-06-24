#include <stdlib.h>
#include <iostream>
#include <chrono>
#include "Rummikub_Tile_Drawing.h"

int rummi(int argc, char *argv[]) {
    const uint64_t heartbeat_interval =  1000000ul;
    uint64_t upperbound;

    Rummikub_Tile_Set tile_set(
        SUIT_COUNT,
        TILES_PER_SUIT,
        DUPLICATE_COUNT,
        WITH_WILDS
    );

    std::vector<std::vector<tile_t>> hands;
    uint8_t hands_analyzed;
    uint64_t stats[NUM_STRAIGHTS];

    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    std::chrono::seconds sim_duration;

    if (argc != 2) {
        std::cout << "Error: expected 1 command-line argument but was provided " << (argc - 1) << "\n";
        return EXIT_FAILURE;
    }

    upperbound = strtoul(argv[1], NULL, 0);

    for (uint8_t i = 0; i < NUM_STRAIGHTS; i++) {
        stats[i] = 0;
    }

    std::cout << "\n";
    std::cout << "======== Simulator configuration ========\n";
    std::cout << "* Upperbound:   " << std::to_string(upperbound) << "\n";
    std::cout << "* Heartbeat:    " << std::to_string(heartbeat_interval) << "\n";
    std::cout << "* Suits:        " << std::to_string(SUIT_COUNT) << "\n";
    std::cout << "* Tiles/suit:   " << std::to_string(TILES_PER_SUIT) << "\n";
    std::cout << "* Duplicates:   " << std::to_string(DUPLICATE_COUNT) << "\n";
    std::cout << "* With Wilds:   " << ((WITH_WILDS) ? "Yes" : "No") << "\n";
    std::cout << "=========================================\n";
    std::cout << "\n";

    start_time = std::chrono::high_resolution_clock::now();

    for (uint64_t i = 0, j = 0; i < upperbound; i += hands_analyzed, j += hands_analyzed) {
        hands_analyzed = Rummikub_Tile_Drawing::draw_hands_and_analyze(tile_set, stats);

        assert(hands_analyzed > 0);

        if (j >= heartbeat_interval) {
            j = 0;
            std::cout << "Heartbeat - passed " << std::to_string(i) << " iterations of " << std::to_string(upperbound) << "\n";
        }
    }

    end_time = std::chrono::high_resolution_clock::now();
    sim_duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

    std::cout << "\n";
    std::cout << "=========== Simulator results ===========\n";
    for (uint8_t i = 0; i < NUM_STRAIGHTS; i++) {
        std::cout << "Straights[" << std::to_string(i + MINIMUM_STRAIGHT) << "] = " << std::to_string(stats[i]) << "\n";
    }
    std::cout << "\n";
    std::cout << "Runtime: " << sim_duration.count() << "s\n";
    std::cout << "=========================================\n";
    std::cout << "\n";

    return EXIT_SUCCESS;
}

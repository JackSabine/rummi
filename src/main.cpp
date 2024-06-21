#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <chrono>
#include "Rummikub_Tile_Set.h"

#define TILES_DRAWN (14)
#define MINIMUM_STRAIGHT (3)

#define SUIT_COUNT (4)
#define TILES_PER_SUIT (13)
#define DUPLICATE_COUNT (2)
#define WITH_WILDS (true)
#define WITHOUT_WILDS (false)


// Highest straight can only be TILES_PER_SUIT
// e.g. TILES_PER_SUIT = 5
// 3 (MINIMUM_STRAIGHT)
// 4
// 5 (TILES_PER_SUIT)
//
// TILES_PER_SUIT - MINIMUM_STRAIGHT + 1
#define NUM_STRAIGHTS (TILES_PER_SUIT - MINIMUM_STRAIGHT + 1)

void print_hand(std::vector<tile_t> hand) {
    std::cout << "------------\n";
    for (tile_t tile : hand) {
        std::cout << Rummikub_Tile_Set::tile_index_to_suit_and_num(tile) << "\n";
    }
    std::cout << "------------\n";
}

void compute_straights(std::vector<tile_t> hand, uint64_t stats[NUM_STRAIGHTS]) {
    uint8_t current_straight;
    tile_t prev_tile;

    std::sort(hand.begin(), hand.end());

    current_straight = 0;

    for (tile_t tile : hand) {
        if (current_straight == 0) {
            // Start condition
            prev_tile = tile;
            current_straight = 1;
            continue;
        }

        if (tile == prev_tile + 1) {
            // Continue the streak;
            prev_tile = tile;
            current_straight++;
        } else if (tile == prev_tile) {
            // Same as previous tile, don't end streak but don't continue it
            continue;
        } else {
            // Not a match
            if (current_straight >= MINIMUM_STRAIGHT) {
                stats[current_straight - MINIMUM_STRAIGHT]++;
            }
            prev_tile = tile;
            current_straight = 1;
        }
    }

    // Final exit check if no streak ended
    if (current_straight >= MINIMUM_STRAIGHT) {
        stats[current_straight - MINIMUM_STRAIGHT]++;
    }
}

int main(int argc, char *argv[]) {
    const uint64_t heartbeat_interval =  1000000ul;
    uint64_t upperbound;

    Rummikub_Tile_Set tile_set(
        SUIT_COUNT,
        TILES_PER_SUIT,
        DUPLICATE_COUNT,
        WITH_WILDS
    );

    std::vector<tile_t> hand;
    uint64_t *stats;

    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    std::chrono::seconds sim_duration;

    if (argc != 2) {
        std::cout << "Error: expected 1 command-line argument but was provided " << (argc - 1) << "\n";
        return EXIT_FAILURE;
    }

    upperbound = strtoul(argv[1], NULL, 0);

    stats = new uint64_t [NUM_STRAIGHTS];
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

    for (uint64_t i = 0, j = 0; i < upperbound; i++, j++) {
        tile_set.shuffle_tiles();
        hand = tile_set.pick_first_n_tiles(TILES_DRAWN);

        compute_straights(hand, stats);

        if (j == heartbeat_interval) {
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

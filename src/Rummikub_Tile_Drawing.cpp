#include <iostream>
#include <algorithm>
#include "Rummikub_Tile_Drawing.h"

void Rummikub_Tile_Drawing::print_hand(std::vector<tile_t> hand) {
    std::cout << "------------\n";
    for (tile_t tile : hand) {
        std::cout << Rummikub_Tile_Set::tile_index_to_suit_and_num(tile) << "\n";
    }
    std::cout << "------------\n";
}

void Rummikub_Tile_Drawing::compute_straights(std::vector<tile_t> hand, uint64_t stats[NUM_STRAIGHTS]) {
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

uint8_t Rummikub_Tile_Drawing::draw_hands_and_analyze(Rummikub_Tile_Set &tile_set, uint64_t stats[NUM_STRAIGHTS]) {
    std::vector<std::vector<tile_t>> hands;
    tile_set.shuffle_tiles();
    hands = tile_set.deal_all_hands(TILES_DRAWN);

    for (auto &hand : hands) {
        compute_straights(hand, stats);
    }

    return hands.size();
}

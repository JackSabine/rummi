#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "Rummikub_Tile_Set.h"

int main(void) {
    Rummikub_Tile_Set tile_set(4, 13, 2, true);
    std::vector<tile_t> hand;

    std::cout << "----------\n";

    for (uint8_t i = 0; i < 8; i++) {
        tile_set.shuffle_tiles();
        hand = tile_set.pick_first_n_tiles(14);

        std::sort(hand.begin(), hand.end());

        for (tile_t tile : hand) {
            std::cout << tile_set.tile_index_to_suit_and_num(tile) << "\n";
        }

        std::cout << "----------\n";
    }

    return EXIT_SUCCESS;
}

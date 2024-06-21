#include <stdlib.h>
#include <iostream>
#include "Rummikub_Tile_Set.h"

int main(void) {
    Rummikub_Tile_Set tile_set(4, 13, 2, true);
    std::vector<tile_t> hand;

    tile_set.shuffle_tiles();
    hand = tile_set.pick_first_n_tiles(14);

    for (tile_t tile : hand) {
        std::cout << tile_set.tile_index_to_suit_and_num(tile) << "\n";
    }

    return EXIT_SUCCESS;
}

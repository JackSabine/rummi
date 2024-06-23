#include <vector>
#include "Rummikub_Game.h"
#include "Rummikub_Tile_Set.h"

class Rummikub_Tile_Drawing {
public:
    static void print_hand(std::vector<tile_t> hand);
    static void compute_straights(std::vector<tile_t> hand, uint64_t stats[NUM_STRAIGHTS]);
    static uint8_t draw_hands_and_analyze(Rummikub_Tile_Set &tile_set, uint64_t stats[NUM_STRAIGHTS]);
};

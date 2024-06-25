#include <inttypes.h>
#include <assert.h>
#include <vector>
#include <string>
#include <random>
#include "Rummikub_Game.h"

class Rummikub_Tile_Set {
private:
    const uint8_t suit_count;
    const uint8_t tiles_per_suit;

    const inline static uint8_t SUIT_POS = 5;
    const inline static uint8_t TILE_NUM_MASK = (1 << SUIT_POS) - 1;

    // Must leave a space between adjacent suit indices for sequence/straight calculator to work
    // e.g. SUIT_POS = 2
    // s|num
    // -+--
    // 0|00
    // 0|01
    // 0|10
    // 0|11 <- illegal
    // 1|00
    const inline static uint8_t LARGEST_TILES_PER_SUIT = (1 << SUIT_POS) - 1;

    // WILD_INDEX cannot be uint8_t_MAX and 4 * (1 << 5) = 128, so more suits will break
    const inline static uint8_t LARGEST_SUIT_COUNT = 4;

    // Add an extra "suit" on top of the last suit
    // e.g. SUIT_COUNT = 2, SUIT_POS = 2
    // s |num
    // --+--
    // 00|00
    // 00|01
    // 00|10
    // 00|11 <- illegal
    // 01|00
    // 01|01
    // 01|10
    // 01|11 <- illegal
    // 10|00 <- WILD_INDEX
    const inline static tile_t WILD_INDEX = SUIT_COUNT * (1 << SUIT_POS);

    std::vector<tile_t> tiles;

    std::random_device rd;
    std::mt19937 g;

public:
    Rummikub_Tile_Set(
        uint8_t suit_count,
        uint8_t tiles_per_suit,
        uint8_t duplicates,
        bool has_wilds
    );
    ~Rummikub_Tile_Set();

    void shuffle_tiles();
    std::vector<tile_t> pick_first_n_tiles(uint8_t n);
    std::vector<std::vector<tile_t>> deal_all_hands(uint8_t tiles_per_hand);

    static tile_t suit_and_num_to_tile_index(uint8_t suit, uint8_t num);
    static std::string tile_index_to_suit_and_num(tile_t index);
};

#include <algorithm>
#include "Rummikub_Tile_Set.h"

Rummikub_Tile_Set::Rummikub_Tile_Set(
    uint8_t suit_count,
    uint8_t tiles_per_suit,
    uint8_t duplicates,
    bool has_wilds
) : suit_count(suit_count), tiles_per_suit(tiles_per_suit) {
    assert(this->tiles_per_suit < 63); // must be a gap between groups of 64

    for (uint8_t d = 0; d < duplicates; d++) {
        for (uint8_t s = 0; s < suit_count; s++) {
            for (uint8_t t = 0; t < tiles_per_suit; t++) {
                tiles.push_back(suit_and_num_to_tile_index(s, t));
            }
        }

        if (has_wilds) tiles.push_back(WILD_INDEX);
    }
}

Rummikub_Tile_Set::~Rummikub_Tile_Set() {}

tile_t Rummikub_Tile_Set::suit_and_num_to_tile_index(uint8_t suit, uint8_t num) {
    return (suit << 6) + num;
}

std::string Rummikub_Tile_Set::tile_index_to_suit_and_num(tile_t index) {
    uint8_t suit, num;

    if (index == WILD_INDEX) return "WILD";

    suit = index >> 6;
    num = index & (0b111111u);
    return std::to_string(suit) + std::string(" : ") + std::to_string(num);
}

void Rummikub_Tile_Set::shuffle_tiles() {
    std::random_shuffle(tiles.begin(), tiles.end());
}

std::vector<tile_t> Rummikub_Tile_Set::pick_first_n_tiles(uint8_t n) {
    assert(tiles.size() >= n);
    return std::vector<tile_t>(tiles.begin(), tiles.begin() + n);
}

std::vector<std::vector<tile_t>> Rummikub_Tile_Set::deal_all_hands(uint8_t tiles_per_hand) {
    std::vector<std::vector<tile_t>> hands;
    uint16_t starting_index;
    uint8_t hands_to_deal;

    hands_to_deal = tiles.size() / tiles_per_hand;

    assert(hands_to_deal > 0);

    starting_index = 0;

    for (uint8_t h = 0; h < hands_to_deal; h++) {
        hands.push_back(std::vector<uint8_t>(
            tiles.begin() + starting_index, tiles.begin() + starting_index + tiles_per_hand
        ));

        starting_index += tiles_per_hand;
    }

    return hands;
}

#include <inttypes.h>
#include <assert.h>
#include <vector>
#include <string>

typedef uint8_t tile_t;

class Rummikub_Tile_Set {
private:
    const uint8_t suit_count;
    const uint8_t tiles_per_suit;

    const static tile_t WILD_INDEX = 255u;
    std::vector<tile_t> tiles;

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

    static tile_t suit_and_num_to_tile_index(uint8_t suit, uint8_t num);
    static std::string tile_index_to_suit_and_num(tile_t index);
};

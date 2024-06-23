#include <inttypes.h>

#pragma once

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

typedef uint8_t tile_t;

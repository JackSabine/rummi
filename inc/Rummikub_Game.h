#include <inttypes.h>

#pragma once

#define TILES_DRAWN (14)
#define MINIMUM_STRAIGHT (3)

#define SUIT_COUNT (4)
#define TILES_PER_SUIT (13)
#define DUPLICATE_COUNT (2)
#define WITH_WILDS (true)
#define WITHOUT_WILDS (false)
#define DOES_SET_HAVE_WILDS (WITH_WILDS)

// Highest straight can only be TILES_PER_SUIT
// e.g. TILES_PER_SUIT = 5
// 3 (MINIMUM_STRAIGHT)
// 4
// 5 (TILES_PER_SUIT)
//
// TILES_PER_SUIT - MINIMUM_STRAIGHT + 1
#define NUM_STRAIGHTS (TILES_PER_SUIT - MINIMUM_STRAIGHT + 1)

#define TILES_IN_SET (DUPLICATE_COUNT * ((SUIT_COUNT * TILES_PER_SUIT) + (DOES_SET_HAVE_WILDS ? 1 : 0)))
#define MAX_DEALABLE_HANDS ((TILES_IN_SET) / (TILES_DRAWN))

typedef uint8_t tile_t;

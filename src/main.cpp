#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include "Rummikub_Tile_Drawing.h"
#include "Heartbeat.h"
#include "Math_Utils.h"
#include "Common.h"

uint64_t iteration_tracker;
uint64_t games_to_play;

void heartbeat_body(void) {
    float progress = static_cast<float>(iteration_tracker);

    progress *= (100.0f / static_cast<float>(games_to_play));

    std::cout << "Heartbeat - " << std::fixed << std::setprecision(2) << progress << "% complete\n";
}

int main(int argc, char *argv[]) {
    const std::chrono::milliseconds heartbeat_interval = std::chrono::milliseconds(20000);
    uint64_t upperbound, real_hands_dealt;
    uint64_t stats[NUM_STRAIGHTS] = { 0 };

    std::chrono::system_clock::time_point start_time, end_time;

    Heartbeat heartbeat(heartbeat_body, heartbeat_interval);

    Rummikub_Tile_Set tile_set(
        SUIT_COUNT,
        TILES_PER_SUIT,
        DUPLICATE_COUNT,
        DOES_SET_HAVE_WILDS
    );

    if (argc != 2) {
        std::cout << "Error: expected 1 command-line argument but was provided " << (argc - 1) << "\n";
        return EXIT_FAILURE;
    }

    upperbound = strtoul(argv[1], NULL, 0);

    games_to_play = div_and_round_up<uint64_t>(upperbound, MAX_DEALABLE_HANDS);
    real_hands_dealt = games_to_play * MAX_DEALABLE_HANDS;

    print_table(
        "Simulator Config",
        row_data_pairs_t({
            {"Real hands dealt", std::to_string(real_hands_dealt)},
            {"Upperbound", std::to_string(upperbound)},
            {"Heartbeat", std::to_string(heartbeat_interval.count()) + " ms"}
        })
    );
    print_game_config();

    iteration_tracker = 0;
    start_time = std::chrono::high_resolution_clock::now();
    heartbeat.start();

    for (; iteration_tracker < games_to_play; iteration_tracker++) {
        Rummikub_Tile_Drawing::draw_hands_and_analyze(tile_set, stats);
    }

    heartbeat.stop();
    end_time = std::chrono::high_resolution_clock::now();

    print_results(stats, start_time, end_time);

    return EXIT_SUCCESS;
}

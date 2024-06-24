#include <stdlib.h>
#include <iostream>
#include <chrono>
#include "Rummikub_Tile_Drawing.h"
#include "Heartbeat.h"
#include "Math_Utils.h"

uint64_t iteration_tracker;
uint64_t games_to_play;

void heartbeat_body(void) {
    uint16_t total_progress;

    total_progress = static_cast<uint16_t>(static_cast<float>(iteration_tracker * 100) / games_to_play);

    std::cout << "Heartbeat - " << std::to_string(total_progress) << "% complete\n";
}

int main(int argc, char *argv[]) {
    const std::chrono::milliseconds heartbeat_interval = std::chrono::milliseconds(5000);
    uint64_t upperbound, real_hands_dealt;

    Rummikub_Tile_Set tile_set(
        SUIT_COUNT,
        TILES_PER_SUIT,
        DUPLICATE_COUNT,
        WITH_WILDS
    );

    std::vector<std::vector<tile_t>> hands;
    uint8_t hands_analyzed;
    uint64_t stats[NUM_STRAIGHTS] = { 0 };

    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    std::chrono::seconds sim_duration;

    Heartbeat heartbeat(heartbeat_body, heartbeat_interval);

    if (argc != 2) {
        std::cout << "Error: expected 1 command-line argument but was provided " << (argc - 1) << "\n";
        return EXIT_FAILURE;
    }

    upperbound = strtoul(argv[1], NULL, 0);

    games_to_play = div_and_round_up<uint64_t>(upperbound, MAX_DEALABLE_HANDS);
    real_hands_dealt = games_to_play * MAX_DEALABLE_HANDS;

    std::cout << "\n";
    std::cout << "======== Simulator configuration ========\n";
    std::cout << "* Real hands dealt:    " << std::to_string(real_hands_dealt) << "\n";
    std::cout << "\n";
    std::cout << "* Upperbound:   " << std::to_string(upperbound) << "\n";
    std::cout << "* Heartbeat:    " << std::to_string(heartbeat_interval.count()) << " ms\n";
    std::cout << "* Suits:        " << std::to_string(SUIT_COUNT) << "\n";
    std::cout << "* Tiles/suit:   " << std::to_string(TILES_PER_SUIT) << "\n";
    std::cout << "* Duplicates:   " << std::to_string(DUPLICATE_COUNT) << "\n";
    std::cout << "* With Wilds:   " << ((WITH_WILDS) ? "Yes" : "No") << "\n";
    std::cout << "=========================================\n";
    std::cout << "\n";

    start_time = std::chrono::high_resolution_clock::now();

    iteration_tracker = 0;

    heartbeat.start();

    for (; iteration_tracker < games_to_play; iteration_tracker++) {
        hands_analyzed = Rummikub_Tile_Drawing::draw_hands_and_analyze(tile_set, stats);
        assert(hands_analyzed > 0);
    }

    heartbeat.stop();

    end_time = std::chrono::high_resolution_clock::now();
    sim_duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

    std::cout << "\n";
    std::cout << "=========== Simulator results ===========\n";
    for (uint8_t i = 0; i < NUM_STRAIGHTS; i++) {
        std::cout << "Straights[" << std::to_string(i + MINIMUM_STRAIGHT) << "] = " << std::to_string(stats[i]) << "\n";
    }
    std::cout << "\n";
    std::cout << "Runtime: " << sim_duration.count() << "s\n";
    std::cout << "=========================================\n";
    std::cout << "\n";

    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <omp.h>
#include "Rummikub_Tile_Drawing.h"
#include "Heartbeat.h"
#include "Math_Utils.h"

uint64_t **thread_progresses;
int thread_count;
uint64_t games_per_thread;

void heartbeat_body(void) {
    uint16_t progress;

    progress = 0;
    for (uint8_t t = 0; t < thread_count; t++) {
        progress += static_cast<uint16_t>(static_cast<float>(*thread_progresses[t] * 100) / games_per_thread);
    }

    progress /= thread_count;
    std::cout << "Heartbeat - " << std::to_string(progress) << "% complete\n";
}

int main(int argc, char *argv[]) {
    const std::chrono::milliseconds heartbeat_interval = std::chrono::milliseconds(5000);
    uint64_t upperbound, real_hands_dealt;
    uint64_t stats_global[NUM_STRAIGHTS] = { 0 };

    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    std::chrono::seconds sim_duration;

    Heartbeat heartbeat(heartbeat_body, heartbeat_interval);

    if (argc != 3) {
        std::cout << "Error: expected 2 command-line argument but was provided " << (argc - 1) << "\n";
        std::cout << "Arguments: <simulation upperbound> <thread count>\n";
        return EXIT_FAILURE;
    }

    upperbound = strtoul(argv[1], NULL, 0);
    thread_count = atoi(argv[2]);

    omp_set_num_threads(thread_count);
    thread_progresses = new uint64_t *[thread_count];

    games_per_thread = div_and_round_up<uint64_t>(upperbound, thread_count * MAX_DEALABLE_HANDS);
    real_hands_dealt = thread_count * games_per_thread * MAX_DEALABLE_HANDS;

    std::cout << "\n";
    std::cout << "======== Simulator configuration ========\n";
    std::cout << "* OPENMP THREAD COUNT: " << std::to_string(thread_count) << "\n";
    std::cout << "* Games per thread:    " << std::to_string(games_per_thread) << "\n";
    std::cout << "* Real hands dealt:    " << std::to_string(real_hands_dealt) << "\n";
    std::cout << "*\n";
    std::cout << "* Upperbound:   " << std::to_string(upperbound) << "\n";
    std::cout << "* Heartbeat:    " << std::to_string(heartbeat_interval.count()) << " ms\n";
    std::cout << "* Suits:        " << std::to_string(SUIT_COUNT) << "\n";
    std::cout << "* Tiles/suit:   " << std::to_string(TILES_PER_SUIT) << "\n";
    std::cout << "* Duplicates:   " << std::to_string(DUPLICATE_COUNT) << "\n";
    std::cout << "* With Wilds:   " << ((DOES_SET_HAVE_WILDS) ? "Yes" : "No") << "\n";
    std::cout << "=========================================\n";
    std::cout << "\n";

    start_time = std::chrono::high_resolution_clock::now();

    #pragma omp parallel shared(stats_global, thread_progresses)
    {
        int tid;
        Rummikub_Tile_Set tile_set(
            SUIT_COUNT,
            TILES_PER_SUIT,
            DUPLICATE_COUNT,
            DOES_SET_HAVE_WILDS
        );
        uint64_t stats_per_thread[NUM_STRAIGHTS] = { 0 };
        uint64_t loop_upperbound = games_per_thread;

        uint64_t i;

        tid = omp_get_thread_num();

        thread_progresses[tid] = &i;

        // Allow all threads to set their progress alias for thread 0
        #pragma omp barrier

        if (tid == 0) {
            heartbeat.start();
        }

        for (i = 0; i < loop_upperbound; i++) {
            Rummikub_Tile_Drawing::draw_hands_and_analyze(tile_set, stats_per_thread);
        }

        #pragma omp critical
        {
            for (uint8_t i = 0; i < NUM_STRAIGHTS; i++) {
                stats_global[i] += stats_per_thread[i];
            }
        }
    }

    heartbeat.stop();

    end_time = std::chrono::high_resolution_clock::now();
    sim_duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

    std::cout << "\n";
    std::cout << "=========== Simulator results ===========\n";
    for (uint8_t i = 0; i < NUM_STRAIGHTS; i++) {
        std::cout << "Straights[" << std::to_string(i + MINIMUM_STRAIGHT) << "] = "
                  << std::to_string(stats_global[i]) << "\n";
    }
    std::cout << "\n";
    std::cout << "Runtime: " << sim_duration.count() << "s\n";
    std::cout << "=========================================\n";
    std::cout << "\n";

    return EXIT_SUCCESS;
}

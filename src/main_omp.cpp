#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <omp.h>
#include "Rummikub_Tile_Drawing.h"
#include "Heartbeat.h"
#include "Math_Utils.h"
#include "Common.h"

uint64_t **thread_progresses;
int thread_count;
uint64_t games_per_thread;

void heartbeat_body(void) {
    float progress = 0.0;

    for (uint8_t t = 0; t < thread_count; t++) {
        progress += static_cast<float>(*thread_progresses[t]);
    }

    progress *= (100.0f / (static_cast<float>(games_per_thread * thread_count)));

    std::cout << "Heartbeat - " << std::fixed << std::setprecision(2) << progress << "% complete\n";
}

int main(int argc, char *argv[]) {
    const std::chrono::milliseconds heartbeat_interval = std::chrono::milliseconds(20000);
    uint64_t upperbound, real_hands_dealt;
    uint64_t stats[NUM_STRAIGHTS] = { 0 };

    std::chrono::system_clock::time_point start_time, end_time;

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

    print_table(
        "Simulator Config",
        row_data_pairs_t({
            {"OpenMP Thread Count", std::to_string(thread_count)},
            {"Real hands dealt", std::to_string(real_hands_dealt)},
            {"Upperbound", std::to_string(upperbound)},
            {"Heartbeat", std::to_string(heartbeat_interval.count()) + " ms"}
        })
    );
    print_game_config();

    start_time = std::chrono::high_resolution_clock::now();

    #pragma omp parallel shared(stats, thread_progresses)
    {
        const int tid = omp_get_thread_num();
        Rummikub_Tile_Set tile_set(
            SUIT_COUNT,
            TILES_PER_SUIT,
            DUPLICATE_COUNT,
            DOES_SET_HAVE_WILDS
        );
        uint64_t stats_per_thread[NUM_STRAIGHTS] = { 0 };
        uint64_t i;

        thread_progresses[tid] = &i;

        // Allow all threads to set their progress alias for thread 0
        #pragma omp barrier

        if (tid == 0) {
            heartbeat.start();
        }

        for (i = 0; i < games_per_thread; i++) {
            Rummikub_Tile_Drawing::draw_hands_and_analyze(tile_set, stats_per_thread);
        }

        #pragma omp critical
        {
            for (uint8_t i = 0; i < NUM_STRAIGHTS; i++) {
                stats[i] += stats_per_thread[i];
            }
        }
    }

    heartbeat.stop();
    end_time = std::chrono::high_resolution_clock::now();

    print_results(stats, start_time, end_time);

    return EXIT_SUCCESS;
}

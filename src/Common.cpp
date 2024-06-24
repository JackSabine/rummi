#include <iostream>
#include <iomanip>
#include <inttypes.h>
#include "Common.h"

void print_table(std::string title, row_data_pairs_t rows) {
    const size_t title_border_width = 14;
    size_t title_width = (title_border_width * 2) + 2 + title.size();
    size_t header_column_width;

    header_column_width = title_width / 2;

    for (auto &row : rows) {
        header_column_width = std::max<size_t>(header_column_width, std::get<0>(row).size());
    }

    std::cout << "\n" << std::string(title_border_width, '=') << " " << title << " " << std::string(title_border_width, '=') << "\n";

    for (auto &row : rows) {
        std::cout << std::left << std::setw(header_column_width) << ("* " + std::get<0>(row)) << ": ";
        std::cout << std::get<1>(row) << "\n";
    }

    std::cout << std::string(title_width, '=') << "\n\n";
}

void print_game_config() {
    print_table(
        "Game Config",
        row_data_pairs_t({
            {"Suits",      std::to_string(SUIT_COUNT)},
            {"Tiles/suit", std::to_string(TILES_PER_SUIT)},
            {"Duplicates", std::to_string(DUPLICATE_COUNT)},
            {"With Wilds", (DOES_SET_HAVE_WILDS) ? "Yes" : "No"}
        })
    );
}
void print_results(uint64_t stats[NUM_STRAIGHTS], std::chrono::system_clock::time_point t_start, std::chrono::system_clock::time_point t_finish) {
    std::chrono::milliseconds duration;

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(t_finish - t_start);

    row_data_pairs_t rows;

    for (uint8_t i = 0; i < NUM_STRAIGHTS; i++) {
        rows.push_back({"Straight " + std::to_string(i + MINIMUM_STRAIGHT), std::to_string(stats[i])});
    }

    rows.push_back({"", ""});
    rows.push_back({"Runtime", std::to_string(duration.count()) + "ms"});

    print_table("Simulation Results", rows);
}

#include <chrono>
#include <vector>
#include <tuple>
#include <string>
#include "Rummikub_Game.h"

typedef std::vector<std::tuple<std::string, std::string>> row_data_pairs_t;

void print_game_config();
void print_table(std::string title, row_data_pairs_t rows);
void print_results(uint64_t stats[NUM_STRAIGHTS], std::chrono::system_clock::time_point t_start, std::chrono::system_clock::time_point t_finish);

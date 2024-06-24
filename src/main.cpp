#include <stdlib.h>
#include <iostream>
#include <chrono>
#include "Rummikub_Tile_Drawing.h"
#include "rummi.h"

int main(int argc, char *argv[]) {
#ifdef OMP
    rummi_omp(argc, argv);
#else
    rummi(argc, argv);
#endif

    return EXIT_SUCCESS;
}

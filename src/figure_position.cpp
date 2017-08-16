#include <cstdio>
#include <figure_position.h>

int figure_position_t::render() {
    for (int i=0; i < size_y; ++i) {
        for (int k=0; k < size_x; ++k)
            printf("%d", layout[i][k]);
        printf("\n");
    }
    return 0;
};
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "image_io.h"

void startTimer(clock_t *time);
void stopTimer(clock_t *time);

int main() {
    unsigned char **large_img, **med_img, **small_img;
    int *width =  (int*)malloc(sizeof(int));
    int *height =  (int*)malloc(sizeof(int));

    clock_t timer;
    printf("Reading files\n");
    startTimer(&timer);

    large_img = read_pgm("input/large_1024x768.pgm", width, height);
    med_img = read_pgm("input/medium_256x256.pgm", width, height);
    small_img = read_pgm("input/small_10x10.pgm", width, height);

    printf("Done reading files\n");
    stopTimer(&timer);



    free(width);
    free(height);
    return 0;
}

void startTimer(clock_t *time) {
    *time = clock();
}
void stopTimer(clock_t *time) {
    printf("Time elapsed : %f s", (*time * 1.0) / CLOCKS_PER_SEC);
}

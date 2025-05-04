#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "image_io.h"
#include "imageop_openmp_v.h"
void startTimer(clock_t *time);
void stopTimer(clock_t *time);

int main() {
    unsigned char **large_img, **med_img, **small_img;
    int width[] = {1024, 256, 10};
    int height[] = {768, 256, 10};
    clock_t timer;
    printf("Reading files\n");
    startTimer(&timer);

    large_img = read_pgm("input/large_1024x768.pgm", width + 0, height + 0);
    med_img = read_pgm("input/medium_256x256.pgm", width + 1, height + 1);
    small_img = read_pgm("input/small_10x10.pgm", width + 2, height + 2);

    printf("Done reading files\n");

    printf("Open MP ----\n");
    printf("Large file");
    startTimer(&timer);
    blur_openmp(large_img, width + 0, height + 0);
    sharpen_openmp(large_img, width + 0, height + 0);
    stopTimer(&timer);
    printf("Medium file");
    startTimer(&timer);
    blur_openmp(med_img, width + 1, height + 1);
    sharpen_openmp(med_img, width + 1, height + 1);
    stopTimer(&timer);
    printf("Small file");
    startTimer(&timer);
    blur_openmp(small_img, width + 2, height + 2);
    sharpen_openmp(small_img, width + 2, height + 2);
    stopTimer(&timer);
    return 0;
}

void startTimer(clock_t *time) {
    *time = clock();
}
void stopTimer(clock_t *time) {
    printf("Time elapsed : %f s", (*time * 1.0) / CLOCKS_PER_SEC);
}

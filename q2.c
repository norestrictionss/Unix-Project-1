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

    large_img = read_pgm_extraedges("input/large_1024x768.pgm", width + 0, height + 0);
    med_img = read_pgm_extraedges("input/medium_256x256.pgm", width + 1, height + 1);
    small_img = read_pgm_extraedges("input/small_10x10.pgm", width + 2, height + 2);

    printf("Done reading files\n");

    printf("Open MP ----\n");
    printf("Large file\n");
    startTimer(&timer);
    printf("Blur operation\n");
    write_pgm("large_blur_open.pgm", blur_openmp(large_img, width + 0, height + 0), width[0], height[0]);
    printf("Sharpen operation\n");
    write_pgm("large_sharpen_open.pgm", sharpen_openmp(large_img, width + 0, height + 0), width[0], height[0]);
    stopTimer(&timer);
    printf("Medium file\n");
    startTimer(&timer);
    write_pgm("medium_blur_open.pgm",   blur_openmp(med_img, width + 1, height + 1), width[1], height[1]);
    write_pgm("medium_sharpen_open.pgm", sharpen_openmp(med_img, width + 1, height + 1), width[1], height[1]);
    stopTimer(&timer);
    printf("Small file\n");
    startTimer(&timer);
    write_pgm("small_blur_open.pgm", blur_openmp(small_img, width + 2, height + 2), width[2], height[2]);
    write_pgm("small_sharpen_open.pgm", sharpen_openmp(small_img, width + 2, height + 2), width[2], height[2]);
    stopTimer(&timer);
    return 0;
}

void startTimer(clock_t *time) {
    *time = clock();
}
void stopTimer(clock_t *time) {
    printf("Time elapsed : %f s\n", (*time * 1.0) / CLOCKS_PER_SEC);
}

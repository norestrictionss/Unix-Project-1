#include <stdio.h>
#include <time.h>

#include "image_io.h"
#include "imageop_openmp_v.h"
#include "imageop_pthread_v.h"
void startTimer(clock_t *time);
void stopTimer(clock_t *time);

int main() {
    unsigned char **large_img, **med_img, **small_img, **result_img;
    int width[] = {1024, 256, 10};
    int height[] = {768, 256, 10};
    clock_t timer;

    printf("Enter the number of threads:");
    int num_thread;
    fscanf(stdin, "%d", &num_thread);

    printf("Reading files\n");

    large_img = read_pgm_extraedges("input/large_1024x768.pgm", width + 0, height + 0);
    med_img = read_pgm_extraedges("input/medium_256x256.pgm", width + 1, height + 1);
    small_img = read_pgm_extraedges("input/small_10x10.pgm", width + 2, height + 2);

    printf("Done reading files\n");

    printf("Open MP ----\n");
    printf("Large file\n");
    startTimer(&timer);
    printf("Blur operation\n");
    result_img = blur_openmp(large_img, width + 0, height + 0);
    stopTimer(&timer);
    write_pgm("large_blur_open.pgm", result_img, width[0], height[0]);
    free_image(result_img, height[0]);

    startTimer(&timer);
    printf("Sharpen operation\n");
    result_img = sharpen_openmp(large_img, width + 0, height + 0);
    stopTimer(&timer);
    write_pgm("large_sharpen_open.pgm", result_img, width[0], height[0]);
    free_image(result_img, height[0]);

    printf("Medium file\n");
    startTimer(&timer);
    printf("Blur operation\n");
    result_img = blur_openmp(med_img, width + 1, height + 1);
    stopTimer(&timer);
    write_pgm("medium_blur_open.pgm", result_img, width[1], height[1]);
    free_image(result_img, height[1]);

    startTimer(&timer);
    printf("Sharpen operation\n");
    result_img = sharpen_openmp(med_img, width + 1, height + 1);
    stopTimer(&timer);
    write_pgm("medium_sharpen_open.pgm", result_img, width[1], height[1]);
    free_image(result_img, height[1]);

    printf("Small file\n");
    printf("Blur operation\n");
    startTimer(&timer);
    result_img = blur_openmp(small_img, width + 2, height + 2);
    stopTimer(&timer);
    write_pgm("small_blur_open.pgm", result_img, width[2], height[2]);
    free_image(result_img, height[2]);

    startTimer(&timer);
    result_img = sharpen_openmp(small_img, width + 2, height + 2);
    printf("Sharpen operation\n");
    write_pgm("small_sharpen_open.pgm", result_img, width[2], height[2]);
    free_image(result_img, height[2]);
    stopTimer(&timer);


    printf("pthread ----\n");
    printf("Large file\n");
    startTimer(&timer);
    printf("Blur operation\n");
    result_img = blur_pthread(large_img, width + 0, height + 0, num_thread);
    stopTimer(&timer);
    write_pgm("large_blur_pthread.pgm", result_img, width[0], height[0]);
    free_image(result_img, height[0]);

    startTimer(&timer);
    printf("Sharpen operation\n");
    result_img = sharpen_pthread(large_img, width + 0, height + 0, num_thread);
    stopTimer(&timer);
    write_pgm("large_sharpen_pthread.pgm", result_img, width[0], height[0]);
    free_image(result_img, height[0]);

    printf("Medium file\n");
    startTimer(&timer);
    printf("Blur operation\n");
    result_img = blur_pthread(med_img, width + 1, height + 1, num_thread);
    stopTimer(&timer);
    write_pgm("medium_blur_pthread.pgm", result_img, width[1], height[1]);
    free_image(result_img, height[1]);

    startTimer(&timer);
    printf("Sharpen operation\n");
    result_img = sharpen_pthread(med_img, width + 1, height + 1, num_thread);
    stopTimer(&timer);
    write_pgm("medium_sharpen_pthread.pgm", result_img, width[1], height[1]);
    free_image(result_img, height[1]);

    printf("Small file\n");
    printf("Blur operation\n");
    startTimer(&timer);
    result_img = blur_pthread(small_img, width + 2, height + 2, num_thread);
    stopTimer(&timer);
    write_pgm("small_blur_pthread.pgm", result_img, width[2], height[2]);
    free_image(result_img, height[2]);

    startTimer(&timer);
    result_img = sharpen_pthread(small_img, width + 2, height + 2, num_thread);
    printf("Sharpen operation\n");
    write_pgm("small_sharpen_pthread.pgm", result_img, width[2], height[2]);
    free_image(result_img, height[2]);
    stopTimer(&timer);

    return 0;
}

void startTimer(clock_t *time) {
    *time = clock();
}
void stopTimer(clock_t *time) {
    printf("Time elapsed : %f s\n", (clock() - *time * 1.0) / CLOCKS_PER_SEC);
}

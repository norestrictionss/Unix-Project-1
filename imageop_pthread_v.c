#include <stdlib.h>
#include <pthread.h>

#include "imageop_pthread_v.h"
struct thread_data {
    unsigned char** image;
    unsigned char** image_new;
    const int* width;
    int height_start;
    int height_end;
};

void *blur_image(void *threadarg);
void *sharpen_image(void *threadarg);
unsigned char** blur_pthread(unsigned char** image, const int* width, const int* height, int num_threads) {
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);

    struct thread_data *mydata = (struct thread_data *)malloc(sizeof(struct thread_data) * num_threads);

    unsigned char **image_new =
        (unsigned char **)calloc((*height), sizeof(unsigned char *));

    // Initialize the array
    for (int i = 0; i < *height; i++)
        image_new[i] = (unsigned char *)calloc((*width), sizeof(unsigned char));

    for (int i = 0; i < num_threads; i++) {
        mydata -> image = image;
        mydata -> image_new = image_new;
        mydata -> width = width;
        mydata -> height_start = i * (*height / num_threads) + 1;
        if ((i == num_threads ) - 1)
            mydata -> height_end = (*height / num_threads) + (*height % num_threads);
        else
            mydata -> height_end = (*height / num_threads);

        pthread_create(&threads[i], NULL, blur_image, (void*) mydata);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return image_new;
}
void *blur_image(void *threadarg) {
    struct thread_data *mydata = (struct thread_data*)threadarg;
    unsigned char** image = mydata -> image;
    unsigned char** image_new = mydata -> image_new;
    const int* width = mydata -> width;
    int height_start = mydata -> height_start;
    int height_end = mydata -> height_end;

    for (int i = height_start; i < height_end + 1; i++) {
        for (int j = 1; j < *width + 1; j++) {
            image_new[i - 1][j - 1] = (image[i - 1][j - 1] + image[i - 1][j] + image[i - 1][j + 1] +
                                       image[i][j - 1] + image[i][j] + image[i][j + 1] +
                                       image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1]) / 9;
        }
    }

    pthread_exit(NULL);
}
unsigned char** sharpen_pthread(unsigned char** image, const int* width, const int* height, int num_threads) {
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);

    struct thread_data *mydata = (struct thread_data *)malloc(sizeof(struct thread_data) * num_threads);

    unsigned char **image_new =
        (unsigned char **)calloc((*height), sizeof(unsigned char *));

    // Initialize the array
    for (int i = 0; i < *height; i++)
        image_new[i] = (unsigned char *)calloc((*width), sizeof(unsigned char));

    for (int i = 0; i < num_threads; i++) {
        mydata -> image = image;
        mydata -> image_new = image_new;
        mydata -> width = width;
        mydata -> height_start = i * (*height / num_threads) + 1;
        if ((i == num_threads ) - 1)
            mydata -> height_end = (*height / num_threads) + (*height % num_threads);
        else
            mydata -> height_end = (*height / num_threads);

        pthread_create(&threads[i], NULL, sharpen_image, (void*) mydata);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return image_new;
}
void *sharpen_image(void *threadarg) {
    struct thread_data *mydata = (struct thread_data*)threadarg;
    unsigned char** image = mydata -> image;
    unsigned char** image_new = mydata -> image_new;
    const int* width = mydata -> width;
    int height_start = mydata -> height_start;
    int height_end = mydata -> height_end;

    for (int i = height_start; i < height_end + 1; i++) {
        for (int j = 1; j < *width + 1; j++) {
            int temp = (0 - image[i - 1][j] + 0 +
                        - image[i][j - 1] + 5 *  image[i][j] - image[i][j + 1] +
                        0 - image[i + 1][j] + 0);
            if (temp < 0)
                temp = 0;
            else if (temp > 255)
                temp = 255;
            image_new[i - 1][j - 1] = temp;
        }
    }

    pthread_exit(NULL);
}

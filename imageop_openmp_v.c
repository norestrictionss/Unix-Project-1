#include <stdlib.h>
#include <omp.h>
#include "imageop_openmp_v.h"
unsigned char** blur_openmp(unsigned char** image, const int* width, const int* height) {
    unsigned char **image_new =
        (unsigned char **)calloc((*height), sizeof(unsigned char *));

    // Initialize the array
    for (int i = 0; i < *height; i++)
        image_new[i] = (unsigned char *)calloc((*width), sizeof(unsigned char));

    // Calculate the middle part
    #pragma omp parallel for firstprivate(image_new, image)
    for (int i = 1; i < *height + 1; i++) {
        for (int j = 1; j < *width + 1; j++) {
            image_new[i - 1][j - 1] = (image[i - 1][j - 1] + image[i - 1][j] + image[i - 1][j + 1] +
                                       image[i][j - 1] + image[i][j] + image[i][j + 1] +
                                       image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1]) / 9;
        }
    }

    return image_new;
}
unsigned char** sharpen_openmp(unsigned char** image, const int* width, const int* height) {
    unsigned char **image_new =
        (unsigned char **)calloc((*height), sizeof(unsigned char *));

    // Initialize the array
    for (int i = 0; i < *height; i++)
        image_new[i] = (unsigned char *)calloc((*width), sizeof(unsigned char));

    // Calculate the middle part
    #pragma omp parallel for firstprivate(image_new, image)
    for (int i = 1; i < *height + 1; i++) {
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

    return image_new;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "imageop_openmp_v.h"
unsigned char** blur_openmp(unsigned char** image, int* width, int* height) {

    unsigned char **image_new =
        (unsigned char **)malloc((*height) * sizeof(unsigned char *));

    // Initialize the array
    #pragma omp parallel for shared(image_new)
    for (int i = 0; i < *height; i++)
        image_new[i] = (unsigned char *)malloc((*width) * sizeof(unsigned char));

    // Calculate the edges
    #pragma omp parallel for shared(image_new, image)
    for (int k = 0; k < 4; k++) {
        int i,j;
        switch (k) {
        case 0:
            // Starting point
            i = 0;
            j = 0;
            // Corner
            image_new[i][j] = (0 + 0 + 0 +
                               0 + image[i][j] + image[i + 1][j] +
                               0 + image[i][j + 1] + image[i + 1][j + 1]) / 9;
            // Edges excluding the corners
            for (i = 1; i < *height && j < *width; i++) {
                image_new[i][j] = (0 + 0 + 0 +
                                   image[i][j - 1] + image[i][j] + image[i][j + 1] +
                                   image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1]) / 9;
            }
            break;
        case 1:
            // Starting point
            i = *height - 1;
            j = 0;
            // Corner
            image_new[i][j] = (0 + 0 + 0 +
                               image[i - 1][j] + image[i][j] + 0 +
                               image[i - 1][j + 1] + image[i][j + 1] + 0) / 9;
            // Edges excluding the corners
            for (int j = 1; i < *height && j < *width; j++) {
                image_new[i][j] = (image[i - 1][j - 1] + image[i - 1][j] + 0 +
                                   image[i][j - 1] + image[i][j] + 0 +
                                   image[i + 1][j - 1] + image[i + 1][j] + 0) / 9;
            }
            break;
        case 2:
            // Starting point
            i = 0;
            j = 0;
            // Corner
            image_new[i][j] = (image[i - 1][j - 1] + image[i][j - 1] + image[i + 1][j - 1] +
                               image[i - 1][j] + image[i][j] + image[i + 1][j] +
                               image[i - 1][j + 1] + image[i][j + 1] + image[i + 1][j + 1]) / 9;
            // Edges excluding the corners
            for (int j = 1; i < *height && j < *width; j++) {
                image_new[i][j] = (image[i - 1][j - 1] + image[i - 1][j] + image[i - 1][j + 1] +
                                   image[i][j - 1] + image[i][j] + image[i][j + 1] +
                                   image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1]) / 9;
            }
            break;
        case 3:
            // Starting point
            i = 0;
            j = 0;
            // Corner
            image_new[i][j] = (image[i - 1][j - 1] + image[i][j - 1] + image[i + 1][j - 1] +
                               image[i - 1][j] + image[i][j] + image[i + 1][j] +
                               image[i - 1][j + 1] + image[i][j + 1] + image[i + 1][j + 1]) / 9;
            // Edges excluding the corners
            for (int j = 1; i < *height && j < *width; j++) {
                image_new[i][j] = (image[i - 1][j - 1] + image[i - 1][j] + image[i - 1][j + 1] +
                                   image[i][j - 1] + image[i][j] + image[i][j + 1] +
                                   image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1]) / 9;
            }
            break;

        }
    }


    // Calculate the middle part
    #pragma omp parallel for shared(image_new, image)
    for (int i = 1; i < *height - 1; i++) {
        for (int j = 1; j < *width - 1; j++) {
            image_new[i][j] = (image[i - 1][j - 1] + image[i - 1][j] + image[i - 1][j + 1] +
                               image[i][j - 1] + image[i][j] + image[i][j + 1] +
                               image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1]) / 9;
        }
    }

    return image_new;
}
unsigned char** sharpen_openmp(unsigned char** image, int* width, int* height) {
}

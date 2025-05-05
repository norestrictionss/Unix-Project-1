#include "image_io.h"

#include <stdio.h>
#include <stdlib.h>

unsigned char **read_pgm(const char *filename, int *width, int *height) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char format[3];
    fscanf(fp, "%s", format);
    if (format[0] != 'P' || format[1] != '2') {
        printf("Unsupported file format!\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d %d", width, height);
    int max_value;
    fscanf(fp, "%d", &max_value);

    unsigned char **image =
        (unsigned char **)malloc((*height) * sizeof(unsigned char *));
    for (int i = 0; i < *height; i++) {
        image[i] = (unsigned char *)malloc((*width) * sizeof(unsigned char));
        for (int j = 0; j < *width; j++) {
            int pixel;
            fscanf(fp, "%d", &pixel);
            image[i][j] = (unsigned char)pixel;
        }
    }
    fclose(fp);
    return image;
}

unsigned char **read_pgm_extraedges(const char *filename, int *width, int *height) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char format[3];
    fscanf(fp, "%s", format);
    if (format[0] != 'P' || format[1] != '2') {
        printf("Unsupported file format!\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d %d", width, height);
    int max_value;
    fscanf(fp, "%d", &max_value);

    unsigned char **image =
        (unsigned char **)calloc((*height + 2), sizeof(unsigned char *));
    image[0] = (unsigned char *)calloc((*width + 2), sizeof(unsigned char));
    for (int i = 1; i <= *height; i++) {
        image[i] = (unsigned char *)calloc((*width + 2), sizeof(unsigned char));
        for (int j = 1; j <= *width; j++) {
            int pixel;
            fscanf(fp, "%d", &pixel);
            image[i][j] = (unsigned char)pixel;
        }
    }
    image[*height + 1] = (unsigned char *)calloc((*width + 2), sizeof(unsigned char));
    fclose(fp);
    return image;
}

void write_pgm(const char *filename, unsigned char **image, int width,
               int height) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error writing file");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "P2\n%d %d\n255\n", width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(fp, "%d ", image[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void free_image(unsigned char **image, int height) {
    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);
}



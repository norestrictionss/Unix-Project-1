
#ifndef IMAGE_IO_H
#define IMAGE_IO_H

unsigned char** read_pgm(const char* filename, int* width, int* height);
void write_pgm(const char* filename, unsigned char** image, int width, int height);
void free_image(unsigned char** image, int height);

#endif

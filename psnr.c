#include "stb_image.h"
#include "stb_image_write.h"
#include <math.h>
#include <stdio.h>
#include <utils.h>

double psnr(const char *first_image_path, const char *second_image_path) {

    int width_image1, height_image1, comp_image1;
    int width_image2, height_image2, comp_image2;

    unsigned char *first_photo = stbi_load(first_image_path, &width_image1, &height_image1, &comp_image1, 0);
    unsigned char *second_photo = stbi_load(second_image_path, &width_image2, &height_image2, &comp_image2, 0);

    if (!first_photo || !second_photo) {
        printf("Error: Could not load images.\n");
        if (first_photo) stbi_image_free(first_photo);
        if (second_photo) stbi_image_free(second_photo);
        return -1.0;
    }

    if (width_image1 != width_image2 || height_image1 != height_image2 || comp_image1 != comp_image2) {
        printf("Image sizes don't match\n");
        stbi_image_free(first_photo);
        stbi_image_free(second_photo);
        return -1.0;
    }

    long long int total_byte = width_image1 * height_image1 * comp_image1;
    double sum = 0;
    for (long long int i = 0; i < total_byte; i++) {
        double diff = first_photo[i] - second_photo[i];
        sum += diff * diff;
    }

    double mse = sum / total_byte;

    double result;

    if (mse == 0){
        printf("Images are the same.\n");
        return INFINITY;
    }

    result = 10.0 * log10(255.0*255.0/mse);

    stbi_image_free(first_photo);
    stbi_image_free(second_photo);

    return result;
}
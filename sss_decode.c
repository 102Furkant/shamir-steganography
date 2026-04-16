#include "stb_image.h"
#include "stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sss.h>
#include <lagrange.h>
#include <utils.h>

extern int N, K;  // N: total number of shares, K: threshold and provided by the user in main

int sss_decode(char **filenames) {

    unsigned char **encoded_images = malloc(K * sizeof(unsigned char *));   // Allocates memory for K share images
    if (!encoded_images) {
        printf("Memory allocation error!\n");
        return 1;
    }

    int width, height, comp;
    for (int i = 0; i < K; i++){   // Loads each share image into memory
        encoded_images[i] = stbi_load(filenames[i], &width, &height, &comp, 0);
        if (!encoded_images[i]) {
            printf("Failed to load image: %s\n", filenames[i]);
            for (int k = 0; k < i; k++) stbi_image_free(encoded_images[k]);
            free(encoded_images);
            return 1;
        }
    }

    size_t total_byte = width * height * comp;

    int *x_values = malloc(K * sizeof(int));

    for (int i = 0; i < K; i++) {
        x_values[i] = read_share_index(encoded_images[i], total_byte);
    }

    

    unsigned char *shuffled_image = malloc(total_byte * sizeof(char));

    int *y_values = malloc(K * sizeof(int));

    for (int i = 0; i < total_byte; i++) {   // Recovers each byte using Lagrange interpolation
        for (int j = 0; j < K; j++){   // Collects y values (share bytes) for the current byte index
            y_values[j] = encoded_images[j][i] % PRIME_NUMBER;
        }
        shuffled_image[i] = lagrange_interpolation(x_values, y_values);
    }

    size_t seed = 12345;

    size_t *perm = malloc(total_byte * sizeof(size_t));
    srand(seed);
    generate_permutation(perm, total_byte);

    size_t *inv_perm = malloc(total_byte * sizeof(size_t));
    invert_permutation(perm, inv_perm, total_byte);

    unsigned char *result_image = malloc(total_byte);

    // Reverses the permutation to restore the original byte order
    reverse_permutation((const char *)shuffled_image, (char *)result_image, inv_perm, total_byte);

    stbi_write_png("decoded_photo.png", width, height, comp, result_image, width * comp);

    free(y_values);
    for (int i = 0; i < K; i++) {
        stbi_image_free(encoded_images[i]);
    }
    free(encoded_images);
    free(result_image);
    free(shuffled_image);
    free(perm);
    free(inv_perm);

    return 0;    
}

static int read_share_index(unsigned char *img, size_t total_byte) {  // Static because there is another function with the same body but a different name in stego_decode.c
    int share_index = 0;
    for (int i = 0; i < 3; i++)
        share_index = (share_index << 1) | (img[total_byte - 3 + i] & 1);
    return share_index;
}
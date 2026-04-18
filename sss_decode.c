#include <stdio.h>
#include <stdlib.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "sss.h"
#include "lagrange.h"
#include "utils.h"

extern int N, K;  // N: total number of shares, K: threshold and provided by the user in main

int sss_decode(char **share_paths, char *result_image_name, int *x_values) {

    unsigned char **input_shares = malloc(K * sizeof(unsigned char *));   // Allocates memory for K share images
    if (!input_shares) {
        printf("Memory allocation error!\n");
        return 1;
    }
    int width, height, comp;
    input_shares[0] = stbi_load(share_paths[0], &width, &height, &comp, 0);  // First share loaded manuelly because of setting total byte

    if (!input_shares[0]) {
        printf("Failed to load image: %s\n", share_paths[0]);
            free(input_shares);
            return 1;
    }

    size_t total_byte = width * height * comp;

    for (int k_index = 1; k_index < K; k_index++){   // Loads each share image into memory
        input_shares[k_index] = stbi_load(share_paths[k_index], &width, &height, &comp, 0);

        if (!input_shares[k_index]) {
            printf("Failed to load image: %s\n", share_paths[k_index]);
            for (int j = 0; j < k_index; j++) stbi_image_free(input_shares[j]);
            free(input_shares);
            return 1;
        }

        if (total_byte != width * height * comp) {
            printf("Image sizes don't match!\n");
            for (int i = 0; i < K; i++) {
                stbi_image_free(input_shares[i]);
            }
            free(input_shares);
            return 1;
        }
   
    }

    unsigned char *shuffled_image = malloc(total_byte * sizeof(unsigned char));

    int *y_values = malloc(K * sizeof(int));

    for (size_t byte_index = 0; byte_index < total_byte; byte_index++) {   // Recovers each byte using Lagrange interpolation
        for (int k_index = 0; k_index < K; k_index++){   // Collects y values (share bytes) for the current byte index
            y_values[k_index] = input_shares[k_index][byte_index] % PRIME_NUMBER;
        }
        shuffled_image[byte_index] = lagrange_interpolation(x_values, y_values);
    }

    size_t seed = 12345;

    size_t *permutation = malloc(total_byte * sizeof(size_t));
    srand(seed);
    generate_permutation(permutation, total_byte);

    size_t *inverse_permutation = malloc(total_byte * sizeof(size_t));
    invert_permutation(permutation, inverse_permutation, total_byte);

    unsigned char *result_image = malloc(total_byte);

    // Reverses the permutation to restore the original byte order
    reverse_permutation((const char *)shuffled_image, (char *)result_image, inverse_permutation, total_byte);

    stbi_write_png(result_image_name, width, height, comp, result_image, width * comp);

    free(y_values);
    for (int i = 0; i < K; i++) stbi_image_free(input_shares[i]);
    free(input_shares);
    free(result_image);
    free(shuffled_image);
    free(permutation);
    free(inverse_permutation);

    return 0;    
}
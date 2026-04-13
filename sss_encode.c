#include "stb_image.h"
#include "stb_image_write.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sss.h>
#include <utils.h>

extern int N, K;   // N: total number of shares, K: threshold and provided by the user in main

int sss_encode(const char *secret_image_path, char **sss_encoded_image_names, size_t seed) {

    int *polynomial_coefficients = (int *)malloc(K * sizeof(int));   // Holds the polynomial coefficients

    int image_width, image_height, image_comp;
    unsigned char *main_image = stbi_load(secret_image_path, &image_width, &image_height, &image_comp, 0);
    
    if (!main_image) {
        printf("Failed to load images.\n");
        free(polynomial_coefficients);
        return 1;
    }

    size_t image_total_byte = image_width * image_height * image_comp;

    // To avoid issues with white colors; since 253 % 251 = 2 % 251, a value of 253 would be calculated as 2 during decoding. This step is to eliminate that problem
    for (int i = 0; i < image_total_byte; i++)   if (main_image[i] > PRIME_NUMBER - 1) main_image[i] = PRIME_NUMBER - 1;

    size_t *perm = malloc(image_total_byte * sizeof(size_t));

    if (!perm) {
        stbi_image_free(main_image);
        free(polynomial_coefficients);
        return 1;
    }

    srand(seed);
    generate_permutation(perm, image_total_byte);

    unsigned char *shuffled = malloc(image_total_byte);

    if (!shuffled) {
        stbi_image_free(main_image);
        free(polynomial_coefficients);
        free(perm);
        return 1;
    }
    
    apply_permutation((const char *)main_image, (char *)shuffled, perm, image_total_byte);

    unsigned char *encoded_images = (unsigned char *)malloc(N * image_width * image_height * image_comp);  // Allocates memory for N images
 
    if (!encoded_images) {
        printf("Memory allocation error!\n");
        stbi_image_free(main_image);
        free(polynomial_coefficients);
        free(shuffled);
        free(perm);
        return 1;
    }

    srand(time(NULL));

    for (size_t byte_index = 0; byte_index < image_width * image_height * image_comp; byte_index++) {   // The following will be applied to each byte
        generate_random_polynomial_coefficient(polynomial_coefficients); 

        polynomial_coefficients[0] = (int)shuffled[byte_index];   // The constant term of the polynomial (x^0) will be the secret value (the byte value)

        encode_byte(polynomial_coefficients, encoded_images, byte_index, image_total_byte);
    }
    
    for (int i = 0; i < N; i++) {
        stbi_write_png(sss_encoded_image_names[i], image_width, image_height, image_comp, encoded_images + (i * image_width * image_height * image_comp), image_width * image_comp);
        printf("%s created.\n", sss_encoded_image_names[i]);
    }

    stbi_image_free(main_image);
    free(encoded_images);
    free(polynomial_coefficients);
    free(shuffled);
    free(perm);
    
    return 0;
}


void generate_random_polynomial_coefficient(int *polynomial_coefficients) {
    for (int i = 1; i < K; i++) {
        polynomial_coefficients[i] = rand() % 51;
    }
}

long long int int_pow(int base, int exponent) {
    if (exponent == 0) return 1;
    long long int result = 1;
    for (int i = 0; i < exponent; i++) result *= base;
    return result;
}

/* polynomial_coefficients = (115, 17, 41) means 115*x^0, 17*x^1, 41*x^2
   so coeff_index serves as both the array index and the exponent. */
void encode_byte(int *polynomial_coefficients, unsigned char *encoded_images, size_t byte_index, size_t image_total_byte) {
    for (int share_index = 0; share_index < N; share_index++) {   // Sets the i-th pixel for each image (for N images)
        int sum = 0;
        for (int coeff_index = 0; coeff_index < K; coeff_index++)   // The for loop that constructs the polynomial and writes to the images

            sum = (sum + polynomial_coefficients[coeff_index] * int_pow(share_index + 1, coeff_index)) % PRIME_NUMBER; // Applying % PRIME_NUMBER (251) in each step to prevent overflow and stay within the finite field

        encoded_images[share_index * image_total_byte + byte_index] = sum;  // Stores the calculated sum (share) into the corresponding byte index of the specific image
    }
}
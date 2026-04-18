#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "sss.h"
#include "utils.h"

extern int N, K;   // N: total number of shares, K: threshold and provided by the user in main

int sss_encode(const char *secret_image_path, const char *destination_folder_path) {

    int *polynomial_coefficients = malloc(K * sizeof(int));   // Holds the polynomial coefficients

    int width, height, comp;
    unsigned char *secret_image = stbi_load(secret_image_path, &width, &height, &comp, 0);
    
    if (!secret_image) {
        printf("Failed to load images.\n");
        free(polynomial_coefficients);
        return 1;
    }

    size_t total_byte = width * height * comp;

    // To avoid issues with white colors; since 253 % 251 = 2 % 251, a value of 253 would be calculated as 2 during decoding. This step is to eliminate that problem
    for (int byte_index = 0; byte_index < total_byte; byte_index++)   if (secret_image[byte_index] > PRIME_NUMBER - 1) secret_image[byte_index] = PRIME_NUMBER - 1;

    size_t seed = 12345;

    size_t *permutation = malloc(total_byte * sizeof(size_t));

    if (!permutation) {
        stbi_image_free(secret_image);
        free(polynomial_coefficients);
        return 1;
    }

    srand(seed);
    generate_permutation(permutation, total_byte);

    unsigned char *shuffled_image = malloc(total_byte);

    if (!shuffled_image) {
        stbi_image_free(secret_image);
        free(polynomial_coefficients);
        free(permutation);
        return 1;
    }
    
    apply_permutation((const char *)secret_image, (char *)shuffled_image, permutation, total_byte);

    unsigned char *all_shares_buffer = (unsigned char *)malloc(N * total_byte);  // Allocates memory for N images
 
    if (!all_shares_buffer) {
        printf("Memory allocation error!\n");
        stbi_image_free(secret_image);
        free(polynomial_coefficients);
        free(shuffled_image);
        free(permutation);
        return 1;
    }

    srand(time(NULL));

    for (size_t byte_index = 0; byte_index < total_byte; byte_index++) {
        generate_random_polynomial_coefficient(polynomial_coefficients); 

        polynomial_coefficients[0] = (int)shuffled_image[byte_index];   // The constant term of the polynomial (x^0) will be the secret value (the byte value)

        encode_byte(polynomial_coefficients, all_shares_buffer, byte_index, total_byte);
    }
    

    for (int n_index = 0; n_index < N; n_index++) {
        char encoded_image_name[128] = {};
        generate_share_path(encoded_image_name, destination_folder_path, n_index);
        stbi_write_png(encoded_image_name, width, height, comp, all_shares_buffer + (n_index * total_byte), width * comp);
        printf("%s created.\n", encoded_image_name);
    }

    stbi_image_free(secret_image);
    free(all_shares_buffer);
    free(polynomial_coefficients);
    free(shuffled_image);
    free(permutation);
    
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
void encode_byte(int *polynomial_coefficients, unsigned char *all_shares_buffer, size_t byte_index, size_t total_byte) {
    for (int share_index = 0; share_index < N; share_index++) {   // Sets the i-th pixel for each image (for N images)
        int sum = 0;
        for (int coeff_index = 0; coeff_index < K; coeff_index++)   // The for loop that constructs the polynomial and writes to the images

            sum = (sum + polynomial_coefficients[coeff_index] * int_pow(share_index + 1, coeff_index)) % PRIME_NUMBER; // Applying % PRIME_NUMBER (251) in each step to prevent overflow and stay within the finite field

        all_shares_buffer[share_index * total_byte + byte_index] = sum;  // Stores the calculated sum (share) into the corresponding byte index of the specific image
    }
}

void generate_share_path(char *output_buffer, const char *destination_folder_path, int index) {
    sprintf(output_buffer, "%s%cshare%d.png", destination_folder_path, PATH_SEP, index + 1);
}
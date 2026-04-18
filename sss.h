#ifndef HEADER_SSS_H
#define HEADER_SSS_H

#ifdef _WIN32
    #define PATH_SEP '\\'
#else
    #define PATH_SEP '/'
#endif

/* We use 251 because it is the largest prime number less than 255.
This allows us to keep pixel values as close to their original range as possible while minimizing data loss during the encoding process.*/
#define PRIME_NUMBER 251

int sss_encode(const char *secret_image_path, const char *destination_folder_path);

void generate_random_polynomial_coefficient(int *polynomial_coefficients);

long long int int_pow(int base, int exponent);

void encode_byte(int *polynomial_coefficients, unsigned char *all_shares_buffer,, size_t byte_index, size_t image_total_byte);

void generate_share_path(char *output_buffer, const char *destination_folder_path, int index);

int sss_decode(char **filenames, char *result_image_name, int *x_values);

#endif
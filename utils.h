#ifndef HEADER_UTILS_H
#define HEADER_UTILS_H

#include <stddef.h>

void generate_permutation(size_t *perm, size_t n);
void apply_permutation(const char *input, char *shuffled, size_t *perm, size_t n);
void invert_permutation(size_t *perm, size_t *inv_perm, size_t n);
void reverse_permutation(const char *shuffled, char *original, size_t *inv_perm, size_t n);

double calculate_psnr(const char *first_photo_path, const char *second_photo_path);

#endif
#include <lagrange.h>


extern int N, K;  // N: total number of shares, K: threshold and provided by the user in main

// Evaluates the polynomial at x = 0 to recover the secret value
int lagrange_interpolation(int *x_values, int *y_values) {
    int secret = 0;
    
    for (int i = 0; i < K; i++) {
        int term = y_values[i];
        for (int j = 0; j < K; j++) {
            if (j != i) {
                term = (term * (0 - x_values[j] + PRIME_NUMBER)) % PRIME_NUMBER;
                int inv = mod_inverse((x_values[i] - x_values[j] + PRIME_NUMBER) % PRIME_NUMBER, PRIME_NUMBER);
                term = (term * inv) % PRIME_NUMBER;
            }
        }
        secret = (secret + term) % PRIME_NUMBER;
    }
    return secret;
}

int mod_inverse(int value, int mod) {
    int original_mod = mod;
    int prev_coeff = 0, curr_coeff = 1;
    int quotient, temp;

    if (mod == 1) return 0;

    while (value > 1) {
        quotient = value / mod;

        temp = mod;
        mod = value % mod;
        value = temp;

        temp = prev_coeff;
        prev_coeff = curr_coeff - quotient * prev_coeff;
        curr_coeff = temp;
    }

    if (curr_coeff < 0) curr_coeff += original_mod;
    return curr_coeff;
}
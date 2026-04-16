#include <stdio.h>
#include <stdlib.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "stego.h"
#include <stdint.h>
#include <string.h>


int steganography_decode(const char *image_path, const char *decoded_image_path) {

    int width_cover_image, height_cover_image, comp_cover_image;

    unsigned char *cover_image = stbi_load(image_path, &width_cover_image, &height_cover_image, &comp_cover_image, 3);

    if (!cover_image) {
        printf("Failed to load images.\n");
        return 1;
    }

    if (comp_cover_image != 3) {
        printf("Only RGB images are supported.\n");
        return 1;
    }

    if (decode_n_bytes_metadata(cover_image + 16, 16) != MAGIC_NUMBER1 || decode_n_bytes_metadata(cover_image + 48, 16) != MAGIC_NUMBER2) {
        printf("There is no secret data in this image.");
        stbi_image_free(cover_image);
        return 1;
    }

    // Read secret image dimensions and lsb_bit_count from the header
    int width_secret_image = decode_n_bytes_metadata(cover_image, 16);
    int height_secret_image = decode_n_bytes_metadata(cover_image + 32, 16);
    int lsb_bit_count = decode_n_bytes_metadata(cover_image + 64, 8);

    size_t secret_image_total_byte = width_secret_image * height_secret_image * 3;

    unsigned char *secret_image = malloc(secret_image_total_byte);

    if (!secret_image) {
        printf("Failed to load images.\n");
        stbi_image_free(cover_image);
        return 1;
    }

    size_t cover_image_index = METADATA_SIZE;  // Embedded data starts after the header
    size_t secret_image_index = 0;
    int padding = (lsb_bit_count - (secret_image_total_byte % lsb_bit_count)) % lsb_bit_count;

    while (secret_image_index + lsb_bit_count <= secret_image_total_byte + padding) {  // Process lsb_bit_count bytes at a time, including padding to
                                                                                        // complete the last chunk
        
        uint8_t de_embeded_bytes[lsb_bit_count];  // Read 8 chunks of m bits and reconstruct m bytes from them (m = lsb_bit_count)
        
        memset(de_embeded_bytes, 0, lsb_bit_count);

        uint8_t de_embeded_bits[8] = {0};   // Prepare 8 chunks of m bits each (m = lsb_bit_count)

        decode_8_bytes_data_to_arr(de_embeded_bits, cover_image, cover_image_index, lsb_bit_count);

        de_embed_bits_to_bytes_arr(de_embeded_bits, de_embeded_bytes, lsb_bit_count);

        set_secret_image(lsb_bit_count, secret_image, secret_image_index, de_embeded_bytes, secret_image_total_byte);

        cover_image_index += 8;
        secret_image_index += lsb_bit_count;
    }

    stbi_write_png(decoded_image_path, width_secret_image, height_secret_image, 3, secret_image, 0);


    stbi_image_free(cover_image);
    free(secret_image);
    return 0;
}


int decode_n_bytes_metadata(unsigned char *cover_image, int how_many_bytes) {

    int secret_value = 0;

    for (int i = 0; i < how_many_bytes; i++) {
        cover_image[i] = cover_image[i] & 0b1;  // Mask out all bits except the LSB, which holds the embedded data
                                                // Safe to modify cover image in-place since we free it without writing
        secret_value = secret_value | cover_image[i];
        if (i != how_many_bytes - 1) secret_value <<= 1;
    }

    return secret_value;
}

void decode_8_bytes_data_to_arr(uint8_t bits_arr[], unsigned char *cover_image, size_t cover_image_index, int lsb_bit_count) {
    // Read 8 pixels from cover image and extract the m LSBs from each

    uint8_t mask = (1 << lsb_bit_count) - 1; // m-bit mask, e.g. m=3 → 0b00000111 = 7 (m = lsb_bit_count)

    for (int i = 0; i < 8; i++) bits_arr[i] = cover_image[cover_image_index + i] & mask;
}

void de_embed_bits_to_bytes_arr(uint8_t bits_arr[],uint8_t bytes_arr[], int lsb_bit_count) {
    // Inverse of the encode bit-transposition: reconstruct original bytes from m-bit chunks

    // Transpose the bit matrix: distribute bits from 8 m-bit chunks back into m bytes (m = lsb_bit_count)
    for (int bit_pos = 0; bit_pos < lsb_bit_count; bit_pos++) {
        for (int bit_i = 0; bit_i < 8; bit_i++) {
            // Extract bit at position bit_pos from bits[bit_i]
            uint8_t bit = (bits_arr[bit_i] >> (lsb_bit_count - bit_pos -1)) & 1;   //  e.g. 101 -> 001  then  001 AND 1,        101 -> 010 then 010 AND 1
            bytes_arr[bit_pos] = (bytes_arr[bit_pos] << 1) | bit;
        }
    }
}

void set_secret_image(int lsb_bit_count, unsigned char *secret_image, size_t secret_image_index, uint8_t de_embeded_bytes[], size_t secret_image_total_byte) {
    // Write reconstructed bytes into secret image, skip padding bytes
    for (int i = 0; i < lsb_bit_count; i++) {
        if (secret_image_index + i < secret_image_total_byte) {
            secret_image[secret_image_index + i] = de_embeded_bytes[i];
        }
    }
}
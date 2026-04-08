#include <stdio.h>
#include <stdlib.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "stego.h"
#include <string.h>
#include <stdint.h>

int steganografi_encode (const char *cover_image_path, const char *secret_image_path, const char *encoded_photo_name, int lsb_bit_count) {

    int width_cover_image, height_cover_image, comp_cover_image;
    int width_secret_image, height_secret_image, comp_secret_image;

    unsigned char* cover_image = stbi_load(cover_image_path, &width_cover_image, &height_cover_image, &comp_cover_image, 3);
    unsigned char* secret_image = stbi_load(secret_image_path, &width_secret_image, &height_secret_image, &comp_secret_image, 3);

    if (!cover_image || !secret_image) {
        printf("Failed to load images.\n");
        return 1;
    }

    if (validate_and_load_images(comp_cover_image, comp_secret_image, lsb_bit_count, width_secret_image, height_secret_image, width_cover_image, height_cover_image)) {
        stbi_image_free(cover_image);
        stbi_image_free(secret_image);
        return 1;
    }

    size_t secret_image_total_byte = (size_t) (width_secret_image * height_secret_image * comp_secret_image);

    // Pad the secret image so its size is a multiple of lsb_bit_count
    int padding = (lsb_bit_count - (secret_image_total_byte % lsb_bit_count)) % lsb_bit_count;

    size_t new_secret_image_total_byte = secret_image_total_byte + padding;

    unsigned char* new_secret_image = malloc(new_secret_image_total_byte);

    if (!new_secret_image) {
        stbi_image_free(cover_image);
        stbi_image_free(secret_image);
        return 1;
    }

    memcpy(new_secret_image, secret_image, secret_image_total_byte);
    memset(new_secret_image + secret_image_total_byte, 0, padding); 

    prepare_payload(width_secret_image, height_secret_image, lsb_bit_count, cover_image);

    size_t cover_image_index = METADATA_SIZE;
    size_t secret_image_index = 0;
    
    uint8_t mask = (1 << lsb_bit_count) - 1; // m-bit mask, e.g. m=3 → 0b00000111 = 7
    
    while (secret_image_index + lsb_bit_count <= new_secret_image_total_byte
           && cover_image_index + 8 <= width_cover_image * height_cover_image * 3) {
        // Prepare 8 chunks of m bits each
        uint8_t bits[8] = {0};
    
        embed_bits_to_cover_image(lsb_bit_count, new_secret_image, secret_image_index, cover_image_index, mask, bits, cover_image);
    
        secret_image_index += lsb_bit_count;
        cover_image_index += 8;
    }
    

    if (!stbi_write_png(encoded_photo_name, width_cover_image, height_cover_image, 3, cover_image, 0)) {
        printf("Failed to write image.\n");
        stbi_image_free(cover_image);
        stbi_image_free(secret_image);
        free(new_secret_image);
        return 1;
    }

    stbi_image_free(cover_image);
    stbi_image_free(secret_image);
    free(new_secret_image);

    return 0;
}

int validate_and_load_images (int comp_cover_image, int comp_secret_image, int lsb_bit_count, 
    int width_secret_image, int height_secret_image, int width_cover_image, int height_cover_image) {

        if (comp_cover_image != 3 || comp_secret_image != 3) {
            printf("Only RGB images are supported.\n");
            return 1;
        }

        if (width_secret_image > 0xFFFF || height_secret_image > 0xFFFF) {
            printf("Secret image is too big to embed\n");
            return 1;
        }

        if (width_secret_image * height_secret_image * (8 / lsb_bit_count + 1) + METADATA_SIZE > width_cover_image * height_cover_image) {
            printf("Insufficient capacity. Cover image is too small to hide the secret data.\n");     
            return 1;
        }
        
        return 0;
    }
void prepare_payload (int secret_width, int secret_height, int lsb_bit_count, unsigned char* cover_image) {

    embed_16_bits_to_ls1b(cover_image, secret_width);
    embed_16_bits_to_ls1b(cover_image + 16, MAGIC_NUMBER1);      // Magic numbers used during decode to verify that the image contains hidden data
    embed_16_bits_to_ls1b(cover_image + 32, secret_height);
    embed_16_bits_to_ls1b(cover_image + 48, MAGIC_NUMBER2);      // Magic numbers used during decode to verify that the image contains hidden data

    // Embed lsb_bit_count into the cover image
    for (int i = 71; i > 63; i--) {
        cover_image[i] = (cover_image[i] & 0b11111110) | (lsb_bit_count & 1);
        lsb_bit_count = lsb_bit_count >> 1;
    }

}

void embed_16_bits_to_ls1b (unsigned char* cover_image, int source_data) {
    for (int i = 15; i >= 0; i--) {
        cover_image[i] = (cover_image[i] & 0b11111110) | (source_data & 1);
        source_data = source_data >> 1;
    }

}

void embed_bits_to_cover_image (int lsb_bit_count, unsigned char* new_secret_image, size_t secret_image_index, size_t cover_image_index,
    uint8_t mask, uint8_t bits_arr[], unsigned char* cover_image){
        // Take m bytes from the secret image and split them bit by bit into 8 chunks
        for (int i = 0; i < lsb_bit_count; i++) {
            unsigned char byte = new_secret_image[secret_image_index + i];
            for (int b = 0; b < 8; b++) {
                int bit_val = (byte >> (7 - b)) & 1; // Start from the most significant bit
                bits_arr[b] = (bits_arr[b] << 1) | bit_val;  // Shift left and append each bit according to position i
            }
        }
    
        // Now bits[] contains 8 values of m bits each
        // Embed them into the LSBs of the cover image
        for (int i = 0; i < 8; i++) {
            cover_image[cover_image_index + i] &= ~mask;       // Clear the m LSBs first
            cover_image[cover_image_index + i] |= (bits_arr[i] & mask); // Then embed
        }

    }
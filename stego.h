#ifndef HEADER_STEGO_H
#define HEADER_STEGO_H

#define MAGIC_NUMBER1 't'
#define MAGIC_NUMBER2 'f'

#define METADATA_SIZE 72

int steganography_encode(const char *bigger_photo_path, const char *smaller_photo_path, const char *encoded_photo_name, int m);

int validate_and_load_images(int comp_cover_image, int comp_secret_image, int lsb_bit_count,
                            int width_secret_image, int height_secret_image, int width_cover_image, int height_cover_image);

void embed_16_bits_to_ls1b(unsigned char *cover_image, int source_data);

void prepare_payload(int width_secret_image, int height_secret_image, int lsb_bit_count, unsigned char *cover_image);

void embed_bits_to_cover_image(int lsb_bit_count, unsigned char *new_secret_image, size_t secret_image_index, size_t cover_image_index,
                                uint8_t mask, uint8_t bits_arr[], unsigned char *cover_image);


int decode_n_bytes_metadata(unsigned char *cover_image, int how_many_bytes);

void decode_8_bytes_data_to_arr(uint8_t bits_arr[], unsigned char *cover_image, size_t cover_image_index, int lsb_bit_count);

void de_embed_bits_to_bytes_arr(uint8_t bits_arr[],uint8_t bytes_arr[], int lsb_bit_count);

void set_secret_image(int lsb_bit_count, unsigned char *secret_image, size_t secret_image_index, uint8_t de_embeded_bytes[], size_t secret_image_total_byte);

int steganography_decode(const char *image_path, const char *decoded_image_path);



#endif
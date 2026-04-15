#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stego.h"
#include "sss.h"
#include "menu.h"
#include "utils.h"

int N;
int K;

void menu() {


    
    int choice;

    while(1){
        CLEAR_SCREEN
        printf("      ░▒▓███████▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░       ░▒▓██████▓▒░░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░▒▓████████▓▒░    \n");
        printf("      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░        \n");
        printf("      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░        \n");
        printf("      ░▒▓███████▓▒░░▒▓█▓▒░░▒▓██████▓▒░       ░▒▓█▓▒░      ░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓███████▓▒░  ░▒▓█▓▒░        \n");
        printf("      ░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░  ░▒▓█▓▒░   ░▒▓█▓▒░        ░▒▓█▓▒░        \n");
        printf("      ░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░  ░▒▓█▓▒░   ░▒▓█▓▒░        ░▒▓█▓▒░        \n");
        printf("      ░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░       ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░  ░▒▓█▓▒░   ░▒▓█▓▒░        ░▒▓█▓▒░        \n\n");
        printf("                                              [ P i x C r y p t  v1.0 ]                                           \n");
        printf("Author: 102Furkant\n\n\n");
        printf("1. Encode an image using Steganography and Shamir's Secret Sharing combined\n");
        printf("2. Encode an image using only Steganography\n");
        printf("3. Encode an image using only Shamir's Secret Sharing\n");
        printf("4. Decode from Steganography and Shamir's Secret Sharing combined\n");
        printf("5. Decode from Steganography Only\n");
        printf("6. Decode from Shamir's Secret Sharing Only\n");
        printf("7. Calculate PSNR value\n");
        printf("8. Exit\n");
        printf("Please enter an option: ");

        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        CLEAR_SCREEN

        switch (choice) {
        case 1:
            encode_both();
            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

            break;
        case 6:

            break;
        case 7: {
            char first_image_path[FOLDER_PATH_SIZE];
            printf("Enter first image path: ");
            set_path_or_name(first_image_path);

            char second_image_path[FOLDER_PATH_SIZE];
            printf("Enter second image path: ");
            set_path_or_name(second_image_path);

            double psnr_value = calculate_psnr(first_image_path, second_image_path);
            if( psnr_value == INFINITY) printf("PSNR Value: INFINITY!\n");
            else if (psnr_value >= 0) printf("PSNR value: %.2lf dB\n", psnr_value);
            wait_until_input();
            break;
        }
        case 8:
            printf("See you!\n");
            return;
            break;
        default:
            printf("Invalid option! Please try again.\n");
            break;
        }

    }

}

void encode_both() {
    char project_folder_name[FILE_NAME_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    printf("For Shamir's Secret Sharing\n");
    printf("--------------------------\n\n");

    char secret_image_path[FOLDER_PATH_SIZE];
    printf("Enter secret image path: ");
    set_path_or_name(secret_image_path);

    CLEAR_SCREEN

    printf("For Steganography\n");
    printf("--------------------------\n\n");

    char cover_image_path[FOLDER_PATH_SIZE] = {};
    printf("Enter cover image path: ");
    set_path_or_name(cover_image_path);

    int lsb_bit_count;

    printf("Enter lsb bit count 1-8 (recommended 1): ");
    if (!scanf("%d", &lsb_bit_count) || lsb_bit_count > 8 || lsb_bit_count < 1) {
        printf("Invalid value!\n");
        return;
    }

    CLEAR_SCREEN

    printf("Encoding ...\n");

    if(set_N_and_K()) return;

    if (sss_encode(secret_image_path, project_folder_name)) return;

    for (int i = 0; i < N; i++) {
        char encoded_stego_path[FILE_NAME_SIZE] = {};

        char share_path[FOLDER_PATH_SIZE] = {};

        generate_encoded_stego_path(encoded_stego_path, secret_image_path, project_folder_name, i);

        generate_share_path(share_path, project_folder_name, i);

        if(steganography_encode(cover_image_path, share_path, encoded_stego_path, lsb_bit_count)) {
            return;
        }
    }

    printf("\n==============================================\n");
    printf("SUCCESS: Encode completed successfully!\n");
    printf("Files saved in: %s\n", project_folder_name);
    printf("==============================================\n\n");

    wait_until_input();
}

void set_path_or_name(char *image) {
    scanf("%99s", image);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

}

int set_N_and_K() {
    printf("Enter total number of shares (2-20): ");
    if (!scanf("%d", &N) || N < 2 || N > 20) {
        printf("Invalid value!\n");
        wait_until_input();
        return 1;
    }

    printf("Enter threshold value (1-7): ");
    if (!scanf("%d", &K) || K < 1 || K > 7) {
        printf("Invalid value!\n");
        wait_until_input();
        return 1;
    }

    if (K > N) {
        printf("Threshold value can not be larger than total numver of shares!\n");
        wait_until_input();
        return 1;
    }

    return 0;
}

int set_project_folder_name(char *project_folder_name) {
    int choice;

    printf("1. Create a folder to save outputs\n");
    printf("2. Save the outputs to an existing folder\n");
    printf("Please enter an option: ");

    if ((scanf("%d", &choice) != 1) || (choice != 1 && choice != 2)) {
        printf("Invalid option!\n");
        wait_until_input();
        return 1;
    }

    else if (choice == 1) {
        printf("Enter folder name: ");
        set_path_or_name(project_folder_name);
        make_dir(project_folder_name)
    }

    else {
        printf("Enter the path: ");
        set_path_or_name(project_folder_name);
    }

    return 0;
}

void generate_encoded_stego_path(char output_path[], const char secret_image_path[], char destination_folder_path[], int index) {

    char secret_image_name[FILE_NAME_SIZE];
    int secret_image_name_len = 0;
    int secret_image_path_len = 0;
    for (int i = 0; i < FILE_NAME_SIZE; i++) {
        if (secret_image_path[i] == '\0') break;
        secret_image_name_len++;
        secret_image_path_len++;
        if (secret_image_path[i] == PATH_SEP) secret_image_name_len = 0;
    }

    for (int i = 0; i < secret_image_name_len; i++) {
        secret_image_name[i] = secret_image_path[secret_image_path_len - secret_image_name_len + i];
    }

    secret_image_name[secret_image_name_len] = '\0';

    sprintf(output_path, "%s%c%s_sss_stego_encoded%d.png", destination_folder_path, PATH_SEP, secret_image_name, index + 1);  
}

void wait_until_input() {
    printf("Press ENTER to return to the main menu...");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    getchar();
}
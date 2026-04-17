#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include "stego.h"
#include "sss.h"
#include "menu.h"
#include "utils.h"

int N;
int K;

void menu() {

    int choice;

    while (1) {
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
            encode_stego_only();
            break;
        case 3:
            encode_sss_only();
            break;
        case 4:
            decode_both();
            break;
        case 5:
            decode_stego_only();
            break;
        case 6:
            decode_sss_only();
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
    printf("Enter the path of the image to be encoded: ");
    set_path_or_name(secret_image_path);

    if(set_N_and_K()) return;

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
        wait_until_input();
        return;
    }

    CLEAR_SCREEN

    printf("Encoding ...\n");

    if (sss_encode(secret_image_path, project_folder_name)) return;

    for (int i = 0; i < N; i++) {
        char encoded_stego_path[FILE_NAME_SIZE] = {};

        char share_path[FOLDER_PATH_SIZE] = {};

        generate_encoded_stego_path(encoded_stego_path, secret_image_path, project_folder_name, i);

        generate_share_path(share_path, project_folder_name, i);

        if(steganography_encode(cover_image_path, share_path, encoded_stego_path, lsb_bit_count)) {
            wait_until_input();
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
    if (!scanf("%d", &N) || N < 2 || N > MAX_N) {
        printf("Invalid value!\n");
        wait_until_input();
        return 1;
    }

    printf("Enter threshold value (1-7): ");
    if (!scanf("%d", &K) || K < 1 || K > MAX_K) {
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

    char secret_image_name[FILE_NAME_SIZE] = {};
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
    
    // Later this algorithm might switch places with strrchr() function

    secret_image_name[secret_image_name_len] = '\0';
    if (index < 0) {
        sprintf(output_path, "%s%c%s_sss_stego_encoded.png", destination_folder_path, PATH_SEP, secret_image_name);
        return;
    }
    sprintf(output_path, "%s%c%s_sss_stego_encoded%d.png", destination_folder_path, PATH_SEP, secret_image_name, index + 1);  
}

void wait_until_input() {
    printf("Press ENTER to return to the main menu...");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    getchar();
}

void encode_stego_only() {
    char project_folder_name[FILE_NAME_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    char cover_image_path[FOLDER_PATH_SIZE] = {};
    printf("Enter cover image path: ");
    set_path_or_name(cover_image_path);

    char secret_image_path[FOLDER_PATH_SIZE];
    printf("Enter the path of the image to be encoded: ");
    set_path_or_name(secret_image_path);

    char encoded_stego_path[FILE_NAME_SIZE] = {};
    generate_encoded_stego_path(encoded_stego_path, secret_image_path, project_folder_name, -1);

    int lsb_bit_count;

    printf("Enter lsb bit count 1-8 (recommended 1): ");
    if (!scanf("%d", &lsb_bit_count) || lsb_bit_count > 8 || lsb_bit_count < 1) {
        printf("Invalid value!\n");
        wait_until_input();
        return;
    }

    CLEAR_SCREEN

    printf("Encoding ...\n");

    if(steganography_encode(cover_image_path, secret_image_path, encoded_stego_path, lsb_bit_count)) {
        wait_until_input();
        return;
    }

    printf("\n==============================================\n");
    printf("SUCCESS: Encode completed successfully!\n");
    printf("Files saved in: %s\n", project_folder_name);
    printf("==============================================\n\n");

    wait_until_input();
}

void encode_sss_only() {
    char project_folder_name[FILE_NAME_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    char secret_image_path[FOLDER_PATH_SIZE];
    printf("Enter the path of the image to be encoded: ");
    set_path_or_name(secret_image_path);

    if(set_N_and_K()) return;

    CLEAR_SCREEN

    printf("Encoding ...\n");

    if (sss_encode(secret_image_path, project_folder_name)) {
        wait_until_input();
        return;
    }

    printf("\n==============================================\n");
    printf("SUCCESS: Encode completed successfully!\n");
    printf("Files saved in: %s\n", project_folder_name);
    printf("==============================================\n\n");

    wait_until_input();
}

void decode_both() {
    char project_folder_name[FILE_NAME_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    int input_k;
    printf("What was the threshold value: ");
    if (!scanf("%d", &input_k) || input_k < 1 || input_k > 7) {
        printf("Invalid value!\n");
        wait_until_input();
        return;
    }

    char result_image_name[FILE_NAME_SIZE] = {};
    printf("Enter a name for result image: ");
    set_path_or_name(result_image_name);

    char result_image_path[FOLDER_PATH_SIZE] = {};

    sprintf(result_image_path, "%s%c%s", project_folder_name, PATH_SEP, result_image_name);

    char stego_decode_input_path[FOLDER_PATH_SIZE] = {};

    int success = 0;

    char **stego_decode_output_names = malloc(input_k * sizeof(char *));

    if (!stego_decode_output_names) {
        printf("Memory allocation failed!\n");
        wait_until_input();
        return;
    }

    for (int i = 0; i < input_k; i++) {
        stego_decode_output_names[i] = malloc(FOLDER_PATH_SIZE * sizeof(char));
        if (!stego_decode_output_names[i]) {
            printf("Memory allocation failed!\n");
            input_k = i;
            goto cleanup;
        }
    }

    printf("Decoding ...\n");

    for (int i = 0; i < input_k; i++) {
        printf("Please enter the %d. image path: ", i + 1);
        set_path_or_name(stego_decode_input_path);

        sprintf(stego_decode_output_names[i], "%s%ctemp_stego%d.png", project_folder_name, PATH_SEP, i + 1);

        if(steganography_decode(stego_decode_input_path, stego_decode_output_names[i])) goto cleanup;
    }

    if (sss_decode(stego_decode_output_names, result_image_path)) goto cleanup;

    success = 1;

    cleanup:
    if (success) {
        for (int i = 0; i < input_k; i++) {
            if (remove(stego_decode_output_names[i]) != 0)
                perror("Temporary files could not be deleted");  // Note for me: no need \n in perror
        }
        printf("Temporary files deleted.\n");
    }
    for (int i = 0; i < input_k; i++) free(stego_decode_output_names[i]);
    free(stego_decode_output_names);

    if (success) {
        printf("\n==============================================\n");
        printf("SUCCESS: Decode completed successfully!\n");
        printf("Files saved in: %s\n", project_folder_name);
        printf("==============================================\n\n");
    }

    wait_until_input();
}

void decode_stego_only() {
    char project_folder_name[FILE_NAME_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    char stego_decode_input_path[FOLDER_PATH_SIZE] = {};
    printf("Enter the path of the image to be decoded: ");
    set_path_or_name(stego_decode_input_path);

    char result_image_name[FILE_NAME_SIZE] = {};
    printf("Enter a name for result image: ");
    set_path_or_name(result_image_name);

    char result_image_path[FOLDER_PATH_SIZE] = {};

    sprintf(result_image_path, "%s%c%s", project_folder_name, PATH_SEP, result_image_name);

    printf("Decoding ...\n");

    if (steganography_decode(stego_decode_input_path, result_image_path)) {
        wait_until_input();
        return;
    }

    printf("\n==============================================\n");
    printf("SUCCESS: Decode completed successfully!\n");
    printf("Files saved in: %s\n", project_folder_name);
    printf("==============================================\n\n");

    wait_until_input();
}

void decode_sss_only() {
    char project_folder_name[FILE_NAME_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    int input_k;
    printf("What was the threshold value: ");
    if (!scanf("%d", &input_k) || input_k < 1 || input_k > 7) {
        printf("Invalid value!\n");
        wait_until_input();
        return;
    }

    char **sss_input_pathes = malloc(input_k * sizeof(char *));
    if (!sss_input_pathes) {
        printf("Memory allocation failed!\n");
        wait_until_input();
        return;
    }
    
    for (int i = 0; i < input_k; i++) {
        sss_input_pathes[i] = malloc(FOLDER_PATH_SIZE * sizeof(char));
        if (!sss_input_pathes[i]) {
            printf("Memory allocation failed!\n");
            for (int j = 0; j < i; j++) free(sss_input_pathes[j]);
            free(sss_input_pathes);
            wait_until_input();
            return;
        }
    }

    for (int i = 0; i < input_k; i++) {
        printf("Please enter the %d. image path: ", i + 1);
        set_path_or_name(sss_input_pathes[i]);
    }


    char result_image_name[FILE_NAME_SIZE] = {};
    printf("Enter a name for result image: ");
    set_path_or_name(result_image_name);
    
    char result_image_path[FOLDER_PATH_SIZE] = {};
    
    sprintf(result_image_path, "%s%c%s", project_folder_name, PATH_SEP, result_image_name);

    printf("Decoding ...\n");

    if (sss_decode(sss_input_pathes, result_image_path)) {  
        for (int i = 0; i < input_k; i++) free(sss_input_pathes[i]);
        free(sss_input_pathes);  
        wait_until_input();
        return;
    }

    printf("\n==============================================\n");
    printf("SUCCESS: Decode completed successfully!\n");
    printf("Files saved in: %s\n", project_folder_name);
    printf("==============================================\n\n");

    for (int i = 0; i < input_k; i++) free(sss_input_pathes[i]);
    free(sss_input_pathes);

    wait_until_input();
}
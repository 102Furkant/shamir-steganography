#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
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
            printf("Enter first image path (include .png, .jpg, etc.): ");
            set_folder_path(first_image_path);

            char second_image_path[FOLDER_PATH_SIZE];
            printf("Enter second image path (include .png, .jpg, etc.): ");
            set_folder_path(second_image_path);

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
    char project_folder_name[FOLDER_PATH_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    printf("For Shamir's Secret Sharing\n");
    printf("--------------------------\n\n");

    char secret_image_path[FOLDER_PATH_SIZE];
    printf("Enter the path of the image to be encoded (include .png, .jpg, etc.): ");
    set_folder_path(secret_image_path);

    if(set_N_and_K()) return;

    CLEAR_SCREEN

    printf("For Steganography\n");
    printf("--------------------------\n\n");

    char cover_image_path[FOLDER_PATH_SIZE] = {};
    printf("Enter cover image path (include .png, .jpg, etc.): ");
    set_folder_path(cover_image_path);

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
        char output_stego_path[FOLDER_PATH_SIZE] = {};

        char sss_share_path[FOLDER_PATH_SIZE] = {};

        generate_stego_output_path(output_stego_path, secret_image_path, project_folder_name, i);

        generate_share_path(sss_share_path, project_folder_name, i);

        if(steganography_encode(cover_image_path, sss_share_path, output_stego_path, lsb_bit_count)) {
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

void set_file_name(char *image) {
    scanf("%255s", image);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

}

void set_folder_path(char *folder_path) {
    scanf("%511s", folder_path);
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
        printf("Enter the folder path: ");
        set_folder_path(project_folder_name);
        make_dir(project_folder_name)
    }

    else {
        printf("Enter the folder path: ");
        set_folder_path(project_folder_name);
    }

    return 0;
}

void generate_stego_output_path(char output_path[], const char secret_image_path[], char destination_folder_path[], int index) {

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

    while ((c = getchar()) != '\n' && c != EOF);
}

void encode_stego_only() {
    char project_folder_name[FOLDER_PATH_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    char cover_image_path[FOLDER_PATH_SIZE] = {};
    printf("Enter cover image path (include .png, .jpg, etc.): ");
    set_folder_path(cover_image_path);

    char secret_image_path[FOLDER_PATH_SIZE];
    printf("Enter the path of the image to be encoded (include .png, .jpg, etc.): ");
    set_folder_path(secret_image_path);

    char output_stego_path[FOLDER_PATH_SIZE] = {};
    generate_stego_output_path(output_stego_path, secret_image_path, project_folder_name, -1);

    int lsb_bit_count;

    printf("Enter lsb bit count 1-8 (recommended 1): ");
    if (!scanf("%d", &lsb_bit_count) || lsb_bit_count > 8 || lsb_bit_count < 1) {
        printf("Invalid value!\n");
        wait_until_input();
        return;
    }

    CLEAR_SCREEN

    printf("Encoding ...\n");

    if(steganography_encode(cover_image_path, secret_image_path, output_stego_path, lsb_bit_count)) {
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
    char project_folder_name[FOLDER_PATH_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    char secret_image_path[FOLDER_PATH_SIZE];
    printf("Enter the path of the image to be encoded (include .png, .jpg, etc.): ");
    set_folder_path(secret_image_path);

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
    char project_folder_name[FOLDER_PATH_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    int input_k;
    printf("What was the threshold value: ");
    if (!scanf("%d", &input_k) || input_k < 1 || input_k > 7) {
        printf("Invalid value!\n");
        wait_until_input();
        return;
    }

    char result_image_name[FILE_NAME_SIZE] = {};
    printf("Enter a name for result image (exclude .png, .jpg, etc.): ");
    set_file_name(result_image_name);

    char result_image_path[FOLDER_PATH_SIZE] = {};

    sprintf(result_image_path, "%s%c%s.png", project_folder_name, PATH_SEP, result_image_name);

    char cover_image_path[FOLDER_PATH_SIZE] = {};

    int *share_indexes = malloc(input_k * sizeof(int));

    int success = 0;

    char **temp_share_paths = malloc(input_k * sizeof(char *));

    if (!temp_share_paths) {
        printf("Memory allocation failed!\n");
        wait_until_input();
        return;
    }

    for (int i = 0; i < input_k; i++) {
        temp_share_paths[i] = malloc(FOLDER_PATH_SIZE * sizeof(char));
        if (!temp_share_paths[i]) {
            printf("Memory allocation failed!\n");
            input_k = i;
            goto cleanup;
        }
    }

    for (int i = 0; i < input_k; i++) {
        printf("Please enter the %d. image path (include .png, .jpg, etc.): ", i + 1);
        set_folder_path(cover_image_path);

        printf("Which share path was this path: ");
        if(!scanf("%d", share_indexes + i) || share_indexes[i] < 1 || share_indexes[i] > MAX_K ) {
            printf("Invalid value!\n");
            goto cleanup;
        }

        if (i > 0) {
            for (int a = 0; a < i; a++) {
                if (share_indexes[a] == share_indexes[i]) {
                    printf("Share indexes must be unique!\n");
                    goto cleanup;
                }
            }
        }

        sprintf(temp_share_paths[i], "%s%ctemp_stego%d.png", project_folder_name, PATH_SEP, i + 1);

        if(steganography_decode(cover_image_path, temp_share_paths[i])) goto cleanup;
    }



    printf("Decoding ...\n");

    if (sss_decode(temp_share_paths, result_image_path, share_indexes)) goto cleanup;

    success = 1;

    cleanup:
    if (success) {
        for (int i = 0; i < input_k; i++) {
            if (remove(temp_share_paths[i]) != 0)
                perror("Temporary files could not be deleted");  // Note for me: no need \n in perror
        }
        printf("Temporary files deleted.\n");
    }
    for (int i = 0; i < input_k; i++) free(temp_share_paths[i]);
    free(temp_share_paths);

    if (success) {
        printf("\n==============================================\n");
        printf("SUCCESS: Decode completed successfully!\n");
        printf("Files saved in: %s\n", project_folder_name);
        printf("==============================================\n\n");
    }

    wait_until_input();
}

void decode_stego_only() {
    char project_folder_name[FOLDER_PATH_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    char stego_input_path[FOLDER_PATH_SIZE] = {};
    printf("Enter the path of the image to be decoded (include .png, .jpg, etc.): ");
    set_folder_path(stego_input_path);

    char result_image_name[FILE_NAME_SIZE] = {};
    printf("Enter a name for result image (exclude .png, .jpg, etc.): ");
    set_file_name(result_image_name);

    char result_image_path[FOLDER_PATH_SIZE] = {};

    sprintf(result_image_path, "%s%c%s.png", project_folder_name, PATH_SEP, result_image_name);

    printf("Decoding ...\n");

    if (steganography_decode(stego_input_path, result_image_path)) {
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
    char project_folder_name[FOLDER_PATH_SIZE] = {};

    if (set_project_folder_name(project_folder_name)) return;

    int input_k;
    printf("What was the threshold value: ");
    if (!scanf("%d", &input_k) || input_k < 1 || input_k > 7) {
        printf("Invalid value!\n");
        wait_until_input();
        return;
    }

    char **share_paths = malloc(input_k * sizeof(char *));
    if (!share_paths) {
        printf("Memory allocation failed!\n");
        wait_until_input();
        return;
    }
    
    for (int i = 0; i < input_k; i++) {
        share_paths[i] = malloc(FOLDER_PATH_SIZE * sizeof(char));
        if (!share_paths[i]) {
            printf("Memory allocation failed!\n");
            for (int j = 0; j < i; j++) free(share_paths[j]);
            free(share_paths);
            wait_until_input();
            return;
        }
    }

    int *share_indexes = malloc(input_k * sizeof(int));

    for (int i = 0; i < input_k; i++) {
        printf("Please enter the %d. share path (include .png .jpv etc.): ", i + 1);
        set_folder_path(share_paths[i]);
        printf("Which share path was this: ");
        if(!scanf("%d", share_indexes + i) || share_indexes[i] < 1 || share_indexes[i] > MAX_K ) {
            printf("Invalid value!\n"); 
            wait_until_input();
            return;
        }

        if (i > 0) {
            for (int a = 0; a < i; a++) {
                if (share_indexes[a] == share_indexes[i]) {
                    printf("Share indexes must be unique!\n");
                    wait_until_input();
                    return;
                }
            }
        }
    }


    char result_image_name[FILE_NAME_SIZE] = {};
    printf("Enter a name for result image (exclude .png, .jpg, etc.): ");
    set_file_name(result_image_name);
    
    char result_image_path[FOLDER_PATH_SIZE] = {};
    
    sprintf(result_image_path, "%s%c%s.png", project_folder_name, PATH_SEP, result_image_name);

    printf("Decoding ...\n");

    if (sss_decode(share_paths, result_image_path, share_indexes)) {  
        for (int i = 0; i < input_k; i++) free(share_paths[i]);
        free(share_paths);  
        wait_until_input();
        return;
    }

    printf("\n==============================================\n");
    printf("SUCCESS: Decode completed successfully!\n");
    printf("Files saved in: %s\n", project_folder_name);
    printf("==============================================\n\n");

    for (int i = 0; i < input_k; i++) free(share_paths[i]);
    free(share_paths);

    wait_until_input();
}


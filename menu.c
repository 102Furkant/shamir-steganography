#include <stdio.h>
#include <stdlib.h>
#include "stego.h"
#include "sss.h"
#include "menu.h"
#include "utils.h"

#ifdef _WIN32
    #include <direct.h>
    #define make_dir(name) _mkdir(name)
    #define CLEAR_SCREEN system("cls");
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define make_dir(name) mkdir(name, 0777)
    #define CLEAR_SCREEN system("clear");
#endif

int N;
int K;

void menu() {

    printf("      ░▒▓███████▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░       ░▒▓██████▓▒░░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░▒▓████████▓▒░    \n");
    printf("      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░        \n");
    printf("      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░        \n");
    printf("      ░▒▓███████▓▒░░▒▓█▓▒░░▒▓██████▓▒░       ░▒▓█▓▒░      ░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓███████▓▒░  ░▒▓█▓▒░        \n");
    printf("      ░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░  ░▒▓█▓▒░   ░▒▓█▓▒░        ░▒▓█▓▒░        \n");
    printf("      ░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░  ░▒▓█▓▒░   ░▒▓█▓▒░        ░▒▓█▓▒░        \n");
    printf("      ░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░       ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░  ░▒▓█▓▒░   ░▒▓█▓▒░        ░▒▓█▓▒░        \n\n");
    
    int choice;

    while(1){
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
            char first_image_path[100];
            printf("Enter first image path: ");
            scanf("%99s", first_image_path);
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            char second_image_path[100];
            printf("Enter second image path: ");
            scanf("%99s", second_image_path);
            while ((c = getchar()) != '\n' && c != EOF);

            double psnr_value = calculate_psnr(first_image_path, second_image_path);
            if( psnr_value > 0) printf("PSNR Value is: %lf", psnr_value);
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
    char secret_image_path[100];
    printf("Enter secret image path: ");
    get_path_or_name(secret_image_path);

    char cover_image_path[100];
    printf("Enter cover image path: ");
    get_path_or_name(cover_image_path);

    char result_image_name[100];
    printf("Enter result image name: ");
    get_path_or_name(result_image_name);

    int lsb_bit_count;
    printf("Enter lsb bit count 1-8 (recommended 1): ");
    if (!scanf("%d", &lsb_bit_count)) {
        printf("Invalid value!\n");
        return;
    }

    if(get_N_and_K) return;

    

    printf("Encoding ...");

    if(steganography_encode(cover_image_path, secret_image_path, result_image_name, lsb_bit_count)) {

    }
}

void get_path_or_name(char *image) {
    scanf("%99s", image);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

}

int get_N_and_K() {
    printf("Enter total number of shares (2-20): ");
    if (!scanf("%d", &N) || N < 2 || N > 20) {
        printf("Invalid value!\n");
        return 1;
    }

    printf("Enter threshold value (1-7): ");
    if (!scanf("%d", &K) || K < 1 || K > 7) {
        printf("Invalid value!\n");
        return 1;
    }

    if (K > N) {
        printf("Threshold value can not be larger than total numver of shares!");
        return 1;
    }

    return 0;
}
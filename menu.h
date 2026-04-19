#ifndef HEADERS_MENU_H
#define HEADERS_MENU_H

#define FILE_NAME_SIZE 256
#define FOLDER_PATH_SIZE 512
#define MAX_K 7
#define MAX_N 20

#ifdef _WIN32
    #include <direct.h>
    #define make_dir(name) _mkdir(name);
    #define CLEAR_SCREEN system("cls");
    #define PATH_SEP '\\'
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define make_dir(name) mkdir(name, 0777);
    #define CLEAR_SCREEN system("clear");
    #define PATH_SEP '/'
#endif

void menu();

void encode_both();

void set_file_name(char *image);

void set_folder_path(char *folder_path);

int set_N_and_K();

int set_project_folder_name(char *project_folder_name);

void generate_stego_output_path(char output_path[], const char secret_image_path[], char destination_folder_path[], int index);

void wait_until_input();

void encode_stego_only();

void encode_sss_only();

void decode_both();

void decode_stego_only();

void decode_sss_only();

#endif
#ifndef HEADERS_MENU_H
#define HEADERS_MENU_H

#define FILE_NAME_SIZE 64
#define FOLDER_PATH_SIZE 128

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

void set_path_or_name(char *image);

int set_N_and_K();

int set_project_folder_name(char *project_folder_name);

void generate_encoded_stego_path(char output_path[], const char secret_image_path[], char destination_folder_path[], int index);

void wait_until_input();



#endif
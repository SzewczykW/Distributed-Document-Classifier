#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>

#define MAX_FILENAME 512
#define MAX_FILES 1024
#define MAX_WORD_LEN 64
#define MAX_KEYWORDS 256

int list_txt_files(const char *dir_path, char files[][MAX_FILENAME], int max_files);
int read_file_content(const char *filename, char *buffer, size_t bufsize);
FILE *open_output_file(const char *output_path);

int read_dictionary(const char *dict_path, char *keywords[], int max_keywords);
void broadcast_dictionary(char *keywords[], int num_keywords);
void receive_dictionary(char *keywords[], int *num_keywords);

#endif


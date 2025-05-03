#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "hash_table.h"

void get_names(char *path, char ***file_name, int *cnt);
void read_dictionary(char *path, char **content, long *len);
void make_profile(char *path, hash_table hash_tbl, unsigned char **profile);
void build_2d_array(int file_cnt, int words_cnt, unsigned char ***vector);
void write_profiles(char *path, int file_cnt, int words_cnt, char **file_name,
                    unsigned char **vector);

#endif

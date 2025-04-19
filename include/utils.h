#ifndef UTILS_H
#define UTILS_H

char **list_documents(const char *dir_path, int *count);
void free_file_list(char **file_list, int count);
void print_help(const char *program_name);

#endif // UTILS_H


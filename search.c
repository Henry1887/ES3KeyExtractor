#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

#define BUFFER_SIZE 4096

const char *target_string = "ES3Defaults";
const char *target_filename = "SaveFile.es3";

// Function to check if a byte is printable
int is_printable(unsigned char byte) {
    return (byte >= 32 && byte <= 126);
}

// Function to search for the original string pattern in a given buffer
int search_in_buffer(const unsigned char *buffer, size_t bytes_read, size_t *string_start) {
    for (size_t i = 0; i <= bytes_read - strlen(target_string); i++) {
        if (memcmp(&buffer[i], target_string, strlen(target_string)) == 0) {
            size_t after_seq1 = i + strlen(target_string) + 9;
            if (after_seq1 + strlen(target_filename) < bytes_read &&
                memcmp(&buffer[after_seq1], target_filename, strlen(target_filename)) == 0) {
                *string_start = after_seq1 + strlen(target_filename) + 12;
                return 1;
            }
        }
    }
    return 0;
}

// Function to search for the alternative pattern in a given buffer
int search_in_buffer_alternative(const unsigned char *buffer, size_t bytes_read, size_t *string_start) {
    for (size_t i = 0; i <= bytes_read - strlen(target_string); i++) {
        if (memcmp(&buffer[i], target_string, strlen(target_string)) == 0) {
            size_t after_seq1 = i + strlen(target_string) + 3;
            if (after_seq1 + strlen(target_filename) < bytes_read &&
                memcmp(&buffer[after_seq1], target_filename, strlen(target_filename)) == 0) {
                *string_start = after_seq1 + strlen(target_filename) + 5;
                return 1;
            }
        }
    }
    return 0;
}

// Function to read a printable key from the buffer
void read_key_from_buffer(const unsigned char *buffer, size_t string_start, size_t bytes_read) {
    size_t print_end = string_start;
    while (print_end < bytes_read && buffer[print_end] != 0x00) {
        if (!is_printable(buffer[print_end])) break;
        print_end++;
    }
    fwrite(&buffer[string_start], 1, print_end - string_start, stdout);
    printf("\n");
}

// Function to search in a single file
int search_in_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Could not open file: %s\n", filename);
        return 0;
    }

    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        size_t string_start;
        
        // Try the original search first
        if (search_in_buffer(buffer, bytes_read, &string_start)) {
            printf("Found key: ");
            read_key_from_buffer(buffer, string_start, bytes_read);

            // Check if the extracted key is "password"
            if (strncmp((char *)&buffer[string_start], "password", 8) == 0) {
                printf("The game may have extra logic to alter the default key, or 'password' is the actual key used by the devs.\n");
            }
            fclose(file);
            return 1;
        }

        // If original search fails, try the alternative pattern
        if (search_in_buffer_alternative(buffer, bytes_read, &string_start)) {
            printf("Found key (alternative): ");
            read_key_from_buffer(buffer, string_start, bytes_read);

            // Check if the extracted key is "password"
            if (strncmp((char *)&buffer[string_start], "password", 8) == 0) {
                printf("The game may have extra logic to alter the default key, or 'password' is the actual key used by the devs.\n");
            }
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Function to search through a directory
void search_directory(const char *directory) {
    struct dirent *entry;
    DIR *dp = opendir(directory);
    if (!dp) {
        printf("Could not open directory: %s\n", directory);
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", directory, entry->d_name);
            struct stat path_stat;
            stat(filepath, &path_stat);
            if (S_ISREG(path_stat.st_mode)) {
                if (search_in_file(filepath)) return;
            }
        }
    }
    closedir(dp);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_or_folder>\n", argv[0]);
        return 1;
    }

    struct stat path_stat;
    stat(argv[1], &path_stat);

    if (S_ISDIR(path_stat.st_mode)) {
        search_directory(argv[1]);
    } else if (S_ISREG(path_stat.st_mode)) {
        if (!search_in_file(argv[1])) {
            printf("Key not found. It might be stored elsewhere or encrypted.\n");
        }
    } else {
        printf("Invalid path: %s\n", argv[1]);
    }

    return 0;
}

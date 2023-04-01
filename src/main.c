#include <stdio.h>
#include <stdlib.h>
#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int check_arguments(int argc, char **argv); // Checks if the user passed the correct arguments
void print_help(char *program_path); // Prints a help message
FILE **open_files(char *input_file_path, char *output_file_path); // Creates the file pointers from filepaths
void timer(int n); // counts n seconds
int copy_file(FILE *input, FILE *output); // copies the files
char *trim_file_path(char *path); // removes the long parts of a path and fives just the filename eg. C:\my_file.txt -> my_file
int reset_file(FILE *file); //

int main(int argc, char **argv) {
	FILE *input;
	FILE *output;
	int seconds;

	if (check_arguments(argc, argv) == 1) {
		print_help(argv[0]);
		return 2;
	}

	FILE **files = open_files(argv[1], argv[2]);
	if (files == NULL) {
		puts("An error occurred\n");
		return 1;
	}
	FILE *input = files[0];
	FILE *output = files[1];
	
	while (1) {
		timer(seconds);
	}

	return 0;
}

int check_arguments(int argc, char **argv) {
	int is_positive_integer(char *string) {
		if (string[0] == '-') {
			return 0;
		}
		for (int i = 0; i < strlen(argv[3]); i += 1) {
			if (!isdigit(argv[3][i])) {
				return 0;
			}
		}
		return 1;
	}

	if (argc != 4 || argv[3] == "0" || !is_positive_integer(argv[3])) {
		puts("Error: Time should be at least 1 second");
		return 1;
	}

	return 0;
}

void print_help(char *program_path) {
	puts("A program to backup files periodically in case of potential data loss\n");
	printf("%s <input file> <output file> <time in seconds>\n");
	puts("Example: backup.exe C:\\homework.txt C:\\Users\\Me\\Documents\\homework_backup.txt 180\n");
}


FILE **open_files(char *input_file_path, char *output_file_path) {
	FILE **files = NULL;
	FILE *input_file = fopen(input_file_path, "rb");
	FILE *output_file = fopen(output_file_path, "wb");

	if (input_file == NULL) {
		printf("failed to open %s\n", input_file_path);
		return files;
	}
	if (output_file == NULL) {
		printf("failed to create %s\n", output_file_path);
		return files;
	}

	files = malloc(2 * sizeof(FILE));
	if (files == NULL) {
		puts("memory error\n");
	}
	files[0] = input_file;
	files[1] = output_file;
	return files;
}

void timer(int n) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (; n > 0; n -= 1) {
        sleep(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
}

int copy_file(FILE *input, FILE *output) {
    int ch;
    while ((ch = fgetc(input)) != EOF) {
        fputc(ch, output);
    }
	fseek(input, 0L, SEEK_SET);
	fseek(output, 0L, SEEK_SET);

	return 0;
}

char *trim_file_path(char *path) {
    char* filename = strrchr(filepath, '/'); // Find last occurrence of '/'

    if (filename == NULL) {
        filename = strrchr(filepath, '\\'); // Find last occurrence of '\'
        if (filename == NULL) {
            filename = filepath;
        } else {
            filename++; // Move past '\'
        }
    } else {
        filename++; // Move past '/'
    }
    char* extension = strrchr(filename, '.'); // Find last occurrence of '.'
    if (extension != NULL) {
        *extension = '\0'; // Replace '.' with '\0' to remove extension
    }
    return filename;
}
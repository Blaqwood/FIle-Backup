#include <stdio.h>
#include <stdlib.h>
#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>
#ifdef WIN32                /*                              */
#include <io.h>             /*                              */
#define F_OK 0              /*  This section is to          */
#define access _access      /*  make sure that a certain    */
#else                       /*  function I used works       */
#include <unistd.h>         /*  on all platforms            */
#endif                      /*                              */

int check_arguments(int argc, char **argv); // Checks if the user passed the correct arguments
void print_help(char *program_path); // Prints a help message
int copy_file(char *input_path, char *output_path); // copies data from one file to another
void timer(int n); // counts n seconds
void log_backup(int is_error); // logs each interval that the file is save to "backup_log.txt"
void log_init();

const char *LOGFILE = "backup_log.txt";

int main(int argc, char **argv) {
	char *input_file_path;
	char *output_file_path;
	int seconds;
	int backups = 0;

	if (check_arguments(argc, argv) == 1) {
		print_help(argv[0]);
		return 2;
	}
	
	input_file_path = argv[1];
	output_file_path = argv[2];
	seconds = atoi(argv[3]);
	log_init(input_file_path, output_file_path);
	
	while (1) {
		// copying the file
		int error = copy_file(input_file_path, output_file_path);

		// count the user specified interval
		timer(seconds);

		if (error) {
			puts("Error: Failed to copy file\n");
		}

		puts("Successfully backed up file\n");
		printf("Backup no. %d\n", backups + 1);
		backups += 1;
		log_backup(error);
	}

	return 0;
}

int check_arguments(int argc, char **argv) {
	// check if all values are strings longer than 1
	for (int i = 0; i < strlen(argv[3]); i += 1) {
		if (strlen(argv[i]) == 0) {
			return 1;
		}
	}
	// check if all options were entered
	if (argc != 4) {
		puts("Error: Wrong arguments provided\n");
		return 1;
	}
	// check if the time is greater than 0
	if (atoi(argv[3]) <= 0) {
		puts("Error: Time should be at least 1 second");
		return 1;
	}

	return 0;
}

void print_help(char *program_path) {
	puts("\nHELP:");
	puts("A program to backup files periodically in case of potential data loss\n");
	printf("%s <input file> <output file> <time in seconds>\n");
	puts("Example: backup.exe C:\\homework.txt C:\\Users\\Me\\Documents\\homework_backup.txt 180\n");
}

void timer(int n) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
	
	// pause th program for each iteration for 1 second
    for (int i = n; i > 0; i -= 1) {
        sleep(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
}

int copy_file(char *input, char *output) {
	FILE *input_file = fopen(input, "rb");
	FILE *output_file = fopen(output, "wb");
	int ch;

	if (input_file == NULL || output_file == NULL) {
		return 1;
	}

	// read a character from input and put it in output to the end of the file
    while ((ch = fgetc(input_file)) != EOF) {
        if (fputc(ch, output_file) == EOF) {
			puts("Failed to write file\n");
		}
    }	

	fclose(input_file);
	fclose(output_file);

	return 0;
}


void log_init(const char *input, const char *output) {
	FILE *file = fopen(LOGFILE, "w");
	if (file == NULL) {
		return;
	}

	fprintf(file, "File Backup\n\n\tINPUT: %s\n\tOUTPUT: %s\n", input, output);
	fclose(file);
}

void log_backup(int is_error) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	FILE *file = fopen(LOGFILE, "a");

	if (file == NULL) {
		return;
	}

	// save the date and time to the log file
	fprintf(file, "now: %d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	if (is_error) {
		fputs(" (Error)", file);
	}

	fputc('\n', file);
	fclose(file);
}

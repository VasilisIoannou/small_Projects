#include <stdio.h>
#include <stdlib.h>

#include "log.h"

static FILE* log_file = NULL;

void create_log() {
    log_file = fopen("game_log.txt", "w");

    if (log_file == NULL) {
        fprintf(stderr, "Error: Could not create log file!\n");
        return;
    }

    fprintf(log_file, "--- Game Log Started ---\n");
    fflush(log_file); 
}

void write_log(const char* str) {
    if (log_file == NULL) return;
    fprintf(log_file, "%s\n", str);
    fflush(log_file);
}

void close_log() {
    if (log_file != NULL) {
        fprintf(log_file, "--- Game Log Ended ---\n");
        fclose(log_file);
        log_file = NULL;
    }
}

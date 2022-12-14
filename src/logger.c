#include "subterra/logger.h"

FILE* logfile;

void logger_init()
{
    /* open file, get time and translate it into our time zone, then print to the file */
    logfile = fopen("log.txt", "w");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(logfile, "subterra log %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(logfile);
}

void logger_log(const char* log)
{
    /* print what we need into the log */
    logfile = fopen("log.txt", "a+");
    fputs(log, logfile);
    fclose(logfile);
}
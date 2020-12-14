#ifndef _AUDITDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define NUM_SUBDIR 100
#define STR_LENGTH 257

/*
Runs a security audit of all files in the given directory and its subdirectories.
*/
void auditDir(char *dirPath, FILE *dataFP, FILE *copyFP, fpos_t *pos, int size);

#endif /* "auditdir.h" */

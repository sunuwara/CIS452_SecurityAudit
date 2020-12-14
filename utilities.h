#ifndef _UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

/*
Copy the contents in read file to replace contents in write file
*/
void copyFile(char *writeFilename, char *readFilename);

/*
Check if given file exists or not
Return 1 if exists and 0 if it does not
*/
int fileExists(char *filepath);

/*
Create permissions string looking at the files stat information.
Return completed permissions string.
*/
char *createPermissionsStr(struct stat statBuf);

#endif /* "utilities.h" */

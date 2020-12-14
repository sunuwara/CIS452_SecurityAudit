#include "utilities.h"

/*
Copy the contents in read file to replace contents in write file
*/
void copyFile(char *writeFile, char *readFile)
{
    FILE *writeFP, *readFP;
    char c;

    // Open files
    writeFP = fopen(writeFile, "w");
    readFP = fopen(readFile, "r");

    if (writeFP == NULL)
    {
        printf("Unable to open %s file\n", writeFile);
        exit(-1);
    }
    else if (readFP == NULL)
    {
        printf("Unable to open %s file\n", readFile);
        exit(-1);
    }

    // Copy contents from read file to write file
    c = fgetc(readFP);
    while (c != EOF)
    {
        fputc(c, writeFP);
        c = fgetc(readFP);
    }

    // Close files
    fclose(writeFP);
    fclose(readFP);
}

/*
Check if given file exists or not
Return 1 if exists and 0 if it does not
*/
int fileExists(char *filepath)
{
    if (access(filepath, F_OK) == 0)
        return 1;
    return 0;
}

/*
Create permissions string looking at the files stat information.
Return completed permissions string.
*/
char *createPermissionsStr(struct stat statBuf)
{
    static char str[11];

    str[0] = (S_ISDIR(statBuf.st_mode)) ? 'd' : (S_ISLNK(statBuf.st_mode)) ? 'l' : '-';
    str[1] = (statBuf.st_mode & S_IRUSR) ? 'r' : '-';
    str[2] = (statBuf.st_mode & S_IWUSR) ? 'w' : '-';
    str[3] = (statBuf.st_mode & S_IXUSR) ? 'x' : '-';
    str[4] = (statBuf.st_mode & S_IRGRP) ? 'r' : '-';
    str[5] = (statBuf.st_mode & S_IWGRP) ? 'w' : '-';
    str[6] = (statBuf.st_mode & S_IXGRP) ? 'x' : '-';
    str[7] = (statBuf.st_mode & S_IROTH) ? 'r' : '-';
    str[8] = (statBuf.st_mode & S_IWOTH) ? 'w' : '-';
    str[9] = (statBuf.st_mode & S_IXOTH) ? 'x' : '-';
    str[10] = '\0';

    return str;
}

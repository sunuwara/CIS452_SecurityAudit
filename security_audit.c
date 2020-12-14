/**************************************************************
Title: CIS 452 Project 2: File System Security Audit
Authors: Aron Sunuwar

Description: This is a program that retrieves information about
             all of the files in your home filesystem. The
             information is stored in a "database" and at each
             "login" the program will automatically perform a
             security audit of the filesystem.
***************************************************************/

#include <stdio.h>
#include "utilities.h"
#include "auditdir.h"

int main()
{
    FILE *dataFP, *copyFP;
    fpos_t position;
    char homeDirPath[] = "./test";
    char dataFilePath[STR_LENGTH];
    char copyFilePath[STR_LENGTH];
    int dataSize = 0;

    sprintf(dataFilePath, "%s/database.txt", homeDirPath);
    sprintf(copyFilePath, "%s/copy.txt", homeDirPath);

    // Open "database" files, copy file is used to update previous "database"
    dataFP = fopen(dataFilePath, "a+");
    copyFP = fopen(copyFilePath, "w");
    fgetpos(dataFP, &position);

    // Get the size of "database"
    fseek(dataFP, 0, SEEK_END);
    dataSize = ftell(dataFP);
    fseek(dataFP, 0, SEEK_SET);

    // Run the audit starting at the "home" directory
    auditDir(homeDirPath, dataFP, copyFP, &position, dataSize);

    // Close "database" files
    fclose(dataFP);
    fclose(copyFP);

    // Update previous "database" file then delete copy file
    copyFile(dataFilePath, copyFilePath);
    remove(copyFilePath);

    return 0;
}

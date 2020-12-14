#include "auditdir.h"
#include "utilities.h"

/*
Runs a security audit of all files in the given directory and its subdirectories.
*/
void auditDir(char *dirPath, FILE *dataFP, FILE *copyFP, fpos_t *pos, int size)
{
    DIR *dirPtr;                          // directory pointer
    struct dirent *ep;                    // entry pointer
    struct stat sb;                       // stat buffer
    char path[STR_LENGTH];                // hold the path of each entry in directory
    char subdirs[NUM_SUBDIR][STR_LENGTH]; // array to hold path of each subdirectories
    int count = 0;                        // keep track of subdirectories
    char *line = NULL;                    // holds each line from file
    size_t len = 0;                       // holds the length of each line
    char tempStr[STR_LENGTH];             // holds some string temporarily
    char filename[STR_LENGTH];            // holds the filename to check in "database" file

    // Open directory
    dirPtr = opendir(dirPath);
    if (stat(dirPath, &sb) < 0)
    {
        perror("directory stat");
        exit(-1);
    }
    if (!S_ISDIR(sb.st_mode))
    {
        printf("Usage: invalid directory\n");
        exit(-1);
    }

    // Read directory and audit all files
    while ((ep = readdir(dirPtr)))
    {
        sprintf(path, "%s/%s", dirPath, ep->d_name);

        // Skip irrelevant files
        if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
            continue;
        if (!strcmp(ep->d_name, "database.txt") || !strcmp(ep->d_name, "copy.txt"))
            continue;

        // If directory then add path to subdir array for future
        if (ep->d_type == DT_DIR)
        {
            strcpy(subdirs[count], path);
            ++count;
        }

        // If regular file then conduct audit
        else
        {
            // Get file stat
            if (stat(path, &sb) < 0)
            {
                perror("file stat");
                exit(-1);
            }

            // Check filename stored in "database" file
            fsetpos(dataFP, pos);
            getline(&line, &len, dataFP);
            line[strcspn(line, "\n")] = 0;
            sprintf(filename, "%s", line);

            if ((size != 0) && (strlen(filename) != 0))
            {
                // Check if filename still exists in file system
                while (fileExists(filename) == 0)
                {
                    printf("WARNING: %s not found, will be removed from database\n", filename);
                    exit(1);
                    for (int i = 0; i < 6; ++i)
                        getline(&line, &len, dataFP);

                    getline(&line, &len, dataFP);
                    line[strcspn(line, "\n")] = 0;
                    sprintf(filename, "%s", line);
                    fgetpos(dataFP, pos);
                }
            }

            printf("--------------------------------------------------\n");
            printf("Audit %s...\n", path);

            // If file exists in "database"
            if (!strcmp(filename, path))
            {
                printf("- File found in database, verifying info\n");
                fputs(filename, copyFP);
                fputs("\n", copyFP);

                // Verify info in "database"
                for (int i = 0; i < 5; ++i)
                {
                    getline(&line, &len, dataFP);
                    line[strcspn(line, "\n")] = 0;

                    switch (i)
                    {
                    case 0:
                        sprintf(tempStr, "Size: %ld", sb.st_size);
                        if (strcmp(line, tempStr) != 0)
                            printf("WARNING: Change in filesize detected\n");
                        break;
                    case 1:
                        sprintf(tempStr, "Access Rights: %s\n", createPermissionsStr(sb));
                        tempStr[strcspn(tempStr, "\n")] = 0;
                        if (strcmp(line, tempStr) != 0)
                            printf("WARNING: Change in access rights detected\n");
                        break;
                    case 2:
                        sprintf(tempStr, "Access: %s", ctime(&sb.st_atime));
                        tempStr[strcspn(tempStr, "\n")] = 0;
                        if (strcmp(line, tempStr) != 0)
                            printf("WARNING: Change in last access time detected\n");
                        break;
                    case 3:
                        sprintf(tempStr, "Modification: %s", ctime(&sb.st_mtime));
                        tempStr[strcspn(tempStr, "\n")] = 0;
                        if (strcmp(line, tempStr) != 0)
                            printf("WARNING: Change in last modification time detected\n");
                        break;
                    case 4:
                        sprintf(tempStr, "Status Change: %s", ctime(&sb.st_ctime));
                        tempStr[strcspn(tempStr, "\n")] = 0;
                        if (strcmp(line, tempStr) != 0)
                            printf("WARNING: Change in last status change detected\n");
                        break;
                    }
                    fputs(tempStr, copyFP);
                    fputs("\n", copyFP);
                }

                fputs("\n", copyFP);
                getline(&line, &len, dataFP);
                fgetpos(dataFP, pos);
            }

            // Otherwise add new file info to copy
            else
            {
                //found = 0;
                printf("- New file detected, adding file info to database\n");

                fputs(path, copyFP);
                fputs("\n", copyFP);

                for (int i = 0; i < 5; ++i)
                {
                    switch (i)
                    {
                    case 0:
                        sprintf(tempStr, "Size: %ld", sb.st_size);
                        break;
                    case 1:
                        sprintf(tempStr, "Access Rights: %s", createPermissionsStr(sb));
                        tempStr[strcspn(tempStr, "\n")] = 0;
                        break;
                    case 2:
                        sprintf(tempStr, "Access: %s", ctime(&sb.st_atime));
                        tempStr[strcspn(tempStr, "\n")] = 0;
                        break;
                    case 3:
                        sprintf(tempStr, "Modification: %s", ctime(&sb.st_mtime));
                        tempStr[strcspn(tempStr, "\n")] = 0;
                        break;
                    case 4:
                        sprintf(tempStr, "Status Change: %s", ctime(&sb.st_ctime));
                        tempStr[strcspn(tempStr, "\n")] = 0;
                        break;
                    }

                    fputs(tempStr, copyFP);
                    fputs("\n", copyFP);
                }

                fputs("\n", copyFP);
            }

            printf("Audit - DONE!\n");
        }
    }

    // Recursively audit subdirectories
    for (int i = 0; i < count; ++i)
        auditDir(subdirs[i], dataFP, copyFP, pos, size);

    // Clean up open pointers
    free(line);
    closedir(dirPtr);
}


#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"



int parseMode(int argc)
{
    if (argc == 1)
    {
        printf("You must enter the path through the command line when calling this program (optional: exact match pattern)");
        return -1;
    }
    else if (argc == 2)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

void mode1(char *path, int depth)
{
    DIR *dir;
    struct dirent *entry;
    struct stat fileInfo;

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir failed");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip . and .. to avoid infinite loop
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char *fullPath = malloc(strlen(path) + strlen(entry->d_name) + 2); // 2 is for the "/" and terminating NULL
        if (fullPath == NULL)
        {
            perror("malloc failed :(");
            return;
        }
        strcpy(fullPath, path);
        strcat(fullPath, "/");
        strcat(fullPath, entry->d_name);

        stat(fullPath, &fileInfo); // Get file info

        for (int i = 0; i < depth; i++)
        {
            printf("    ");
        }

        if (S_ISREG(fileInfo.st_mode))
        {
            printf("%s (%ld bytes)\n", entry->d_name, fileInfo.st_size);
        }
        else if (S_ISDIR(fileInfo.st_mode))
        {
            printf("%s/ (directory)\n", entry->d_name);
            mode1(fullPath, depth + 1);
        }
        free(fullPath);
    }

    closedir(dir);
}

int mode2(char *path, stack_t *s, int depth, char *pattern)
{
    DIR *dir;
    struct dirent *entry;
    struct stat fileInfo;
    int matchFound = 0;

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir failed");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip . and .. to avoid infinite loop
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char *fullPath = malloc(strlen(path) + strlen(entry->d_name) + 2); // 2 is for the "/" and terminating NULL
        if (fullPath == NULL)
        {
            perror("malloc failed :(");
            closedir(dir);
            return -1;
        }
        strcpy(fullPath, path);
        strcat(fullPath, "/");
        strcat(fullPath, entry->d_name);

        if (stat(fullPath, &fileInfo) == -1)
        {
            perror("stat failed");
            free(fullPath);
            return -1;
        } // Get file info


        if (S_ISREG(fileInfo.st_mode))
        {
            if (strcmp(pattern, entry->d_name) == 0)
            {
                char sizeStr[30];
                sprintf(sizeStr, " (%ld bytes)\n", fileInfo.st_size);
                char *temp = malloc(strlen(entry->d_name) + 25 + strlen(sizeStr) + 2);
                if (temp == NULL)
                {
                    perror("malloc failed :(");
                    free(fullPath);
                    return -1;
                }
                strcpy(temp, "");
                for (int i = 0; i < depth; i++)
                {
                    strcat(temp, "    ");
                }
                strcat(temp, entry->d_name);
                strcat(temp, sizeStr);

                push(s, temp);
                matchFound = 1;
            }
            else {
                return 0;
            }
        }
        else if (S_ISDIR(fileInfo.st_mode))
        {
            
            if (mode2(fullPath, s, depth + 1, pattern))
            {
                matchFound = 1;
                char *temp = malloc(strlen(entry->d_name) + (depth * 4) + 20);
                if (temp == NULL)
                {
                    perror("malloc failed :(");
                    return -1;
                }
            
                strcpy(temp, "");
                for (int i = 0; i < depth; i++)
                {
                    strcat(temp, "    ");
                }

                strcat(temp, entry->d_name);
                strcat(temp, "/ (directory)\n");

                push(s, temp);
            }
        }
        free(fullPath);
    }

    closedir(dir);
    return matchFound;
}

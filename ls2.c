#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"

char *pathname;

// TODO: function definitions here for ls2

void parseMode(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("You must enter the path through the command line when calling this program (optional: exact match pattern)");
    }
    else if (argc == 2)
    {
        pathname = (char *)malloc(strlen(argv[1]) + 1);
        strcpy(pathname, argv[1]);
        mode1(pathname, 0);
    }
    else
    {
        mode2();
    }
}

void mode1(const char *path, int depth)
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

        char *fullPath = malloc(strlen(path) + strlen(entry->d_name) + 2);
        if (fullPath == NULL)
        {
            perror("malloc failed :(");
            return;
        }
        strcpy(fullPath, path);
        strcat(fullPath, "/");
        strcat(fullPath, entry->d_name);

        if (stat(fullPath, &fileInfo) == -1)
        {
            perror("stat failed");
            continue;
        }
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

    closedir(dir); // Close directory
}

void mode2()
{
}

/**
 * This is just an example. Delete this before
 * submission.
 */
/* void example(int *x)
{
    *x = thisIsGlobal;
} */

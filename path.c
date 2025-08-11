#include "path.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// This is the full definition of the struct.
// It is only visible inside this file (path.c), hiding it from the user.
struct Path
{
    size_t size;
    char *raw_path;
};

Path *path_create(const char *raw)
{
    // If input was null, return pointer to NULL
    if (raw == NULL)
    {
        return NULL;
    }

    Path *new_path_created = malloc(sizeof(Path));

    // Check if malloc succeeded (always to be checked)
    if (new_path_created == NULL)
    {
        return NULL; // Can't create the path, malloc didn't allocate space
    }

    new_path_created->size = strlen(raw);
    new_path_created->raw_path = malloc(new_path_created->size + 1); // + 1 for the ending '\0'

    // Check if malloc succeeded
    if (new_path_created->raw_path == NULL)
    {
        free(new_path_created);
        return NULL;
    }

    // Copy the contents of raw inside the newly reserved path pointer
    strcpy(new_path_created->raw_path, raw);

    return new_path_created;
}

Path *path_create_from_cwd(void)
{
    Path *new_path_created = malloc(sizeof(Path));

    // Check if malloc succeeded (always to be checked)
    if (new_path_created == NULL)
    {
        return NULL; // Can't create the path, malloc didn't allocate space
    }

    // Pass NULL to ask getcwd() to allocate the buffer for us.
    char *cwd_string = getcwd(NULL, 0);

    if (cwd_string != NULL)
    {
        new_path_created->size = strlen(cwd_string);
        new_path_created->raw_path = malloc(new_path_created->size + 1); // + 1 for the ending '\0'

        // Check if malloc succeeded
        if (new_path_created->raw_path == NULL)
        {
            free(new_path_created);
            return NULL;
        }

        // Copy the contents of raw inside the newly reserved path pointer
        strcpy(new_path_created->raw_path, cwd_string);

        free(cwd_string); // We are responsible for freeing the buffer allocated by getcwd
    }
    else
    {
        perror("getcwd() error in path_create_from_cwd");
        free(new_path_created);
        free(cwd_string);
        return NULL;
    }

    return new_path_created;
}

void path_destroy(Path *path)
{
    // Freeing a NULL pointer should be a safe operation but it's "good practice"
    // to check beforehand
    if (path == NULL)
    {
        return;
    }

    free(path->raw_path);
    free(path);
}

const char *path_get_raw(const Path *path)
{
    if (path == NULL)
    {
        return NULL;
    }

    return path->raw_path;
}

const char *path_get_pretty(const Path *path)
{
    if (path == NULL)
    {
        return NULL;
    }

    return path->raw_path;
}

bool path_set(Path *path, const char *raw)
{
    if (path == NULL || raw == NULL)
    {
        return false;
    }

    size_t new_size = strlen(raw);
    char *new_path_raw = malloc(new_size + 1);

    if (new_path_raw == NULL)
    {
        free(new_path_raw);
        return false;
    }

    // Free the old raw path
    free(path->raw_path);

    // Copy values to the old path fields
    path->size = new_size;
    path->raw_path = new_path_raw;

    // Copy actual contents
    strcpy(path->raw_path, raw);

    return true;
}

void path_update_cwd(Path *path)
{
    // Pass NULL to ask getcwd() to allocate the buffer for us.
    char *cwd_string = getcwd(NULL, 0);

    if (cwd_string != NULL)
    {
        if (path_set(path, cwd_string))
        {
            printf("path is --%s--\n", cwd_string);
        }
        else
        {
            perror("path_set() failed in path_update_cwd");
        }

        free(cwd_string); // Free the buffer allocated by getcwd, otherwise it would leak memory.
    }
    else
    {
        perror("getcwd() error in path_update_cwd");
    }
}

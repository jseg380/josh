#ifndef MYSHELL_PATH_H
#define MYSHELL_PATH_H

#include <stdbool.h>
#include <sys/types.h>

// Hide the implementation details so the user of the library can't see them
// and can't modify them causing errors/undefined behavior.
// This is the forward declaration. It makes Path an "opaque type".
// Users of this header can only work with pointers to Path (Path *).
// When the definition is hidden, the compiler will produce an error if the
// user tries to access p->some_field because, from main.c's perspective, the
// Path struct has no known members. You **force the user to go through your
// approved functions (path_get_raw, path_set, etc.)**, where you can control
// everything and maintain a valid state.
typedef struct Path Path;

/**
 * @brief Allocates and initializes a new Path object on the heap.
 * @param raw The raw string to initialize the path with.
 * @return A pointer to the new Path object, or NULL if allocation fails.
 */
Path *path_create(const char *raw);

/**
 * @brief Allocates and initializes a new Path object with the current working directory.
 *        This is useful for creating a Path object that represents the current directory.
 * @return A pointer to the new Path object, or NULL if allocation fails.
 */
Path *path_create_from_cwd(void);

/**
 * @brief Frees all memory associated with a Path object.
 * @param path A pointer to the Path object to be destroyed.
 */
void path_destroy(Path *path);

// --- Getters ---
/**
 * @brief Gets the raw string representation of the path.
 * @param path A pointer to the Path object.
 * @return A const pointer to the internal raw path string.
 */
const char *path_get_raw(const Path *path);

/**
 * @brief Gets a prettified string representation of the path.
 * @param path A pointer to the Path object.
 * @return A const pointer to the prettified path string.
 */
const char *path_get_pretty(const Path *path);

// --- Setters ---
/**
 * @brief Updates the path with a new raw string.
 *        This may reallocate internal memory.
 * @param path A pointer to the Path object to modify.
 * @param raw The new raw string for the path.
 * @return true on success, false on memory allocation failure.
 */
bool path_set(Path *path, const char *raw);

/**
 * @brief Updates the path with the current working directory.
 *        This may reallocate internal memory.
 * @param path A pointer to the Path object to modify.
 */
void path_update_cwd(Path *path);

#endif // !MYSHELL_PATH_H

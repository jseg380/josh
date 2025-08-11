#ifndef MYSHELL_COMMAND_H
#define MYSHELL_COMMAND_H

#include <sys/types.h> // For uint

// --- Foundational Type Definitions ---

// The result code for any command, built-in or external
typedef int CommandResult;

/**
 * @brief Format for the functions that implement commands
 * 
 * @param argc          The number of arguments in the argv array.
 * @param argv          An array of string arguments for the command.
 * @param output_buffer A buffer provided by the caller where this function can
 *                      write its text output.
 * @param buffer_size   The total size in bytes of the output_buffer. Used to
 *                      prevent buffer overflows.
 * 
 * @return              Returns 0 on success, or a non-zero value on failure.
 */
typedef CommandResult CommandFunction(int argc, char *argv[], char *output_buffer, size_t buffer_size);

// The data structure returned by the parser and used to launch commands.
// Needed by main.c (to create it) and builtins.c (to use it).
typedef struct
{
    uint count;       // Number of arguments
    char **arguments; // Array of pointers to char (i.e. strings)
} ParsedInput;

#endif

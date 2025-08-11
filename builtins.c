#include "builtins.h"
#include "command.h"
#include "config.h"
#include "path.h"
#include <stdint.h> // For uint8_t
#include <stdio.h>  // For printf, fflush, stdout
#include <stdlib.h> // For exit
#include <string.h> // For strcmp
#include <unistd.h> // For chdir

// This struct is an IMPLEMENTATION DETAIL of how we look up built-ins.
// It is only used inside this file, so it is defined here and nowhere else.
// This is the correct choice because no other module needs to know about it.
typedef struct
{
    char *string;              // string that represents the command, e.g. "exit"
    CommandFunction *function; // function that the command should execute
    char *short_help;          // short string with basic help of the command
} BuiltinCommand;

// =================================================================
// Declarations: Private builtin functions
// =================================================================
// Forward-declare the actual command functions. 'static' makes them private
// to this file.

/**
 * @brief Get the index of a builtin command in the BUILTIN_COMMANDS array
 *
 * @param command Command to check
 * @return Index of the command in the BUILTIN_COMMANDS array, or -1 if not found
 */
int get_builtin_command_index(const char *command);

/**
 * @brief Exits the shell with a given exit status (default 0).
 *
 * This command terminates the shell process. If an argument is provided,
 * it is parsed as an integer and used as the exit status. If no argument
 * is given, the shell exits with status 0.
 *
 * @param argc          Number of arguments passed to the command.
 *                      Expected: 0 or 1.
 * @param argv          Array of argument strings. If argc >= 1, argv[0] should
 *                      be an integer string.
 * @param output_buffer A buffer provided by the caller where this function can
 *                      write its text output.
 * @param buffer_size   The total size in bytes of the output_buffer. Used to
 *                      prevent buffer overflows.
 *
 * @return Exit status to be passed to the OS.
 */
static CommandResult builtin_exit(int argc, char *argv[], char *output_buffer, size_t buffer_size);

/**
 * @brief Changes the working directory of the shell
 *
 * @param argc          Number of arguments passed to the command. Expected: 1.
 * @param argv          Array of argument strings. argv[0] should be a valid
 *                      Path variable.
 * @param output_buffer A buffer provided by the caller where this function can
 *                      write its text output.
 * @param buffer_size   The total size in bytes of the output_buffer. Used to
 *                      prevent buffer overflows.
 *
 * @return
 */
static CommandResult builtin_cd(int argc, char *argv[], char *output_buffer, size_t buffer_size);

/**
 * @brief
 *
 * @param argc          Number of arguments passed to the command. Expected: 1.
 * @param argv          Array of argument strings. argv[0] should be a valid
 *                      Path variable.
 * @param output_buffer A buffer provided by the caller where this function can
 *                      write its text output.
 * @param buffer_size   The total size in bytes of the output_buffer. Used to
 *                      prevent buffer overflows.
 *
 * @return
 */
static CommandResult builtin_help(int argc, char *argv[], char *output_buffer, size_t buffer_size);

// The constant array is the core data of this module. It is private.
// It is the lookup table for this module's logic. 'static' ensures it's not
// visible to the linker from other files.
static const BuiltinCommand BUILTIN_COMMANDS[] = {
    {"exit", &builtin_exit, "Exit the shell"},
    {"cd", &builtin_cd, "Change directory"},
    {"help", &builtin_help, "Show help about available commands"},
    {NULL, NULL, NULL} // Use a NULL sentinel for robust iteration.
};

// =================================================================
// Definitions: Public functions
// =================================================================

bool builtin_exists(const char *command_name)
{
    return (get_builtin_command_index(command_name) != -1);
}

CommandResult builtin_execute(const ParsedInput *parsed_command, char *output_buffer, size_t buffer_size)
{
    const char *command_name = parsed_command->arguments[0];
    int builtin_command_index = get_builtin_command_index(command_name);

    CommandFunction *function_to_call = BUILTIN_COMMANDS[builtin_command_index].function;

    int arguments_count = parsed_command->count - 1;
    char **command_arguments = parsed_command->arguments + 1;

    int exit_status = function_to_call(arguments_count, command_arguments, NULL, 0);
    return exit_status;
}

CommandResult builtin_execute_empty(void)
{
    // printf("");
    // print_prompt(0);
    fflush(stdout);
    return 0;
}

// =================================================================
// Definitions: Private builtin functions
// =================================================================

int get_builtin_command_index(const char *command_name)
{
    int index = -1;

    for (int i = 0; BUILTIN_COMMANDS[i].string != NULL && index == -1; i++)
    {
        if (!strcmp(command_name, BUILTIN_COMMANDS[i].string))
        {
            index = i;
        }
    }

    return index;
}

CommandResult builtin_exit(int argc, char *argv[], char *output_buffer, size_t buffer_size)
{
    uint8_t exit_code = 0;

    if (argc > 0)
    {
        exit_code = atoi(argv[0]);
    }

    printf("%s\n", EXIT_MESSAGE);
    exit(exit_code);
}

CommandResult builtin_cd(int argc, char *argv[], char *output_buffer, size_t buffer_size)
{
    char *path_to_change_to = NULL;

    // --- Step 1: Determine the target directory ---

    if (argc == 0)
    {
        // Case: User typed just "cd". We need to go to the HOME directory.
        path_to_change_to = getenv("HOME");
        if (path_to_change_to == NULL)
        {
            // In the rare case that the HOME variable isn't set.
            fprintf(stderr, "myshell: cd: HOME not set\n");
            return 1; // Return failure
        }
    }
    else if (argc == 1)
    {
        // Case: User typed "cd /some/path". The target is the first argument.
        path_to_change_to = argv[0];
    }
    else if (argc > 1)
    {
        // Case: User typed "cd path1 path2" or more. This is an error.
        fprintf(stderr, "myshell: cd: too many arguments\n");
        return 1; // Return failure
    }

    // --- Step 2: Execute the directory change ---

    // The chdir() system call does the actual work.
    // It returns 0 on success and -1 on error.
    if (chdir(path_to_change_to) != 0)
    {
        // If chdir fails, it sets the global 'errno' variable.
        // perror() is the perfect tool to print a descriptive error message.
        // It will print our string, followed by a system error like
        // "No such file or directory".
        perror("myshell: cd");
        return 1; // Return failure
    }

    // --- Step 3: Update shell state ---

    // Update the PWD variable
    Path *cwd = path_create_from_cwd();
    if (setenv("PWD", path_get_raw(cwd), 1) != 0)
    {
        perror("myshell: cd, updating PWD");
        return 1; // Return failure
    }
    // Destroy the path we created, memory leak if this is skipped
    path_destroy(cwd);

    // Success!
    return 0;
}

CommandResult builtin_help(int argc, char *argv[], char *output_buffer, size_t buffer_size)
{
    printf("Commands available:\n");
    BuiltinCommand *current_command_ptr = (BuiltinCommand *)BUILTIN_COMMANDS;

    const size_t number_builtin_commands = sizeof(BUILTIN_COMMANDS) / sizeof(BUILTIN_COMMANDS[0]);

    for (int i = 0; i < number_builtin_commands; i++)
    {
        printf(" - %s    %s\n", current_command_ptr->string, current_command_ptr->short_help);

        // Advance by one element
        current_command_ptr = current_command_ptr + 1;
    }

    return 0;
}

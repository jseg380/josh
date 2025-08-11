#include "builtins.h"  // For the commands built in the shell
#include "command.h"   // For ParsedInput
#include "config.h"    // For the shell configuration
#include "constants.h" // For constants
#include "process.h"
#include <dirent.h>    // For opendir, readdir
#include <signal.h>    // For signal(capture Ctrl+C)
#include <stdbool.h>   // For bool
#include <stddef.h>    // For size_t
#include <stdint.h>    // For uint8_t
#include <stdio.h>     // For printf, scanf, etc.
#include <stdlib.h>    // For malloc, free, exit, getenv
#include <string.h>    // For strcmp, strtok
#include <sys/stat.h>  // For stat
#include <sys/types.h> // For uint
#include <sys/wait.h>  // For waitpid
#include <unistd.h>    // For fork, exec, chdir

// ============================================================================
//
//
// Functions declarations
// ============================================================================

/**
 * @brief Read a line from stdin
 *
 * @return A string containing the line read from stdin.
 */
char *read_line(void);

/**
 * @brief Signal handler for SIGINT (Ctrl+C)
 *
 * @param sig Signal number (not used here, but required by the signal handler signature)
 */
void sigint_handler(int sig);

/**
 * @brief Process a command
 *
 * @param command Command to process
 * @return Exit status of the command
 */
int process_input(const char *command);

/**
 * @brief Read-Eval-Print Loop (REPL) for the shell
 *
 * @return Exit code passed to the exit command
 */
int read_eval_print_loop();

/**
 * @brief Print the shell prompt
 *
 * @param last_result Last result of the last command executed
 */
void print_prompt(const int last_result);

// ============================================================================
//
//
// Definitions
// ============================================================================

ParsedInput parse_arguments(const char *command)
{
    ParsedInput result;
    result.count = 0;
    result.arguments = NULL;

    char *copy = strdup(command); // make a mutable copy
    if (!copy)
    {
        return result;
    }

    uint capacity = 8;
    result.arguments = malloc(sizeof(char *) * capacity);
    int argument_count = 0;

    char *argument = strtok(copy, " ");
    while (argument != NULL)
    {
        if (argument_count >= capacity)
        {
            capacity *= 2;
            result.arguments = realloc(result.arguments, sizeof(char *) * capacity);
        }
        result.arguments[argument_count++] = strdup(argument); // copy the argument
        argument = strtok(NULL, " ");
    }

    // NULL-terminate the array
    result.arguments[argument_count] = NULL;
    result.count = argument_count;

    free(copy); // we don't need the original copy anymore
    return result;
}

int process_input(const char *input_line)
{
    // Process input (command + arguments)
    ParsedInput parsed_command = parse_arguments(input_line);

    // The command is the first word in the whole input
    const char *command_name = parsed_command.arguments[0];

    // for process-specific env variables see this test:
    /*
    ╭─ fish  ~/Downloads                                                                                                                                                             15:31:51 
    ╰─❯ mv script.sh 'ls=a'
    ╭─ fish  ~/Downloads                                                                                                                                                             15:31:51 
    ╰─❯ ls
    ls=a
    ╭─ fish  ~/Downloads                                                                                                                                                             15:31:52 
    ╰─❯ chmod 755 ls=a 
    ╭─ fish  ~/Downloads                                                                                                                                                             15:31:55 
    ╰─❯ ls
    ls=a
    ╭─ fish  ~/Downloads                                                                                                                                                             15:31:56 
    ╰─❯ ls=a 
    fish: Unsupported use of '='. In fish, please use 'set ls a'.

    ╭─ fish  ~/Downloads                                                                                                                                                             15:32:04 
    ╰─❯ 'ls=a'
    hello
    */

    // Pressing enter without writing anything will call this function twice
    // one time with an empty string and one time with NULL.
    if (command_name == NULL)
    {
        return builtin_execute_empty();
    }

    int exit_status = 0;

    if (builtin_exists(command_name))
    {
        exit_status = builtin_execute(&parsed_command, NULL, 0);
    }
    else
    {
        exit_status = launch_process(&parsed_command, NULL, 0);
    }

    return exit_status;
}

void print_prompt(const int last_result)
{
    printf("%s", DEFAULT_PROMPT_OPENING);
    if (last_result != 0)
    {
        printf("[%i]", last_result);
    }
    printf("%s", DEFAULT_PROMPT_CLOSING);
}

char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0; // getline will allocate a buffer for us
    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (feof(stdin))
        {
            exit(EXIT_SUCCESS); // We received an EOF (Ctrl+D)
        }
        else
        {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    // Remove \n from the string
    line[strcspn(line, "\n")] = 0;

    return line;
}

void sigint_handler(int sig)
{
    printf("\n");
    print_prompt(0);
    fflush(stdout);
}

int read_eval_print_loop()
{
    int last_result = 0;

    while (true)
    {
        print_prompt(last_result);

        const char *line = read_line();

        last_result = process_input(line);
    }

    return last_result;
}

int main(int argc, char *argv[])
{
    // Signal handlers (to capture Ctrl+C and the likes)
    signal(SIGINT, sigint_handler);

    printf("%s\n", INIT_MESSAGE);

    int exit_code = read_eval_print_loop();

    return exit_code;
}

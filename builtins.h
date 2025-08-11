#ifndef MYSHELL_BUILTINS_H

#include "command.h" // For CommandResult
#include <stdbool.h> // For bool

// This file is the public contract, anything in here can be used by whatever
// imports this module. Thus, the builtin_exit, builtin_cd, etc., functions are
// the "secret recipes." The main shell loop (main.c) has absolutely no reason
// to call builtin_cd directly. If it did, it would be bypassing the entire
// command lookup system

/**
 * @brief Check whether a command is a builtin or not
 *
 * @param command_name Command to check
 * @return true if the command name corresponds to a builtin, false otherwise
 */
bool builtin_exists(const char *command_name);

/**
 * @brief Execute a builtin command
 *
 * @param parsed_command ParsedInput of the command to execute
 * @param argc Number of arguments to the command
 * @param argv Array of arguments to the command
 * @return CommandResult Result of the command execution
 */
CommandResult builtin_execute(const ParsedInput *parsed_command, char *output_buffer, size_t buffer_size);

/**
 * @brief Execute the special 'empty' builtin command executed when no command
 *  is given and the user just presses Enter on an empty prompt (or filled with
 *  spaces).
 *
 * @return CommandResult Result of the command execution
 */
CommandResult builtin_execute_empty(void);

#endif // !MYSHELL_BUILTINS_H

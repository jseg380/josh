#ifndef MYSHELL_PROCESS_H
#define MYSHELL_PROCESS_H

#include "command.h" // For ParsedInput, CommandResult

/**
 * @brief Launches an external command in a new child process.
 * 
 * This function handles the fork-exec-wait sequence required to run
 * an external program located in the system's PATH.
 * 
 * @param parsed_input A pointer to the ParsedInput struct containing the
 *                     command and its arguments.
 * @return The exit status of the child process, or a failure code if the
 *         process could not be launched.
 */
CommandResult launch_process(const ParsedInput *parsed_input, char *output_buffer, size_t buffer_size);

#endif

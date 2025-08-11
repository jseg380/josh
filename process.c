#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

CommandResult launch_process(const ParsedInput *parsed_input, char *output_buffer, size_t buffer_size)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // --- This is the child process ---
        // The child process attempts to replace itself with the new program.
        if (execvp(parsed_input->arguments[0], parsed_input->arguments) == -1)
        {
            // execvp only returns if an error occurred.
            perror("myshell");
            exit(EXIT_FAILURE); // VERY IMPORTANT: Terminate the child process.
        }
    }
    else if (pid < 0)
    {
        // --- Forking error ---
        perror("myshell");
        return -1; // Indicate a failure to launch.
    }

    // --- This is the parent process ---
    // The parent waits for the child to terminate.

    // We pass 0 as the last argument because we are not interested in special
    // states like WUNTRACED for now. waitpid will block here until the child
    // has either exited or been killed.
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("myshell: waitpid failed");
        return -1; // Return a failure code.
    }

    // Now, we need to check HOW the child terminated.
    if (WIFEXITED(status))
    {
        // The child terminated normally (with exit() or return).
        // We can get its exit code.
        return WEXITSTATUS(status);
    }

    // If we get here, the child was terminated by a signal.
    return 1;
}

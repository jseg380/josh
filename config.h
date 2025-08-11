#ifndef MYSHELL_CONFIG_H
#define MYSHELL_CONFIG_H

// --- Compile-Time Constants (Self-Contained Values) ---
// Use `static const` for simple, literal-like values.
// This is type-safe, modular, and optimized away by the compiler.
// It avoids creating unnecessary dependencies.

/**
 * @brief Initial message displayed when the shell starts
 */
static const char *INIT_MESSAGE = "Welcome to MyShell!";

/**
 * @brief Exit message displayed when the shell exits
 */
static const char *EXIT_MESSAGE = "Exiting MyShell...";

#endif // !MYSHELL_CONFIG_H

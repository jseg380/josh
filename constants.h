#ifndef MYSHELL_CONSTANTS_H
#define MYSHELL_CONSTANTS_H

// --- Compile-Time Constants (Self-Contained Values) ---
// Use `static const` for simple, literal-like values.
// This is type-safe, modular, and optimized away by the compiler.
// It avoids creating unnecessary dependencies.
static const char *DEFAULT_PATH_RAW = "/";

// --- Internal Architectural Limits ---
// These are safety nets and design decisions, not user preferences.
// static const int MAX_ARGS_PER_COMMAND = 256;
// static const int MAX_INPUT_BUFFER_SIZE = 4096;

// --- Default Identifiers ---
static const char *DEFAULT_PROMPT_OPENING = "myshell";
static const char *DEFAULT_PROMPT_CLOSING = " -> ";
// static const char* CONFIG_FILENAME = ".myshellrc";

#endif // !MYSHELL_CONSTANTS_H

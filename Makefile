# --- Variables ---
# The compiler to use
CC = gcc

# Compiler flags:
# -Wall -Wextra  : Turn on all reasonable warnings.
# -g             : Include debug information.
# -std=gnu99     : Use the GNU99 standard.
CFLAGS = -Wall -Wextra -g -std=gnu99

# Linker flags:
# -lm            : Link against the math library (for functions like pow, etc.)
LDFLAGS = -lm

# The name of our final executable
TARGET = jsh

# Find all .c source files in the current directory
SRCS = $(wildcard *.c)

# Generate the corresponding .o object file names
OBJS = $(SRCS:.c=.o)


# --- Build Rules ---

# The default rule: build the entire project.
# This is what runs when you just type 'make'.
all: $(TARGET)

# Rule to link the final executable.
# It depends on all the object files being created first.
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile any .c file into a .o file.
# This rule is used implicitly for each source file.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# --- Cleanup Rule ---

# Rule to clean up all compiled files.
clean:
	rm -f $(TARGET) $(OBJS)

# Tells 'make' that 'all' and 'clean' are not actual files.
.PHONY: all clean

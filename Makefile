# Common compiler flags
CFLAGS = -Wall -Wextra -g

# Math library flag
MATH_LIB = -lm

# List of all your executable names
# Add all your program's executable names here
#EXECUTABLES = test my_other_program ...
EXECUTABLES = test

# Phony targets to prevent conflicts with files of the same name
.PHONY: all clean

all:$(EXECUTABLES)

test: test.c
	$(CC) $(CFLAGS) test.c -o test $(MATH_LIB)


# Rule to clean all executables
clean:
	@echo "Cleaning up executables..."
	rm -f $(EXECUTABLES)
	@echo "Done."
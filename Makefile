###############################################################################
#                                                                             #
#                                                 🭈🭆🭂███🭞🭜🭘  ███🭞🭜🭘██████     #
#    Makefile                                  🭈🭆🭂███🭞🭜🭘     🭞🭜🭘   ██████     #
#                                           🭈🭆🭂███🭞🭜🭘           🭈🭆🭂███🭞🭜🭘     #
#    By: vlaroque                        🭈🭆🭂███🭞🭜🭘           🭈🭆🭂███🭞🭜🭘        #
#        vlaroque@student.42.fr          ██████████████████  ██████   🭈🭆🭂     #
#                                        ██████████████████  ██████🭈🭆🭂███.fr  #
#                                                    ██████   🬚🬢🬞🬋🬏🬚🬢🬦🬞🬋🬃     #
#                                                    ██████   🬕🬀▐🬂▌🬕🬣▐🬞🬰🬃     #
#                                                                             #
###############################################################################


# Check HOSTTYPE environment variable
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# Library name with HOSTTYPE
NAME = libft_malloc_$(HOSTTYPE).so
SYMLINK = libft_malloc.so

# Compiler and flags
CC = gcc
CFLAGS = -Werror -Wall -Wextra -MMD -fPIC -O2 -g
LDFLAGS = -shared
LIBS = -lpthread

# Libft configuration
LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
LIBFT_INCLUDES = $(LIBFT_DIR)/includes

# Source files
SRC = malloc.c \
	zone.c\
	block.c\
	show_allocated_memory.c\
	internal.c\
	debug.c
SRC_DIR = src

# Headers
INCLUDES_DIR = includes
PRIVATE_INCLUDES = includes/private
PUBLIC_HEADER = $(INCLUDES_DIR)/libft_malloc.h
PRIVATE_HEADERS = $(wildcard $(PRIVATE_INCLUDES)/*.h)

# Objects
OBJECT_DIR = obj
OBJ = $(addprefix $(OBJECT_DIR)/, $(SRC:.c=.o))
DEPS = $(OBJ:.o=.d)

# Test
TEST_BIN = test_malloc

# Default target
all: $(NAME) $(SYMLINK)

# Build libft first
$(LIBFT_LIB):
	@if [ -d "$(LIBFT_DIR)" ]; then \
		echo "Building libft..."; \
		$(MAKE) -C $(LIBFT_DIR); \
	else \
		echo "Error: libft directory not found at $(LIBFT_DIR)"; \
		exit 1; \
	fi

# Build the shared library (.so)
$(NAME): $(OBJ) $(LIBFT_LIB)
	@echo "Linking $(NAME)..."
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIBFT_LIB) $(LIBS)

# Create symbolic link
$(SYMLINK): $(NAME)
	ln -sf $(NAME) $(SYMLINK)

# Compile object files
$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.c $(PUBLIC_HEADER) $(PRIVATE_HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -I$(LIBFT_INCLUDES) -I$(PRIVATE_INCLUDES) -c $< -o $@

# Clean objects and dependencies
clean:
	/bin/rm -rf $(OBJECT_DIR)
	@if [ -d "$(LIBFT_DIR)" ]; then \
		$(MAKE) -C $(LIBFT_DIR) clean; \
	fi


# Clean everything
fclean: clean
	/bin/rm -f $(NAME) $(SYMLINK)
	@if [ -d "$(LIBFT_DIR)" ]; then \
		$(MAKE) -C $(LIBFT_DIR) fclean; \
	fi;
	@if [ -f "$(TEST_BIN)" ]; then \
		/bin/rm -f $(TEST_BIN) ;\
	fi

# Rebuild everything
re:
	$(MAKE) fclean
	$(MAKE) all

# Show current HOSTTYPE
show-hosttype:
	@echo "HOSTTYPE: $(HOSTTYPE)"
	@echo "Library name: $(NAME)"
	@echo "Libft directory: $(LIBFT_DIR)"

# Test target (if you have a test file)
test: $(NAME)
	$(CC) test_malloc.c -I$(INCLUDES_DIR) -L. -lft_malloc -o $(TEST_BIN)
	LD_LIBRARY_PATH=. ./$(TEST_BIN)
# Include dependency files
-include $(DEPS)

.PHONY: all clean fclean re show-hosttype test

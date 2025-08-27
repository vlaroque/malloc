# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/13 13:25:30 by vlaroque          #+#    #+#              #
#    Updated: 2020/01/19 20:08:24 by vlaroque         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CPPFLAGS = -I includes -I includes/private -MMD

CC = gcc

CFLAGS = -Werror -Wall -Wextra

SRC =	ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memccpy.c \
		ft_memmove.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_strlen.c \
		ft_strdup.c \
		ft_strcpy.c \
		ft_strncpy.c \
		ft_strcat.c \
		ft_strncat.c \
		ft_strlcat.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_strstr.c \
		ft_strnstr.c \
		ft_strcmp.c \
		ft_strncmp.c \
		ft_atoi.c \
		ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_ispunct.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_memalloc.c \
		ft_memdel.c \
		ft_strnew.c \
		ft_strdel.c \
		ft_strclr.c \
		ft_striter.c \
		ft_striteri.c \
		ft_strmap.c \
		ft_strmapi.c \
		ft_strequ.c \
		ft_strnequ.c \
		ft_strsub.c \
		ft_strjoin.c \
		ft_strtrim.c \
		ft_strsplit.c \
		ft_itoa.c \
		ft_putchar.c \
		ft_putstr.c \
		ft_putendl.c \
		ft_putnbr.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_lstnew.c \
		ft_lstdelone.c \
		ft_lstdel.c \
		ft_lstadd.c \
		ft_lstiter.c \
		ft_lstmap.c \
		ft_nbrlen.c \
		ft_freechartab.c \
		ft_countwords.c \
		ft_power.c \
		ft_sqrt.c \
		op_bzero.c\
		get_next_line.c\
		ft_printf.c\
		ft_printf/ft_color.c\
		ft_printf/ft_conversions.c\
		ft_printf/ft_data_init.c\
		ft_printf/ft_generic_conv_fct.c\
		ft_printf/ft_generic_int_fct.c\
		ft_printf/ft_meganbr.c\
		ft_printf/ft_meganbr_maths.c\
		ft_printf/ft_meganbr_processing.c\
		ft_printf/ft_parsing.c\
		ft_printf/ft_writings.c\
		ft_printf/getdata_varg.c\
		ft_printf/put_bonus.c\
		ft_printf/put_cspc.c\
		ft_printf/put_di.c\
		ft_printf/put_f.c\
		ft_printf/put_f_rounding.c\
		ft_printf/put_f_writings.c\
		ft_printf/putnbr_base.c\
		ft_printf/put_oxXp.c\
		ft_printf/put_u.c\
		ft_printf/put_up_case.c


INCLUDES_DIR = includes
SRC_DIR = src

PUBLIC_HEADER = $(INCLUDES_DIR)/libft.h

PRIVATE_HEADERS = $(INCLUDES_DIR)/private

OBJECT_DIR = obj

OBJ = $(addprefix $(OBJECT_DIR)/, $(SRC:.c=.o))
DEPS = $(OBJ:.o=.d)

all : $(NAME)

$(NAME): $(OBJ) $(INCLUDES_DIR)/libft.h
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.c $(PUBLIC_HEADER) $(PRIVATE_HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean :
	/bin/rm -rf $(OBJECT_DIR)

fclean : clean
	/bin/rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

-include $(DEPS)

.PHONY: all, clean, fclean, re

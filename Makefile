# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 15:27:07 by lbellmas          #+#    #+#              #
#    Updated: 2025/03/17 11:49:26 by lbellmas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME       = pipex
BONUS		= pipex_bonus
AR         = ar
ARFLAGS    = -rcs
CC         = cc
CFLAGS     = -Wall -Wextra -Werror -g #-fsanitize=address
OFLAGS     = -MMD -MF $(@:.o=.d)

# Directorios
SRCDIR     = src
BSRCDIR		= bonus
UTILSDIR   = utils
DEPDIR     = deps
OBJDIR     = objs
BOBJDIR		= bonus_obj
OBJ_GETDIR = objs/get_next_line
PRINTFDIR  = printf
GETDIR     = get_next_line

# Archivos de cabecera
LIBGET     = $(GETDIR)/get_next_line_bonus.h
LIB        = header/ft_pipex.h

# Archivos fuente
SRC        = ft_parsing.c ft_process.c
BSRC		= 
UTILS      =  # Definir los archivos fuente de utils si los hay
GET        = get_next_line_bonus.c get_next_line_utils_bonus.c

# Archivos de objetos
OBJS       = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
BOBOJ		= $(addprefix $(BOBJDIR)/, $(BSRC:.c=.o))
OBJS_GET   = $(addprefix $(OBJ_GETDIR)/, $(GET:.c=.o))
DEPS       = $(addprefix $(DEPDIR)/, $(SRC:.c=.d) $(UTILS:.c=.d) $(GET:.c=.d))

# Biblioteca
LIBPRINTF  = $(PRINTFDIR)/libftprintf.a

# Archivo principal
MAIN       = ft_pipex.c
BMAIN		= ft_pipex_bonus.c
# Colores
RED        = \033[0;31m
GREEN      = \033[0;32m
YELLOW     = \033[0;33m
BLUE       = \033[0;34m
PURPLE     = \033[0;35m
CYAN       = \033[0;36m
RESET      = \033[m

# Objetivo principal
all: $(LIBPRINTF) $(NAME) $(LIB) $(LIBGET) Makefile

-include $(DEPS)

# Compilación de archivos fuente generales
$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c Makefile | $(OBJDIR) $(DEPDIR)
	@printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@
	@mv $(OBJDIR)/*.d $(DEPDIR)

$(BOBJ): $(BOBJDIR)/%.o : $(BSRCDIR)/%.c Makefile | $(BOBJDIR) $(DEPDIR)
	@printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@
	@mv $(OBJDIR)/*.d $(DEPDIR)

# Compilación de archivos de get_next_line
$(OBJS_GET): $(OBJ_GETDIR)/%.o : $(GETDIR)/%.c Makefile | $(OBJ_GETDIR) $(DEPDIR)
	@printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@
	@mv $(OBJ_GETDIR)/*.d $(DEPDIR)

# Creación de directorios
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJ_GETDIR):
	@mkdir -p $(OBJ_GETDIR)

$(DEPDIR):
	@mkdir -p $(DEPDIR)

# Compilación de la biblioteca printf
$(LIBPRINTF):
	@printf "%b" "$(BLUE)$(@F)$(RESET)\n"
	@$(MAKE) --silent -C $(PRINTFDIR)

# Enlace final del ejecutable
$(NAME): $(MAIN) $(OBJS) $(OBJS_GET) $(LIBPRINTF)
	@printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(MAIN) $(OBJS) $(LIBPRINTF) $(OBJS_GET) -o $(NAME)

bonus: $(BONUS) $(LIBPRINTF) $(LIB) $(LIBGET) Makefile

$(BONUS): $(BMAIN) $(BOBJ) $(OBJS) $(OBJS_GET) $(LIBPRINTF)
	@printf "%-42b%b" "$(PURPLE)$<:" "$(BLUE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $(BMAIN) $(BOBJ) $(OBJS) $(LIBPRINTF) $(OBJS_GET) -o $(BONUS)

# Limpiar objetos y dependencias
clean:
	@printf "%b" "$(BLUE)Cleaning objects...$(RESET)\n"
	@rm -rf $(OBJDIR)
	@rm -rf $(DEPDIR)
	@rm -rf $(OBJ_GETDIR)
	@$(MAKE) -C $(PRINTFDIR) clean --silent

# Limpiar todo
fclean: clean
	@printf "%b" "$(BLUE)Cleaning all files...$(RESET)\n"
	@rm -f $(NAME)
	@rm -rf $(BONUS)
	@$(MAKE) -C $(PRINTFDIR) fclean --silent

# Reconstrucción total
re: fclean all

.PHONY: all clean fclean re $(LIBPRINTF)


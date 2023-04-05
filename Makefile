# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llecoq <llecoq@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/26 09:22:35 by llecoq            #+#    #+#              #
#    Updated: 2022/09/11 16:18:31 by llecoq           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= 		ircserv

INCLUDES =		$(shell find srcs/ includes/ -type f -name "*.hpp")
SRCS_FILES	=	$(shell find srcs/ -type f -name "*.cpp")
DIR =			$(shell find srcs/ -type d)
INCLUDES_DIR = 	$(DIR) \
				includes/

OBJS_PATH =		.objs/
OBJS_FILES = 	$(SRCS_FILES:.cpp=.o)
OBJS = 			$(addprefix $(OBJS_PATH),$(OBJS_FILES))

CXXC	=		@clang++
CFLAGS	= 		-Wall -Wextra -Werror -std=c++98

RM		= 		@rm -rf

all		:	$(NAME)

$(NAME):	$(OBJS)
			$(CXXC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp $(INCLUDES) Makefile
			@mkdir -p $(addprefix $(OBJS_PATH), $(DIR))
			$(CXXC) $(CFLAGS) -c $< -o $@ $(addprefix -I , $(INCLUDES_DIR))

clean:
			$(RM) $(OBJS_PATH) 

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omahdiou <omahdiou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/28 04:48:03 by omahdiou          #+#    #+#              #
#    Updated: 2024/08/28 04:48:12 by omahdiou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CPPFILES = server.cpp main.cpp client.cpp Channel.cpp registration.cpp join.cpp invite.cpp \
		mode.cpp utils.cpp kick.cpp part.cpp quit.cpp privmsg.cpp topic.cpp \
		list.cpp 
CPPOFILES = $(CPPFILES:%.cpp=%.o)
CPPC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g 

HEADERS = Server.hpp Channel.hpp client.hpp numeric_replies.hpp utils.hpp

all: $(NAME)

$(NAME): $(CPPOFILES) $(HEADERS)
	@$(CPPC) $(CPPOFILES) -o $(NAME)

%.o: %.cpp
	@$(CPPC) $(CPPFLAGS) $< -c

clean:
	@rm -rf $(CPPOFILES)

fclean: clean
	@rm -rf $(NAME)

f: fclean

re: fclean all 

.PHONY: all clean fclean re

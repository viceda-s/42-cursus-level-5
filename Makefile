NAME	= ircserv
CC		= c++
FLAGS	= -Werror -Wextra -Wall -std=c++98
RM		= rm -rf

SRC		= src/main.cpp src/Server.cpp src/Client.cpp src/Channel.cpp
OBJ		= $(SRC:%.cpp=obj/%.o)

all: $(NAME)

obj/%.o : %.cpp
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@ -I inc

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $@

clean:
	$(RM) obj

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
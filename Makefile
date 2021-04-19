NAME = webserv

#-------------------------------------------------------------------------------
#	COMPILE OPTIONS
#-------------------------------------------------------------------------------
CC = clang++
CFLAG = -Wall -Wextra -Werror --std c++98 -g

#-------------------------------------------------------------------------------
#	FILE NAMES
#-------------------------------------------------------------------------------
LIST_SRCS = Webserver Utils Server Location FdSet main

	
	
DIR_SRCS = ./srcs/
DIR_OBJS = ./objs/

SRCS = $(addprefix $(DIR_SRCS), $(addsuffix .cpp, $(LIST_SRCS)))
OBJS = $(addprefix $(DIR_OBJS), $(addsuffix .o, $(LIST_SRCS)))

#-------------------------------------------------------------------------------
#	OTHER
#-------------------------------------------------------------------------------
RM = rm -rf

#-------------------------------------------------------------------------------
#	RULES
#-------------------------------------------------------------------------------
all : $(NAME)

$(DIR_OBJS):
	mkdir $(DIR_OBJS)

$(NAME) : $(DIR_OBJS) $(OBJS)
	$(CC) $(CFLAG) $(OBJS) -o $(NAME)

$(DIR_OBJS)%.o : $(DIR_SRCS)%.cpp
	$(CC) $(CFLAG) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(DIR_OBJS)
	$(RM) $(NAME)

re: fclean all
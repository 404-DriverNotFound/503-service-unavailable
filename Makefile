NAME = webserv

#-------------------------------------------------------------------------------
#	COMPILE OPTIONS
#-------------------------------------------------------------------------------
CC = clang++
CFLAG = -Wall -Wextra -Werror --std c++98 -g
ASAN = -fsanitize=address
#-------------------------------------------------------------------------------
#	FILE NAMES
#-------------------------------------------------------------------------------
LIST_SRCS = \
	Buffer\
	Cgi\
	Client\
	FdSet\
	Http\
	HttpReq\
	HttpRes\
	Location\
	main\
	Server\
	Socket\
	Time\
	Utils\
	Webserver

	
	
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

asan : $(DIR_OBJS) $(OBJS)
	$(CC) $(ASAN) $(CFLAG) $(OBJS) -o $(NAME)

$(DIR_OBJS)%.o : $(DIR_SRCS)%.cpp
	$(CC) $(CFLAG) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(DIR_OBJS)
	$(RM) $(NAME)

re: fclean all

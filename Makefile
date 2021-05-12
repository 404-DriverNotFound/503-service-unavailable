NAME = webserv

#-------------------------------------------------------------------------------
#	COMPILE OPTIONS
#-------------------------------------------------------------------------------
CC = clang++
CFLAG =  --std c++98 -g
# CFLAG = -Wall -Wextra -Werror --std c++98 -g
ASAN = -fsanitize=address
#-------------------------------------------------------------------------------
#	FILE NAMES
#-------------------------------------------------------------------------------
LIST_SRCS = \
	Cgi\
	Client\
	ConfigSet\
	FdSet\
	Http\
	HttpReq\
	HttpRes\
	Location\
	main\
	Method\
	MethodGet\
	MethodPost\
	MethodPut\
	Server\
	Socket\
	Stream\
	Time\
	Utils\
	Webserver

	
	
DIR_SRCS = ./srcs/srcs/
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

disk: 
	rm -rf ~/Library/Caches
	rm -rf ./.temp*

$(DIR_OBJS):
	mkdir $(DIR_OBJS)

$(NAME) : disk $(DIR_OBJS) $(OBJS)
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

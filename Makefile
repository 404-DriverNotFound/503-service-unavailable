NAME = webserv

#-------------------------------------------------------------------------------
#	COMPILE OPTIONS
#-------------------------------------------------------------------------------
CC = clang++
# CFLAG =  --std c++98 -g
CFLAG = -Wall -Wextra -Werror --std c++98
ASAN = -fsanitize=address
#-------------------------------------------------------------------------------
#	FILE NAMES
#-------------------------------------------------------------------------------
LIST_SRCS = \
	AConfig\
	Client\
	Cgi\
	State\
	StateBody\
	StateCgi\
	StateChunkedBody\
	StateChunkedBodyLen\
	StateChunkedBodyCRLF\
	StateChunkedBodyEnd\
	StateLoadBody\
	StateMethod\
	StateHead\
	StateSet\
	StateStartLine\
	StateDone\
	StateWait\
	ConfigGlobal\
	ConfigServer\
	ConfigLocation\
	FdSet\
	File\
	Http\
	HttpReq\
	HttpRes\
	main\
	Path\
	Socket\
	Stream\
	Time\
	Tokenizer\
	Utils\
	Webserver

	
	
DIR_SRCS = ./webserver/srcs/
DIR_OBJS = ./objs/
DIR_TMP = ./tmp/

SRCS = $(addprefix $(DIR_SRCS), $(addsuffix .cpp, $(LIST_SRCS)))
OBJS = $(addprefix $(DIR_OBJS), $(addsuffix .o, $(LIST_SRCS)))

BONUS = 0

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

$(DIR_TMP):
	mkdir $(DIR_TMP)

$(NAME) : $(DIR_OBJS) $(OBJS) $(DIR_TMP)
	$(CC) $(CFLAG) $(OBJS) -o $(NAME)

bonus : $(DIR_OBJS) set_bonus $(OBJS) $(DIR_TMP)
	$(CC) $(CFLAG) $(OBJS) -o $(NAME) -lpthread
	rm -rf bonus_file

set_bonus:
	touch bonus_file


asan : $(DIR_OBJS) $(OBJS)
	$(CC) $(ASAN) $(CFLAG) $(OBJS) -o $(NAME) 

$(DIR_OBJS)%.o : $(DIR_SRCS)%.cpp
	@if [ $$(find . -name bonus_file) ];\
	 then $(CC) $(CFLAG) -c $< -o $@ -D __BONUS__=1;\
	 else $(CC) $(CFLAG) -c $< -o $@ -D __BONUS__=0;\
	fi
	@echo compile: $<

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(DIR_TMP)
	$(RM) $(DIR_OBJS)
	$(RM) $(NAME)
	$(RM) ./root/multiple_same
	$(RM) ./root/file_should_exist_after

re: fclean all

#include <string>
#include <unistd.h>
#include <iostream>

size_t				strlen(const char* str);
int					strncmp(char* a, char* b, size_t len);

struct				Buffer
{
	int				fd;
	ssize_t			buffer_size;
	char*			buffer;
	char*			cursor;
	char*			end;
	int				flag_token;

					Buffer(int fd, size_t buffer_size);
	virtual			~Buffer();

	int				get_token(std::string& token, int seq);
	int				read_buffer();
	int				get_rest();
	bool			is_eof();	
};

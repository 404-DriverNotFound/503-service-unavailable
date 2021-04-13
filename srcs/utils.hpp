#include <string>
#include <unistd.h>
#include <iostream>

size_t				strlen(const char* str);
int					strncmp(char* a, char* b, size_t len);
bool	get_seq_token(std::string &origin, std::string::iterator &it, std::string &token, char *seq, size_t	reserve_size = 1000);
bool	get_set_token(std::string &origin, std::string::iterator &it, std::string &token, char *set, size_t	reserve_size = 1000);
bool	get_chr_token(std::string &origin, std::string::iterator &it, std::string &token, char c, size_t reserve_size = 1000);

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


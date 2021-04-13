#include <string>
#include <unistd.h>

size_t				strlen(const char* str)
{
	const char*		head = str;

	while (*str++)
	{}
	return str - head;
}

int					strncmp(char* a, char* b, size_t len)
{
	while (len-- && *a && *b)
	{
		if (a != b)
			return a - b;
		a++;
		b++;
	}
	return 0;
}

struct	Buffer
{
	int				fd;
	ssize_t			buffer_size;
	char*			buffer;
	char*			cursor;
	char*			end;
	int				flag_token;

					Buffer(size_t buffer_size);
	virtual			~Buffer();

	void			get_token_seq(std::string& token, char* seq);
	void			get_token_set(std::string& token, char* set);
	int				read_buffer();
	int				get_rest();
	bool			is_eof();	
};
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
					Buffer::Buffer(size_t buffer_size)
: buffer_size(buffer_size)
{
	buffer = new char[buffer_size + 1];
	cursor = buffer;
	end = buffer;
}

					Buffer::~Buffer()
{
	delete[] buffer;
}

int					Buffer::read_buffer()
{
	ssize_t			len = -1;
	if (cursor == buffer)
	{
		len = read(fd, buffer, buffer_size);
		end = buffer + len;
	}
	return len;
}

void				Buffer::get_token_seq(std::string& token, char* seq)
{
	ssize_t			len = read_buffer();
	ssize_t			len_seq = strlen(seq);

	while (cursor != end)
	{
		if (strncmp(seq, cursor, len_seq) == 0)
		{
			flag_token = 0;
			return ;
		}
		token.push_back(*cursor++);
	}




	flag_token = 1;
	return ;
}
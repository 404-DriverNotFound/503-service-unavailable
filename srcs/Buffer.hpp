#include <deque>
#include <string>
#include <unistd.h>

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

	int				get_token(std::string& token, int sep);
	int				get_token(std::deque<uint8_t>& token, int sep);
	int				read_buffer();
	ssize_t			size() const;
	ssize_t			write(size_t s);
};


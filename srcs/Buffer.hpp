#pragma once
#include <deque>
#include <string>
#include <unistd.h>
#include "utils.hpp"

struct				Buffer
{
	int				fd;
	ssize_t			buffer_size;
	char*			buffer;
	char*			cursor;
	char*			end;
	int				read_request;
	int				is_token_complete;
	ssize_t			write_request;
	ssize_t			len;

					Buffer(int fd, size_t buffer_size);
	virtual			~Buffer();

	void			get_token(std::string& token, int sep);
	void			get_token_seq(std::string& token, char* seq);
	char			get_token_set(std::string& token, char* set);
	int				read_buffer();
	ssize_t			size() const;
	void			write(size_t s, int fd);

	private:
};


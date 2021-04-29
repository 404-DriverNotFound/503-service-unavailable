#pragma once
#include <deque>
#include <string>
#include <unistd.h>
#include "Utils.hpp"

/*##############################################################################
Buffer
##############################################################################*/

using std::cout;
using std::endl;
using std::string;

// struct				Buffer
// {
// 	/*--------------------------------------------------------------------------
// 	Member
// 	--------------------------------------------------------------------------*/
// 	int				fd_in;				// 읽어들일 곳
// 	int				fd_out;				// 쓸 곳
// 	ssize_t			buffer_size;
// 	char*			buffer;
// 	char*			cursor;
// 	char*			end;
// 	bool			is_readable;		// 읽기 가능한지
// 	bool			is_writeable;		// 읽기 가능한지
// 	bool			is_token_complete;	// 토큰이 완전한지(불완전하면 이어붙임)
// 	ssize_t			remain_write;		// 덜 쓴 문자의 수
// 	ssize_t			len;

// 	/*--------------------------------------------------------------------------
// 	Method
// 	--------------------------------------------------------------------------*/
// 					Buffer();
// 					Buffer(int fd, size_t buffer_size = 0x100000);
// 					Buffer(const Buffer& x);
// 	Buffer&			operator=(const Buffer& x);
// 	virtual			~Buffer();

// 	void			init(int fd, size_t buffer_size = 0x100000);
// 	size_t			refill_buffer();
// 	void			get_token(std::string& token, int sep);
// 	void			get_token_seq(std::string& token, char* seq);
// 	char			get_token_set(std::string& token, char* set);
// 	ssize_t			size() const;
// 	size_t			write(size_t s);
// 	bool			is_read_req();
// };


#pragma once
#include <list>
#include "Utils.hpp"

using std::list;
using std::string;
using std::cout;
using std::endl;

/*##############################################################################
Buffer
##############################################################################*/

struct Buffer
{
	uint8_t*		start;
	uint8_t*		end;
	size_t			remain;
};

/*##############################################################################
Stream
##############################################################################*/

struct Stream
{
	/*--------------------------------------------------------------------------
	Members
	--------------------------------------------------------------------------*/
	int				fd_in;		// 클라이언트 소켓
	int				fd_out;		// 곧바로 데이터를 전송받을 fd
	list<Buffer>	buffers;
	uint8_t*		it_buffer;
	size_t			default_capacity;
	int				token_factor;
	size_t			pass_remain;

	/*--------------------------------------------------------------------------
	Methods
	--------------------------------------------------------------------------*/
	private:
					Stream();
					Stream(const Stream& x);
	public:
					Stream(size_t default_capacity, int fd_in, int fd_out = 1);
					~Stream();
	Stream&			operator=(const Stream& x);
	void			init(size_t default_capacity, int fd_in, int fd_out = 1);
	size_t			pass(size_t s);					// 스트림 내부 버퍼로부터 외부의 fd로 바로 쓰기
	size_t			pass();							// pass 최댓값
	ssize_t			fill(size_t s);					// 스트림 내부로 읽어들이기
	void			write(const string& str);		// 스트림에 데이터 쓰기
	void			write(uint8_t*	buff, size_t s);
	string			read(size_t s);					// 스트림 내부 버퍼로부터 밖으로 읽기
	size_t			read(size_t s, uint8_t* b);		// 스트림 내부 버퍼로부터 밖으로 읽기
	bool			get_seq_token(string &token, const char *seq);
	int				get_set_token(string &token, const char *set);
	bool			get_chr_token(string &token, const char c);
	void			get_set_token(list<string>& tokens, const char* set);
	void			get_chr_token(list<string>& tokens, const char* set);
	bool			get_line(string &token);
	void			add_buffer(size_t s);
	void			delete_buffer();
	Stream&			operator<<(const string& str);
	Stream&			operator>>(string& str);
	void			clear();
	size_t			size();
	void			print();
	void			print_line();
};
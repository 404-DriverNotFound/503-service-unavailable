#include <list>
#include "Utils.hpp"

using std::list;
using std::string;

struct Buffer
{
	uint8_t*		start;
	uint8_t*		end;
};

struct Stream
{
	int						fd_in;		// 클라이언트 소켓
	list<Buffer>			buffers;
	list<Buffer>::iterator	it_list;
	uint8_t*				it_buffer;

	ssize_t			read(uint8_t* b, size_t s);				// 스트림 내부 버퍼로부터 밖으로 읽기
	ssize_t			write(int fd, uint8_t* b, size_t s);	// 스트림 내부 버퍼로부터 외부의 fd로 바로 쓰기
	void			fill(size_t s);							// 클라이언트 소켓으로부터 새로운 버퍼 채우기
	bool			get_seq_token(string &token, const char *seq, size_t reserve_size = 1000);
	int				get_set_token(string &token, const char *set, size_t reserve_size = 1000);
	bool			get_chr_token(string &token, const char c, size_t reserve_size = 1000);
	void			get_set_token(list<string>& tokens, const char* set);
	void			get_chr_token(list<string>& tokens, const char* set);
};
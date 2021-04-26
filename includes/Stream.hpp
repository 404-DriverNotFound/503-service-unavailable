#include <list>
#include "Utils.hpp"

using std::list;
using std::string;

struct Buffer
{
	uint8_t*		start;
	uint8_t*		end;
	size_t			remain;
};

struct Stream
{
	int						fd_in;		// 클라이언트 소켓
	int						fd_out;		// 곧바로 데이터를 전송받을 fd
	list<Buffer>			buffers;
	list<Buffer>::iterator	it_list;
	uint8_t*				it_buffer;
	size_t					default_buffer_size;
	

					Stream();
					Stream(const Stream& x);
					~Stream();
	Stream&			operator=(const Stream& x);
	
	void			init(size_t default_capcity, int fd_in, int fd_out = 1);
	ssize_t			read(uint8_t* b, size_t s);				// 스트림 내부 버퍼로부터 밖으로 읽기
	ssize_t			write(int fd, uint8_t* b, size_t s);	// 스트림 내부 버퍼로부터 외부의 fd로 바로 쓰기
	void			fill(size_t s);							// 클라이언트 소켓으로부터 새로운 버퍼 채우기
	bool			get_seq_token(string &token, const char *seq);
	int				get_set_token(string &token, const char *set);
	bool			get_chr_token(string &token, const char c);
	void			get_set_token(list<string>& tokens, const char* set);
	void			get_chr_token(list<string>& tokens, const char* set);
};
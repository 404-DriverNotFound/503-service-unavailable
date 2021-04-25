#include <vector>
#include "Utils.hpp"

using std::vector;

struct Buffer
{
	uint8_t*		start;
	uint8_t*		end;
};

struct Stream
{
	vector<Buffer>	buffers;

	ssize_t			fill_buffer(int fd);						// 채운 바이트 수, 음수는 오류
	ssize_t			fill_buffer(uint8_t* start, uint8_t* end);	// 채운 바이트 수, 음수는 오류
	ssize_t			read(uint8_t* bf, size_t s);				// 읽은 바이트 수, 음수는 오류
	ssize_t			write(uint8_t* bf, size_t s);				// 쓴 바이트 수, 음수는 오류
};

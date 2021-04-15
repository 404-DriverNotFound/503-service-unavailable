#pragma once
#include <string>
#include <deque>
#include <vector>

class Location
{
	private:
		std::string					location;
	protected:
		std::string					root;
		std::vector<std::string>	error_page;
		std::vector<std::string>	index;
		std::vector<std::string>	method;
		u_int32_t					head_length;
		u_int64_t					body_length;
		bool						autoindex;
		u_int16_t					timeout;
		Location();
		void	chunked(std::vector<std::string>&);
	public:
		Location(std::deque<std::string>&);
		~Location();
};

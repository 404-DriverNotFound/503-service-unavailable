#include <cstring>
#include <string>

bool	get_seq_token(std::string &origin, std::string::iterator &it, std::string &token, char *seq, size_t	reserve_size = 1000)
{
	std::string::iterator	end = origin.end(); 
	int						seq_len;

	seq_len = std::strlen(seq);
	token.clear();
	token.reserve(reserve_size);
	while (it != end)
	{
		if (!strncmp(it.base(), seq, seq_len))
		{
			it += seq_len;
			return true;
		}
		token.push_back(*it);
		++it;
	}
	return false;
}
#include <cstring>
#include <string>

bool	get_seq_token(std::string &origin, std::string::iterator &it, std::string &token, char *seq, size_t	reserve_size = 1000)
{
    char                    *s, *e;
	int						seq_len;

    s = it.base();
    e = origin.end().base();
	seq_len = std::strlen(seq);
	token.clear();
	token.reserve(reserve_size);
	while (s != e)
	{
		if (!strncmp(s, seq, seq_len))
		{
			s += seq_len;
            it += seq_len;
			return true;
		}
		token.push_back(*s);
		s++;
        ++it;
	}
	return false;
}
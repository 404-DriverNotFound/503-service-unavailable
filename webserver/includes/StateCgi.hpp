#include "State.hpp"

class StateCgi: public State
{
	public:
		StateCgi();
		~StateCgi();
		State*	action(Client& ref);
		void	terminate_cgi(Client& ref);
};
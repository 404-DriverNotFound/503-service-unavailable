// #include "../includes/ClassModule.hpp"
// #include "../includes/ConfigGlobal.hpp"
#include "../includes/Webserver.hpp"

int		main(int argc, char** argv, char** env)
{
	set<string>		s;

	cout << s << endl;


	try
	{
		Webserver::init_static_members(argc, argv, env);
		cout << Webserver::config << endl;
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
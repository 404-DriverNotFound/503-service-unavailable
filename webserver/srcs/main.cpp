// #include "../includes/ClassModule.hpp"
#include "../includes/ConfigGlobal.hpp"

int		main(int argc, char** argv, char** env)
{
	set<string>		s;

	cout << s << endl;


	try
	{
		ConfigGlobal	global_config(argc, argv, env);

		cout << global_config << endl;
		// Webserver		webserver(global_config);
		
		// webserver.start_server();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
// #include "../includes/ClassModule.hpp"
#include "../includes/ConfigGlobal.hpp"

int		main(int argc, char** argv, char** env)
{
	try
	{
		ConfigGlobal	global_config(argc, argv, env);
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
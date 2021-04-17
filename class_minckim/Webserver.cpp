#include "Webserver.hpp"

void			Webserver::start_server()
{
	int		result;

	while (42)
	{
		to_be_checked_read = to_be_checked;
		to_be_checked_write = to_be_checked;
		to_be_checked_exception = to_be_checked;
		result = select(config.max_connection, &to_be_checked_read, &to_be_checked_write, &to_be_checked_exception, &config.select_timeout);
		if (result < 0)
			throw SelectFailed();
		if (result == 0)	// timeout
			continue;
		check_new_connection();
		manage_clients();
	}
}

void			Webserver::check_new_connection()
{
	server_iterator		end = servers.end();
	for (server_iterator it = servers.begin() ; it != end ; ++it)
	{
		if (to_be_checked_read.get(it->sock.fd) == 0)
			continue;
		clients.push_back(it->sock.fd);
	}
}

void			Webserver::manage_clients()
{
	client_iterator		end = clients.end();
	for (client_iterator it = clients.begin() ; it != end ; ++it)
	{
		it->client_process(to_be_checked_read, to_be_checked_write);
	}
}
#include "../includes/ConfigSet.hpp"

ConfigSet::ConfigSet()
{}

ConfigSet::ConfigSet(const ConfigSet& x)
: name(x.name),
root(x.root),
auth(x.auth),
error_page(x.error_page),
index(x.index),
method(x.method),
head_length(x.head_length),
body_length(x.body_length),
autoindex(x.autoindex),
timeout(x.timeout)
{}

ConfigSet::~ConfigSet()
{}

ConfigSet&	ConfigSet::operator=(const ConfigSet& x)
{
	if (&x == this)
		return *this;
	name = x.name;
	root = x.root;
	auth = x.auth;
	error_page = x.error_page;
	index = x.index;
	method = x.method;
	head_length = x.head_length;
	body_length = x.body_length;
	autoindex = x.autoindex;
	timeout = x.timeout;
	return *this;
}
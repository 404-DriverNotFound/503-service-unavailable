#include "../includes/ConfigSet.hpp"

ConfigSet::ConfigSet()
: method(0),
head_length(8000),
body_length(1000000),
autoindex(false),
timeout(5000000)
{}

ConfigSet::ConfigSet(const ConfigSet& x)
: name(x.name),
root(x.root),
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
	error_page = x.error_page;
	index = x.index;
	method = x.method;
	head_length = x.head_length;
	body_length = x.body_length;
	autoindex = x.autoindex;
	timeout = x.timeout;
	return *this;
}

const char*		ConfigSet::InvalidConfig::what() const throw()
{
	return "Invalid config";
}
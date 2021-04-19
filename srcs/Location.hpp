#pragma once
#include <string>
#include <deque>
#include <vector>
#include "DataClass.hpp"

struct Location : public ConfigSet
{
	public:
		std::string		location;
		Location();
	public:
		Location(std::deque<std::string>&);
		~Location();
};

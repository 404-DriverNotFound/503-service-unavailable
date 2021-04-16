#pragma once
#include <string>
#include <deque>
#include <vector>
#include "DataClass_sujung.hpp"

class Location : public ConfigSet
{
	private:
		std::string					location;
		Location();
	public:
		Location(std::deque<std::string>&);
		~Location();
};
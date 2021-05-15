#pragma once
#include "Utils.hpp"
#include "Tokenizer.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <set>

using std::string;
using std::map;
using std::set;

/*##############################################################################
Config Server
##############################################################################*/

class AConfig
{
	/*==========================================================================
	Member
	==========================================================================*/
	protected:
		string				_name;
		string				_root;
		set<string>			_index_page;
		set<string>			_error_page;
		set<string>			_method;
		size_t				_head_max;
		size_t				_body_max;
		bool				_autoindex;

	/*==========================================================================
	Constructor & Destructor
	==========================================================================*/
	private:
	public:
		AConfig();
		AConfig(const AConfig& ref);
		AConfig(map<string, string>& config_server_map);
		AConfig& operator=(const AConfig& ref);
		virtual ~AConfig();

	/*==========================================================================
	Method
	==========================================================================*/
	protected:
		void				set_set(set<string>& container, string& val);

	/*==========================================================================
	Setter
	==========================================================================*/
	public:
		void				set_name(string& val);
		void				set_root(string& val);
		void				set_index_page(string& val);
		void				set_error_page(string& val);
		void				set_method(string& val);
		void				set_head_max(string& val);
		void				set_body_max(string& val);
		void				set_autoindex(string& val);

	/*==========================================================================
	Getter
	==========================================================================*/
	public:
		const string&		get_name() const;
		const string&		get_root() const;
		const set<string>&	get_index_page() const;
		const set<string>&	get_error_page() const;
		const set<string>&	get_method() const;
		size_t				get_head_max() const;
		size_t				get_body_max() const;
		bool				get_autoindex() const;

	/*==========================================================================
	Exception
	==========================================================================*/
	public:
		class ConfigElementInvalid: public std::exception
		{	public: const char* what() const throw();	};

		class ConfigElementEmpty: public std::exception
		{	public: const char* what() const throw();	};
};

std::ostream&		operator<<(std::ostream& os, const AConfig& conf);
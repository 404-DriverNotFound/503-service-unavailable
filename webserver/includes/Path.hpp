#pragma once
#include "Tokenizer.hpp"
#include <string>
#include <list>
#include <iomanip>
#include <iostream>

using std::list;
using std::string;

/*##############################################################################
Config Global
##############################################################################*/

class Path
{
	public:
		enum flag
		{
			flag_not_exist,
			flag_cgi,
			flag_dir,
			flag_file,
		};
	/*==========================================================================
		Member
	==========================================================================*/
	public:
	// private:
		flag			_flag;
		string			_path_info;
		string			_query;
		list<string>	_segments;
		string			_extension;
		string			_path_translated;
	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	public:
		Path();
		Path(string& raw);
		Path(const string& ref);
		~Path();
		Path&	operator=(Path& ref);
	/*==========================================================================
		Method
	==========================================================================*/
	private:
		void			assemble_path(string& x, const list<string>& segments);
		void			arrange_segment(list<string>& segments);
	public:
		void			clear();

	/*==========================================================================
		Setter
	==========================================================================*/
	public:
		void			set_path(string& raw);
		void			set_root(const string& root);
		bool			set_index_page(const set<string>& pages);
	private:
		void			set_flag();
		void			set_path_info();
		void			set_path_translated();
		void			set_extension(string& segment);

	/*==========================================================================
		Getter
	==========================================================================*/
	public:
		const string&		get_path_info() const;
		const string&		get_path_translated() const;
		const list<string>&	get_segments() const;
		flag				get_flag() const;
		// bool				is_cgi() const;
		// bool				is_dir() const;
		// bool				is_file() const;
		// bool				is_exist() const;
};


/*
쿼리와 path_info 생성

로케이션후보 = 경로조각.fornt()

로케이션 목록에서 로케이션후보와 매치되는 것을 찾기
찾으면 경로조각.front()를 해당 후보의 루트와 교체
못찾으면 그대로
---
cgi이면 cgi 실행
{
	임시파일
	임시파일
}

디렉로리이면 get
{
	임시파일
	경로에 인덱스 추가
}

나머지 경우
GET :
없으면: 오토인덱스면: 오토인덱스 페이지, 오토인덱스 아니면 에러페이지
있으면: 파일 전송
{
	임시파일
	경로 그대로
}

POST : 파일 이어쓰기
{
	경로 그대로
	임시파일
}
PUT : 파일 덮어쓰기
{
	경로 그대로
	임시파일
}
DELETE : 



*/
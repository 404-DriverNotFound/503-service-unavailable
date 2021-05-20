#pragma once
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <vector>
#include "Utils.hpp"
#include "Stream.hpp"

/*##############################################################################
Cgi
##############################################################################*/
/*
 * 복사생성자는 실제로 복사를 해주는 기능이 없고, 단지 컨테이너에 들어가기 위한 것임.
 * init으로 초기화를 해야 함.
 * 
 * 멤버 명세
 * -----------------------------------------------------------------------------
 * - pid = CGI가 실행됐을 때의 pid
 * - status
 * - is_exit = 종료 상태 (참이면 종료)(check_exit()으로 갱신)
 * - return_code = 반환값 (check_exit()으로 갱신)
 * - fd_in = 읽기용
 * - fd_out = 출력용
 * - 
 * - 임시파일(요청 본문)                    임시파일(응답 본문)
 * - fd_in --------------> CGI -----------> fd_out
 * -
 * -
 * -
 * 
 * - path = 실행할 스크립트의 경로 (init의 인자)
 * - extention = init에서 생성
 * - meta_variable = init의 인자
 * - cgi_bin = 스크립트 인터프리터의 경로 트리(cgi_bin[extention]으로 경로 얻음)
 *
 * 함수 명세
 * -----------------------------------------------------------------------------
 * - init(스크립트의 경로, 메타변수) : 초기화. 특히 파이프 초기화
 * - start_cgi() : 포크, 각 파이프를 dup2로 복사해서 가져감
 * - set_extention() : 스크립트 경로에서 확장자 추출, extention 초기화
 * - make_argv() : execve(000, argv, 000)을 위한 함수
 * - check_exit() : 종료상태와 종료코드 확인
 * - destroy_pipe() : 파이프 닫음
 * - teminate() : cgi 종료
 * 
 * 사용 방법
 * -----------------------------------------------------------------------------
 * cgi.init(스크립트, 메타변수);
 * cgi.start_cgi();
 * ................................
 * 
 *  cgi.fd_in 에 데이터 전송
 *  cgi.fd_out 으로부터 데이터 받음
 * 
 * ................................
 * cgi.terminate();


*/
using std::vector;
using std::map;
using std::string;

struct Cgi
{
	/*--------------------------------------------------------------------------
	Typedef
	--------------------------------------------------------------------------*/
	public:
	typedef map<string, string>	 map_path;

	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	public:
	int					pid;
	int					status;
	int					is_exit;
	int					return_code;
	string&				path;
	string&				extension;
	int					fd_in;
	int					fd_out;
	vector<string>		meta_variables;

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	private:
	/*constructor*/	Cgi();
	/*constructor*/	Cgi(const Cgi& x);
	Cgi&			operator=(const Cgi& x);

	public:
	/*constructor*/	Cgi(string& path, string& extension, int fd_in, int fd_out);
	/*destructor*/	~Cgi();
	void			start_cgi();
	void			set_extension();
	char * const*	make_argv();
	char * const*	make_meta_variable();
	bool			check_exit();

	/*--------------------------------------------------------------------------
	Static Method
	--------------------------------------------------------------------------*/	
};
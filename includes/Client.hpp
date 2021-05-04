#pragma once
struct Client;
#include <sys/types.h>
#include "Server.hpp"
#include "Stream.hpp"
#include "Socket.hpp"
#include "HttpReq.hpp"
#include "HttpRes.hpp"
#include "Cgi.hpp"
#include "FdSet.hpp"
#include "Server.hpp"
#include "Time.hpp"

#define BUFFER_SIZE		16

using std::string;
using std::map;

enum e_status
{
	STATUS_START_LINE,
	STATUS_HEADER,
	STATUS_CHECK_MSG,
	STATUS_METHOD,
	STATUS_SEND_MSG,
	// STATUS_SEND_CGI,
	STATUS_DONE,
};

enum e_status_proc
{
	STATUS_INIT,
	// PROC_CGI,
	// END_CGI,
	STATUS_RECV_CHUNKED_BODY,
	STATUS_RECV_BODY,
	STATUS_RECV_DONE
	// RECV_BODY_CHUNKED,
	// MAKE_MSG,
};

enum e_status_recv
{
	STATUS_LEN,
	STATUS_BODY,
	STATUS_NL,
};

/*##############################################################################
Client
##############################################################################*/
/*
복사생성자때문에, 소켓 초기화는 init함수로 외부에서 진행함
프로세스 함수 외부에서 셀렉트 결과에 따라 스트림in에 수신 데이터가 채워짐

================================================================================
프로세스:
스위치 (상태)
----------------------------------------------------------------------
시작줄:
	스트림으로부터 한 줄 얻기
	줄이 완벽하면 
		시작줄로 구문분석 한다
		상태를 헤더로 변경
	상태가 "시작줄" 상태 그대로이면 리턴
----------------------------------------------------------------------
헤더:
	스트림으로부터 한 줄 얻기
	if 내용이 없으면
		요청 바디의 길이가 0이면
			상태를 메시지 처리로 변경
		대기시간 체크, 초과했으면
			상태 = 메시지 처리
	else if 빈 줄이면
		상태를 메시지 처리로 변경
	else if 줄이 완벽하면
		헤더로 구문분석 한다 -> 일단은 헤더별로 값만 저장
		요청 바디의 길이는 별도로 저장한다.
	else ( == 줄이 불완전하면)
		리턴
	상태가 "헤더" 상태 그대로이면 리턴
----------------------------------------------------------------------
메시지 처리:
	서버 목록에서 서버 찾기(호스트 헤더)
	서버에서 로케이션 찾기(path_info의 첫 번째 경로조각이거나, 경로조각이 1개인 경우 "/")
		로케이션을 찾지 못하면 not_found
	권한 및 메서드 허용여부 확인
	확장자 확인. 확장자가 CGI가 필요한 확장자인 경우는 CGI 실행
		메타변수 생성
		CGI 초기화
		스트림in을 CGI로 연결
		CGI 시작
	상태를 메서드로 변경
	메서드 분기 함수 실행
----------------------------------------------------------------------
메서드:
	메서드 분기 함수 실행
	상태가 "메서드" 상태 그대로이면 리턴
----------------------------------------------------------------------
종료:



================================================================================
메서드 분기 함수:
스위치 (메서드)
----------------------------------------------------------------------
GET:
	
	if CGI (O):
		CGI실행
		빈 줄 나올 때까지 CGI.stream_out.getline()
			stream_out << 헤더를 파싱
	stream_out << 리소스
	파일 사이즈 확인 -> Content-Length 결정
	stream_out << 헤더 작성(시작줄, 헤더)
	상태 = 메시지 전달
----------------------------------------------------------------------
HEAD:
	
	if CGI (O):
		CGI실행
		빈 줄 나올 때까지 CGI.stream_out.getline()
	파일 사이즈 확인 -> Content-Length 결정
	stream_out << 헤더 작성(시작줄, 헤더)
	if CGI (O):
		stream_out << 헤더를 파싱
	상태 = 메시지 전달
----------------------------------------------------------------------
POST:
	if CGI (O)
		CGI실행
		chunked일 경우
			본문에서 길이 얻기
			길이가 완벽하면
				길이만큼 CGI에 본문 전달
			완벽하지 않으면
				남은 길이를 저장
		chunked 아닐 경우
			남은 콘텐츠 길이만큼 CGI에 본문 입력
		빈 줄 나올 때까지 CGI.stream_out.getline()
			stream_out << 헤더를 파싱	
	stream_out << 헤더 작성(시작줄, 헤더)

PUT:
	리소스 쓰기
DELETE:
	리소스 삭제
...




*/
struct Client
{
	/*--------------------------------------------------------------------------
	Member types
	--------------------------------------------------------------------------*/
	typedef map<string, string>::iterator		iterator_header;
	typedef map<string, string>::iterator		iterator_host;
	typedef map<string, Server>::iterator		iterator_server;
	typedef map<string, Location>::iterator		iterator_location;
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	public:
	Socket					sock;				// 클라이언트의 소켓
	Stream					stream_in;			// 데이터를 받는 스트림
	Stream					stream_out;			// 데이터를 보내는 스트림
	string					line;				// 스트림으로부터 추출한 토큰이 완벽하지 않을 경우를 위한 저장소
	e_status				status;				// 현재 처리 상태
	e_status_proc			status_proc;		// 구체적인 처리 상태
	e_status_recv			status_recv;
	HttpReq					req;				// 요청메시지
	HttpRes					res;				// 응답메시지
	Cgi						cgi;				// CGI
	string					location_name;		// 로케이션 이름
	string					path_translated;	// 실제 경로
	char**					meta_variables;
	Server*					server;				// 참조할 서버
	Location*				location;			// 로케이션
	map<string, Server>&	servers;			// 로케이션과 서버를 결정하기 위해 참조할 서버 목록(포트에 종속된 서버 목록)
	Time					alive;

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	private:
				Client();
	public:
				Client(const Client&);
				Client(map<string, Server>& servers);
	virtual		~Client();
	Client&		operator=(const Client&);
	void		init(int fd);

	void		client_process(FdSet& r, FdSet& w);
	
	void		recv_start_line();
	void		recv_header();

	void		set_server();
	void		set_location();
	void		translate_path();
	void		check_auth();
	void		check_method();


	void		process_method();





	void		process_msg();
	void		process_get();
	void		process_head();
	size_t		process_head_base();
	void		process_put();
	void		process_post();
	void		process_options();
	void		process_delete();
	
	void		send_msg(FdSet& w);
	void		send_get();
	void		send_put();
	void		send_post();
	void		send_options();
	void		send_delete();


	void		recv_body(size_t len);
	void		recv_body_chunked();
	void		proc_cgi();
	char**		make_meta_variable();
	void		terminate_cgi();

	void		replace_location();
};	//Client

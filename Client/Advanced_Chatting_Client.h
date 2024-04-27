#ifndef AD_CHAT_CLIENT_H
#define AD_CHAT_CLIENT_H

#include <time.h>						// _strtime_s 함수 사용
#include <stdio.h>						// sprintf_s 함수 사용
#include <shellapi.h>					// ShellExecute 함수 사용
#define _USE_INIT_WINDOW_				// 윈도우 전역 속성 초기화 함수 직접 지정
#include "tipsware.h"					// EasyWin32 사용
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")		// sndPlaySound 함수 사용

#define MAX_COUNT	5					// 실행할 프로그램 최대 개수
#define WIN_WIDTH	660					// 클라이언트 영역의 너비 설정
#define WIN_HEIGHT	800					// 클라이언트 영역의 폭 설정
#define WIN_COLOR	RGB(82, 97, 124)	// 전체 색상을 RGB(82, 97, 124)로 지정
#define TEXT_COLOR	RGB(128, 202, 128)  // 에디트 박스 제목 색상 지정

// 실행할 프로그램을 저장한 문자열 
const char *gp_name_table[MAX_COUNT] = { "메모장", "그림판", "탐색기", "엣지", "크롬" };
// 실행할 프로그램(확장자 포함)
const char *gp_cmd_table[MAX_COUNT] = { "notepad.exe", "mspaint.exe", "explorer.exe", "msedge.exe", "chrome.exe" };

// 컨트롤 타입에 대한 아이디 열거체
enum CtrlTypeId
{
	IDC_CONNECT_BTN = 1000,		// 서버에 접속 버튼의 아이디
	ID_NOTIFY = 1000,			// 컨트롤 조작 상태의 아이디
	IDC_DISCONNECT_BTN,			// 접속 해제 버튼의 아이디
	IDC_SEND_CHAT_BTN,			// 채팅을 전송하는 버튼의 아이디
	IDC_DEL_CHAT_BTN,			// 채팅 내용을 초기화 하는 버튼의 아이디
	IDC_PLAY_BTN,				// 음악 재생 버튼의 아이디
	IDC_STOP_BTN,				// 음악 중지 버튼의 아이디
	IDC_SCREEN_CAP_BTN,			// 화면 캡처 버튼의 아이디
	IDC_EVENT_LIST = 2001,		// 상태 목록을 관리하는 리스트 박스의 아이디
	IDC_KEY_LIST,				// 후킹 데이터를 표시하는 리스트 박스의 아이디
	IDC_SERVER_IP_EDIT = 3000,	// 서버 주소 입력용 에디트 컨트롤의 아이디
	IDC_USER_ID_EDIT,			// 아이디 입력용 에디트 컨트롤의 아이디
	IDC_USER_PW_EDIT,			// 패스워드 입력용 에디트 컨트롤의 아이디
	IDC_INPUT_CHAT_EDIT			// 채팅 입력용 에디트 컨트롤의 아이디
};

// 채팅 데이터 전달의 종류에 따른 열거체
enum ChatDataDeliveryType
{
	REQ_USER_CHAT = 21,			// C -> S 채팅 데이터 전달
	ANS_USER_CHAT				// S -> C 채팅 데이터 전달(브로드캐스팅)
};

// 프로그램에서 사용할 내부 데이터
typedef struct ApplicationData
{
	NeoSocketData client;		// 서버 소켓 서비스를 위해 필요한 정보
	HINSTANCE h_key_hook_dll;	// 키보드 후킹 관련 DLL 핸들
	TW_AID walk_image;			// png 이미지의 위치, 개수, 목록을 저장한 구조체
	void *p_event_list;			// 프로그램 상태 목록을 관리하는 리스트 박스 주소
	void *p_con_btn;			// 서버 접속 버튼 주소
	void *p_discon_btn;			// 서버 접속 해제 버튼 주소
	void *p_send_btn;			// 채팅 전송 버튼 주소
	void *p_server_ip_edit;		// 사용자 ip 입력하는 에디트 컨트롤 주소
	void *p_user_id_edit;		// 사용자 id 입력하는 에디트 컨트롤 주소
	void *p_user_pw_edit;		// 사용자 pw 입력하는 에디트 컨트롤 주소
	void *p_in_chat_edit;		// 사용자가 채팅 내용을 입력하는 에디트 컨트롤 주소
	void *p_play_btn;			// 음악 재생 버튼의 주소
	void *p_stop_btn;			// 음악 중지 버튼의 주소
	void *p_del_chat;			// 채팅 삭제 버튼의 주소
	void *p_screen_cap;			// 화면 캡처 버튼의 주소
	void *p_key_list;			// 입력된 키 목록을 저장할 리스트 박스 주소
}AD;

// 후킹 루틴 설치에 사용될 설치 함수
typedef void (*FP_KEY_HOOK_INSTALL)(HWND ah_target_wnd, int a_msg_id, int a_filter_count, UINT8 a_filter_list[]);


#endif // !AD_CHAT_CLIENT



#ifndef AD_CHAT_SERVER_H
#define AD_CHAT_SERVER_H

#include <stdlib.h>
#define _USE_INIT_WINDOW_  // 윈도우 전역 초기화 함수를 직접 구현하도록 지정 (InitWindow)
#include <stdio.h> // sprintf_s, fopen_s, fclose 함수를 사용하기 위해
#include "tipsware.h"

enum TpyeCtrlId
{
    IDC_SERVICE_START_BTN = 1000,   // 서비스 시작 버튼의 아이디
    IDC_SERVICE_STOP_BTN,           // 서비스 멈춤 버튼의 아이디
    IDC_MODIFY_USER_BTN = 1100,     // 사용자 정보 변경 버튼의 아이디
    IDC_DEL_USER_BTN,               // 사용자 정보 삭제 버턴의 아이디
    IDC_DEL_CHAT_BTN,               // 채팅창 내용 전부 삭제
    IDC_USER_LIST  = 2000,          // 사용자 목록을 관리하는 리스트 박스의 아이디
    IDC_EVENT_LIST,                 // 상태 목록을 관리하는 리스트 박스의 아이디
    IDC_IP_COMBO = 3000,            // 서비스 아피 목록을 관리하는 콤보 박스의 아이디
    IDC_USER_LEVEL_COMBO,           // 사용자 등급 목록을 관리하는 콤보 박스의 아이디
    IDC_USER_ID_EDIT = 4000,        // 아이디 입력용 에디트 컨트롤의 아이디
    IDC_USER_PASSWORD_EDIT,         // 암호 입력용 에디트 컨트롤의 아이디
    IDC_USER_NAME_EDIT,             // 이름 입력용 에디트 컨트롤의 아이디
    IDC_USER_ETC_EDIT,              // 기타 정보 입력용 에디트 컨트롤의 아이디
};

enum DataType
{
    REQ_USER_CHAT,    // C -> S 채팅 데이터 전달
    ANS_USER_CHAT     // S -> C 채팅 데이터 전달 (브로드 캐스팅)
};


// 이 서버가 사용할 회원 등급 목록 (9단계)
const char *gp_level_string[] = { "손님", "임시회원", "일반회원", "성실회원", "우수회원", "공동체", "운영진", "VIP", "관리자" };
int g_level_len[] = { 4, 8, 8, 8, 8, 6, 6, 3, 6 };

typedef struct ApplicationData  // 프로그램에서 사용할 내부 데이터
{
    NeoServerData server_data;  // 서버 소켓 서비스를 위해 필요한 정보
    void *p_user_list;          // 사용자 목록을 관리하는 리스트 박스
    void *p_event_list;         // 프로그램 상태 목록을 관리하는 리스트 박스
} AD;


#endif // !AD_CHAT_SERVER_H


// Visual Studio 2022 x86 Release Mode
// Advanced_Chatting_Client.cpp

#include "pch.h"						// 컴파일 속도 향상
#include "Advanced_Chatting_Client.h"	// ew32 외 필요한 헤더 파일이 내장된 헤더파일

// 자신이 사용하고자 하는 윈도우 전역 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "로그인 기반 채팅 클라이언트";
	// 윈도우 속성 변경
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 각 항목을 선택 여부에 따라 다른 색상으로 출력하는 함수
// a_index: 항목의 위치, a_select: 선택(1), 선택안함(0)*/
void DrawItem(int a_index, int a_select)
{
	// 선택 여부에 따른 색상 값을 배열로 설정(조건문을 줄이기 위해)
	COLORREF fill_color[2] = { RGB(0, 0, 128), RGB(0, 100, 255) };
	COLORREF text_color[2] = { RGB(200, 200, 200), RGB(255, 255, 255) };

	// 사각형과 사각형 내부에 명령어에 해당하는 문자열을 출력
	Rectangle(10 + a_index * 120, 320, 10 + (a_index + 1) * 120, 360, RGB(0, 100, 200), fill_color[a_select]);
	printf(50 + a_index * 120, 330, text_color[a_select], gp_name_table[a_index]);
}

// 마우스 왼쪽 버튼을 누른 경우
void OnLeftBtnDown(int a_mixed_key, POINT a_pos)
{
	// 격자 방식 사용.
	unsigned int x = (a_pos.x - 20) / 120, y = (a_pos.y - 320) / 40;
	if (x < MAX_COUNT && y < 1)
	{
		for (int i = 0; i < MAX_COUNT; ++i)
		{
			// 5개의 사각형을 모두 어두운 파란색으로 출력	
			DrawItem(i, 0);
		}
		// 사용자가 선택한 위치에 해당하는 사각형만 밝은 색 사각형으로 다시 그림
		DrawItem(x, 1);
		// 사용자가 선택한 명령어를 수행
		ShellExecute(NULL, "open", gp_cmd_table[x], NULL, NULL, SW_SHOW);
	}
	// 정보를 윈도우에 출력
	ShowDisplay();
}

// 이벤트를 목록에 추가하는 함수
void AddEventString(AD *ap_app, const char *ap_str)
{
	// 현재 프로그램의 상태를 리스트 박스에 추가
	ListBox_InsertString(ap_app->p_event_list, -1, ap_str, 0);
}

// 실행된 후킹 작업을 정리하는 함수
void UninstallKeyHooking(AD *ap_app)
{
	if (ap_app->h_key_hook_dll) {  // 키보드 후킹 작업이 시작되었는지 체크한다.
		// 키보드 후킹을 중지하는 함수의 주소를 얻는다.
		void(*fp)() = (void(*)())GetProcAddress(ap_app->h_key_hook_dll, "UninstallKeyHook");
		if (fp != NULL) (*fp)();  // 키보드 후킹을 중지한다.
		::FreeLibrary(ap_app->h_key_hook_dll);  // 후킹에 사용될 DLL을 정리한다.
		ap_app->h_key_hook_dll = NULL;  // 변수 초기화
	}
}

// 윈도우에 발생하는 일반 메시지를 처리하는 함수
int OnUserMsg(HWND ah_wnd, UINT a_msg_id, WPARAM wParam, LPARAM lParam)
{
	AD *p_app = (AD *)GetAppData();
	// 접속 여부
	if (CLIENT_SOCKET_CONNECT == a_msg_id) ProcessConnectionResult(&p_app->client, WSAGETSELECTERROR(lParam));
	// 데이터 처리
	else if (CLIENT_SOCKET_COMMON == a_msg_id) ProcessSocketEvent(&p_app->client, WSAGETSELECTEVENT(lParam));
	// 키보드로 문자 한 개를 입력할 경우
	else if (26001 == a_msg_id) {
		// 키보드 입력에 따른 문자 데이터 정보를 저장하는 배열
		char temp_str[64];
		// 문자열의 현재 상태값을 표시
		sprintf_s(temp_str, 64, "wParam: %08X, lParam: %08X, ASCii Key: %c", wParam, lParam, toascii(wParam));
		// 키 한 개가 눌릴 때마다 리스트 박스의 새로운 위치에서 문자 출력
		ListBox_InsertString(p_app->p_key_list, -1, temp_str);
	}
	return 0;
}

// 사용자의 상태 통보를 사용하는 함수
void ClientSocketNotifyProc(NeoSocketData *ap_data, UINT32 a_state)
{
	// 서버에 연결 성공
	//if (a_state == 1) 
	//{
	//	MessageBox(gh_main_wnd, "연결 성공!!", "서버 연결 상태", MB_ICONINFORMATION);
	//}
	//// 서버와 연결 해제
	//if (a_state == 50) 
	//{
	//	MessageBox(gh_main_wnd, "연결 해제!!", "서버 연결 상태", MB_ICONERROR);
	//}
}

// 네트워크 메시지를 처리하는 함수
int ProcessingSocketMessage(NeoSocketData *ap_data)
{
	// 프로그램에 저장된 데이터를 얻음
	AD *p_app = (AD *)GetAppData();
	switch (ap_data->msg_id)
	{
	// 클라이언트 -> 서버 접속할 경우
	case ANS_WELCOME_MSG:
	{
		// 로그인 구조체
		LoginData login_data;
		// 아이디
		GetCtrlName(FindControl(IDC_USER_ID_EDIT), login_data.id, 32);
		// 비밀번호
		GetCtrlName(FindControl(IDC_USER_PW_EDIT), login_data.password, 32);
		// 로그인 시도
		SendFrameData(&p_app->client, REQ_LOGIN_DATA, &login_data, sizeof(login_data));
	}
	break;
	// 서버에 성공적으로 접속할 경우
	case ANS_LOGIN_OK:
	{
		AddEventString(p_app, "서버에 성공적으로 로그인했습니다!");
		MessageBox(gh_main_wnd, "연결 성공!!", "서버 연결 상태", MB_ICONINFORMATION);
	}
	break;
	// 서버 접속에 실패할 경우(로그인 실패)
	case ANS_LOGIN_FAIL:
	{
		AddEventString(p_app, (char *)ap_data->p_recv_data);
		// 소켓 객체 제거
		DestroySocket(&p_app->client);
		AddEventString(p_app, "서버 접속에 실패했습니다!");
		MessageBox(gh_main_wnd, "로그인 실패!!", "서버 연결 상태", MB_ICONERROR);
	}
	break;
	// 서버가 보내준 채팅 정보
	case ANS_USER_CHAT:
	{
		AddEventString(p_app, (char *)ap_data->p_recv_data);
	}
	break;
	// 서버가 보내준 내부 시스템 메시지
	case ANS_SYSTEM_MESSAGE:
	{
		AddEventString(p_app, (char *)ap_data->p_recv_data);
	}
	break;
	default:
	break;
	}
	return 1;
}

// 반복적으로 수행할 작업(png 애니메이션, 시계)을 표시하는 함수
TIMER RedrawTimer(NOT_USE_TIMER_DATA)
{
	AD *p_data = (AD *)GetAppData();
	// 현재 시간을 출력하는 요소
	Rectangle(550, 20, 655, 40, RGB(0, 100, 255), RGB(0, 0, 120));
	char str[32];
	_strtime_s(str, 32);
	SelectFontObject("굴림체", 18, 1);
	TextOut(565, 20, RGB(0, 200, 0), str);
	// 걷는 이미지 출력
	DrawImageAID(&p_data->walk_image, 505, 17, 0.35, 0.18);
	// 정보를 윈도우에 출력
	ShowDisplay();
}

// 윈도우가 종료될 때 호출될 함수
void OnDestroy()
{
	AD *p_app = (AD *)GetAppData();
	CleanUpClientData(&p_app->client);
	UninstallKeyHooking(p_app);
	KillTimer(1);
}

// 사용자 채팅 메시지를 서버로 전송하는 함수
void SendChatData(AD *ap_app)
{
	// 로그인된 상황에서만 사용
	if (ap_app->client.is_connected == 3)
	{
		char str[512];
		// 채탕 입력용 에디트 컨트롤
		void *p_edit = FindControl(IDC_INPUT_CHAT_EDIT);
		// 입력된 문자열의 길이를 얻음
		int len = Edit_GetLength(p_edit);
		// 입력한 채팅 정보를 얻음
		GetCtrlName(p_edit, str, 512);
		// 사용자가 입력한 정보를 서버로 전송
		SendFrameData(&ap_app->client, REQ_USER_CHAT, str, len + 1);
		// 입력된 채팅 글을 지움
		SetCtrlName(p_edit, "");
	}
}

// 컨트롤을 조작하는 함수
void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, void *ap_ctrl)
{
	// 프로그램 내부 데이터를 얻으
	AD *p_app = (AD *)GetAppData();
	switch (a_ctrl_id)
	{
	// [서버에 접속] 버튼을 누른 경우
	case IDC_CONNECT_BTN:
	{
		// 서버와 연결되지 않은 상태에서만 서버에 접속
		if (p_app->client.h_socket == INVALID_SOCKET)
		{
			char str[32];
			// 사용자가 입력한 서버 주소를 얻음
			GetCtrlName(FindControl(IDC_SERVER_IP_EDIT), str, 32);
			// 서버에 접속 시도
			ConnectToServer(&p_app->client, str, 12023);
			AddEventString(p_app, "서버에 접속을 시도 합니다!");
		}
		else AddEventString(p_app, "이미 서버에 접속된 상태입니다!");
	}
	break;
	// [접속 해제] 버튼을 누른 경우
	case IDC_DISCONNECT_BTN:
	{
		if (p_app->client.h_socket != INVALID_SOCKET)
		{
			DestroySocket(&p_app->client);
			AddEventString(p_app, "서버와의 연결을 끊었습니다.");
		}
	}
	break;
	// [전송] 버튼을 누른 경우
	case IDC_SEND_CHAT_BTN:
	{
		SendChatData(p_app);
	}
	break;
	// 채팅 내용을 입력하는 에디트 컨트롤
	case IDC_INPUT_CHAT_EDIT:
	{
		if (a_notify_code == ID_NOTIFY) SendChatData(p_app);
	}
	break;
	// 음악 재생 및 중지 버튼 컨트롤 누른 경우
	// 음악 재생 시 음악 반복 재생
	case IDC_PLAY_BTN:
	{
		sndPlaySound("background.wav", SND_LOOP | SND_ASYNC);
	}
	break;
	// 중지 버튼 클릭 시 음악 멈춤
	case IDC_STOP_BTN:
	{
		sndPlaySound(NULL, SND_ASYNC);
	}
	break;
	// 현재 실습 화면 캡처 버튼 컨트롤 누른 경우
	case IDC_SCREEN_CAP_BTN:
	{
		// 화면 캡처 버튼의 주소에 캡처된 이미지 저장
		p_app->p_screen_cap = CaptureScreenGP();
		// 저장된 이미지를 (11,370)위치에 0.31 * 0.15의 비율로 출력
		DrawImageGP(p_app->p_screen_cap, 11, 370, 0.31, 0.15);
		// 이미지를 screen.png 라는 이름으로 해당 실습 파일에 저장
		SaveImageGP(p_app->p_screen_cap, "screen.png");
		// 메모리에 적재되어 있는 이미지 데이터를 제거
		DeleteImageGP(p_app->p_screen_cap);
		// 정보를 윈도우에 출력
		ShowDisplay();
	}
	break;
	// 대화 삭제 버튼 클릭 시
	case IDC_DEL_CHAT_BTN:
	{
		ListBox_ResetContent(p_app->p_event_list);
	}
	break;
	default:
		break;
	}
}

// 컨트롤 조작하는 함수 등록
ON_MESSAGE(OnLeftBtnDown, NULL, NULL, OnCommand, OnDestroy, OnUserMsg)

// 이벤트 메시지를 추가하는 함수
void ClientAddEventString(NeoSocketData *ap_server, const char *ap_str)
{
	// 프로그램에 저장된 데이터를 얻음
	AD *p_app = (AD *)GetAppData();
	AddEventString(p_app, ap_str);
}

// 화면을 구성하는 요소(UI) 그리는 함수
void DrawScreenUI()
{
	// 창의 크기 조정
	ChangeWorkSize(WIN_WIDTH, WIN_HEIGHT);
	// 전체 색상을 RGB(82, 97, 124)로 채움
	Clear(0, WIN_COLOR);
	// 내부 프로그램에 저장될 메모리를 초기화
	AD p_data = { 0, };
	
	// 기본 값을 사용해서 클라이언트 소켓 서비스 초기화
	InitData(&p_data.client, ProcessingSocketMessage, ClientAddEventString, ClientSocketNotifyProc);
	// 서버와의 연결, 채팅 상태를 표시하는 리스트 박스 생성
	p_data.p_event_list = CreateListBox(5, 50, 650, 160, IDC_EVENT_LIST, NULL, 0);
	printf(10, 300, RGB(0, 0, 255), "원하는 항목을 선택하세요!!", 1);
	// 수평 방향으로 5개의 파란색 사각형을 차례로 나열함
	for (int i = 0; i < MAX_COUNT; ++i)
	{
		DrawItem(i, 0);
	}
	// 서버에 접속 버튼 생성
	p_data.p_con_btn = CreateButton("서버에 접속", 260, 17, 80, 26, IDC_CONNECT_BTN);
	// 접속 해제 버튼 생성
	p_data.p_discon_btn = CreateButton("접속 해제", 340, 17, 80, 26, IDC_DISCONNECT_BTN);
	// 채팅 삭제 버튼 생성
	p_data.p_del_chat = CreateButton("채팅 삭제", 420, 17, 80, 26, IDC_DEL_CHAT_BTN);
	// 음악 재생 및 중지 버튼 생성
	p_data.p_play_btn = CreateButton("음악 재생", 260, 240, 70, 26, IDC_PLAY_BTN);
	p_data.p_stop_btn = CreateButton("음악 중지", 330, 240, 70, 26, IDC_STOP_BTN);
	// 화면 캡처 버튼 생성
	p_data.p_screen_cap = CreateButton("화면 캡처", 400, 240, 70, 26, IDC_SCREEN_CAP_BTN);
	// 서버 ip, 아이디, 패스워드 입력할 에디트 컨트롤 생성
	// 서버 ip주소 칸에는 기본 주소가 에디트 컨트롤 안에 들어가 있음
	// 서버와 연결 시 서버에 저장되어 있는 ip주소를 입력해야 함
	p_data.p_server_ip_edit = CreateEdit(5, 23, 80, 20, IDC_SERVER_IP_EDIT, 0);
	SetCtrlName(p_data.p_server_ip_edit, "127.0.0.1");
	p_data.p_user_id_edit = CreateEdit(88, 23, 80, 20, IDC_USER_ID_EDIT, 0);
	p_data.p_user_pw_edit = CreateEdit(171, 23, 80, 20, IDC_USER_PW_EDIT, ES_PASSWORD);
	// 채팅 데이터를 입력할 에디트 컨트롤 생성
	p_data.p_in_chat_edit = CreateEdit(5, 212, 600, 20, IDC_INPUT_CHAT_EDIT, 0);
	// 엔터 키 누르면 notifyCode 1000번 실행
	EnableEnterKey(p_data.p_in_chat_edit);
	// 전송 버튼 생성
	p_data.p_send_btn = CreateButton("전송", 608, 212, 48, 20, IDC_SEND_CHAT_BTN);
	// 글꼴 및, 텍스트 색상 설정
	// 각 에디트 컨트롤 제목 출력
	SelectFontObject("맑은 고딕", 18, 0);
	SetTextColor(TEXT_COLOR);
	TextOut(9, 5, "서버 주소");
	TextOut(92, 5, "아이디");
	TextOut(175, 5, "비밀번호");

	p_data.h_key_hook_dll = ::LoadLibrary("TipsKeyHook.dll");
	if (p_data.h_key_hook_dll) { // DLL 로드 성공 여부를 체크한다.
		// 키보드 후킹 작업을 시작하는 함수의 주소를 얻는다.
		FP_KEY_HOOK_INSTALL fp = (FP_KEY_HOOK_INSTALL)GetProcAddress(p_data.h_key_hook_dll, "InstallKeyHook");
		// 키보드 후킹 작업을 시작한다. 체크된 키는 26001번 메시지로 이 프로그램에 전달된다.
		if (fp != NULL) (*fp)(gh_main_wnd, 26001, -1, NULL);
	}
	// 후킹 데이터를 표시하는 리스트 박스 생성
	// 리스트 박스의 높이는 18로 지정
	p_data.p_key_list = CreateListBox(7, 550, 600, 202, IDC_KEY_LIST);
	ListBox_SetItemHeight(p_data.p_key_list, 18);
	// image 폴더의 8개의 이미지를 메모리에 로드함
	// 프로그램 종료 시 자동으로 제거됨
	LoadImageAID(&p_data.walk_image, ".\\image\\walk_", 8);
	// 0.1초마다 이미지가 반복적으로 호출됨
	SetTimer(1, 100, RedrawTimer);
	// 프로그램 내부 데이터의 주소를 가져옴
	SetAppData(&p_data, sizeof(p_data));
	// 정보를 윈도우에 출력
	ShowDisplay();
}

// 프로그램 실행하는 메인 함수
int main()
{	
	DrawScreenUI();
	// 닫힘 버튼 클릭 시 강제 종료 
	return 0;
}

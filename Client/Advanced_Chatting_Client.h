#ifndef AD_CHAT_CLIENT_H
#define AD_CHAT_CLIENT_H

#include <time.h>						// _strtime_s �Լ� ���
#include <stdio.h>						// sprintf_s �Լ� ���
#include <shellapi.h>					// ShellExecute �Լ� ���
#define _USE_INIT_WINDOW_				// ������ ���� �Ӽ� �ʱ�ȭ �Լ� ���� ����
#include "tipsware.h"					// EasyWin32 ���
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")		// sndPlaySound �Լ� ���

#define MAX_COUNT	5					// ������ ���α׷� �ִ� ����
#define WIN_WIDTH	660					// Ŭ���̾�Ʈ ������ �ʺ� ����
#define WIN_HEIGHT	800					// Ŭ���̾�Ʈ ������ �� ����
#define WIN_COLOR	RGB(82, 97, 124)	// ��ü ������ RGB(82, 97, 124)�� ����
#define TEXT_COLOR	RGB(128, 202, 128)  // ����Ʈ �ڽ� ���� ���� ����

// ������ ���α׷��� ������ ���ڿ� 
const char *gp_name_table[MAX_COUNT] = { "�޸���", "�׸���", "Ž����", "����", "ũ��" };
// ������ ���α׷�(Ȯ���� ����)
const char *gp_cmd_table[MAX_COUNT] = { "notepad.exe", "mspaint.exe", "explorer.exe", "msedge.exe", "chrome.exe" };

// ��Ʈ�� Ÿ�Կ� ���� ���̵� ����ü
enum CtrlTypeId
{
	IDC_CONNECT_BTN = 1000,		// ������ ���� ��ư�� ���̵�
	ID_NOTIFY = 1000,			// ��Ʈ�� ���� ������ ���̵�
	IDC_DISCONNECT_BTN,			// ���� ���� ��ư�� ���̵�
	IDC_SEND_CHAT_BTN,			// ä���� �����ϴ� ��ư�� ���̵�
	IDC_DEL_CHAT_BTN,			// ä�� ������ �ʱ�ȭ �ϴ� ��ư�� ���̵�
	IDC_PLAY_BTN,				// ���� ��� ��ư�� ���̵�
	IDC_STOP_BTN,				// ���� ���� ��ư�� ���̵�
	IDC_SCREEN_CAP_BTN,			// ȭ�� ĸó ��ư�� ���̵�
	IDC_EVENT_LIST = 2001,		// ���� ����� �����ϴ� ����Ʈ �ڽ��� ���̵�
	IDC_KEY_LIST,				// ��ŷ �����͸� ǥ���ϴ� ����Ʈ �ڽ��� ���̵�
	IDC_SERVER_IP_EDIT = 3000,	// ���� �ּ� �Է¿� ����Ʈ ��Ʈ���� ���̵�
	IDC_USER_ID_EDIT,			// ���̵� �Է¿� ����Ʈ ��Ʈ���� ���̵�
	IDC_USER_PW_EDIT,			// �н����� �Է¿� ����Ʈ ��Ʈ���� ���̵�
	IDC_INPUT_CHAT_EDIT			// ä�� �Է¿� ����Ʈ ��Ʈ���� ���̵�
};

// ä�� ������ ������ ������ ���� ����ü
enum ChatDataDeliveryType
{
	REQ_USER_CHAT = 21,			// C -> S ä�� ������ ����
	ANS_USER_CHAT				// S -> C ä�� ������ ����(��ε�ĳ����)
};

// ���α׷����� ����� ���� ������
typedef struct ApplicationData
{
	NeoSocketData client;		// ���� ���� ���񽺸� ���� �ʿ��� ����
	HINSTANCE h_key_hook_dll;	// Ű���� ��ŷ ���� DLL �ڵ�
	TW_AID walk_image;			// png �̹����� ��ġ, ����, ����� ������ ����ü
	void *p_event_list;			// ���α׷� ���� ����� �����ϴ� ����Ʈ �ڽ� �ּ�
	void *p_con_btn;			// ���� ���� ��ư �ּ�
	void *p_discon_btn;			// ���� ���� ���� ��ư �ּ�
	void *p_send_btn;			// ä�� ���� ��ư �ּ�
	void *p_server_ip_edit;		// ����� ip �Է��ϴ� ����Ʈ ��Ʈ�� �ּ�
	void *p_user_id_edit;		// ����� id �Է��ϴ� ����Ʈ ��Ʈ�� �ּ�
	void *p_user_pw_edit;		// ����� pw �Է��ϴ� ����Ʈ ��Ʈ�� �ּ�
	void *p_in_chat_edit;		// ����ڰ� ä�� ������ �Է��ϴ� ����Ʈ ��Ʈ�� �ּ�
	void *p_play_btn;			// ���� ��� ��ư�� �ּ�
	void *p_stop_btn;			// ���� ���� ��ư�� �ּ�
	void *p_del_chat;			// ä�� ���� ��ư�� �ּ�
	void *p_screen_cap;			// ȭ�� ĸó ��ư�� �ּ�
	void *p_key_list;			// �Էµ� Ű ����� ������ ����Ʈ �ڽ� �ּ�
}AD;

// ��ŷ ��ƾ ��ġ�� ���� ��ġ �Լ�
typedef void (*FP_KEY_HOOK_INSTALL)(HWND ah_target_wnd, int a_msg_id, int a_filter_count, UINT8 a_filter_list[]);


#endif // !AD_CHAT_CLIENT



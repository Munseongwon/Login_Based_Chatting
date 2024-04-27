#ifndef AD_CHAT_SERVER_H
#define AD_CHAT_SERVER_H

#include <stdlib.h>
#define _USE_INIT_WINDOW_  // ������ ���� �ʱ�ȭ �Լ��� ���� �����ϵ��� ���� (InitWindow)
#include <stdio.h> // sprintf_s, fopen_s, fclose �Լ��� ����ϱ� ����
#include "tipsware.h"

enum TpyeCtrlId
{
    IDC_SERVICE_START_BTN = 1000,   // ���� ���� ��ư�� ���̵�
    IDC_SERVICE_STOP_BTN,           // ���� ���� ��ư�� ���̵�
    IDC_MODIFY_USER_BTN = 1100,     // ����� ���� ���� ��ư�� ���̵�
    IDC_DEL_USER_BTN,               // ����� ���� ���� ������ ���̵�
    IDC_DEL_CHAT_BTN,               // ä��â ���� ���� ����
    IDC_USER_LIST  = 2000,          // ����� ����� �����ϴ� ����Ʈ �ڽ��� ���̵�
    IDC_EVENT_LIST,                 // ���� ����� �����ϴ� ����Ʈ �ڽ��� ���̵�
    IDC_IP_COMBO = 3000,            // ���� ���� ����� �����ϴ� �޺� �ڽ��� ���̵�
    IDC_USER_LEVEL_COMBO,           // ����� ��� ����� �����ϴ� �޺� �ڽ��� ���̵�
    IDC_USER_ID_EDIT = 4000,        // ���̵� �Է¿� ����Ʈ ��Ʈ���� ���̵�
    IDC_USER_PASSWORD_EDIT,         // ��ȣ �Է¿� ����Ʈ ��Ʈ���� ���̵�
    IDC_USER_NAME_EDIT,             // �̸� �Է¿� ����Ʈ ��Ʈ���� ���̵�
    IDC_USER_ETC_EDIT,              // ��Ÿ ���� �Է¿� ����Ʈ ��Ʈ���� ���̵�
};

enum DataType
{
    REQ_USER_CHAT,    // C -> S ä�� ������ ����
    ANS_USER_CHAT     // S -> C ä�� ������ ���� (��ε� ĳ����)
};


// �� ������ ����� ȸ�� ��� ��� (9�ܰ�)
const char *gp_level_string[] = { "�մ�", "�ӽ�ȸ��", "�Ϲ�ȸ��", "����ȸ��", "���ȸ��", "����ü", "���", "VIP", "������" };
int g_level_len[] = { 4, 8, 8, 8, 8, 6, 6, 3, 6 };

typedef struct ApplicationData  // ���α׷����� ����� ���� ������
{
    NeoServerData server_data;  // ���� ���� ���񽺸� ���� �ʿ��� ����
    void *p_user_list;          // ����� ����� �����ϴ� ����Ʈ �ڽ�
    void *p_event_list;         // ���α׷� ���� ����� �����ϴ� ����Ʈ �ڽ�
} AD;


#endif // !AD_CHAT_SERVER_H


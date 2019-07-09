#include "stdafx.h"
#include "ConsolMain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CConsolMain::CConsolMain()
	: m_pSqlDB(NULL)
{
	m_pSqlDB = new CSqllitDB();
}

CConsolMain::~CConsolMain()
{
	if (m_pSqlDB)
	{
		delete m_pSqlDB;
		m_pSqlDB = NULL;
	}
}

int CConsolMain::Process()
{
	// �������˵�
	CreateMainMenu();
	printf("������˵�ѡ�");
	char menu[256] = { 0 };
	char* pMenu = gets_s(menu, sizeof(menu) - 1);
	
	int ncommend = atoi(pMenu);

	switch (ncommend)
	{
	case 1:
		CreateDB();
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		return 0;
	default:
	{
		printf("����������������룡\r\n");
		system("pause");
	}
	break;
	}
	system("cls");
	return 1;
}

void CConsolMain::CreateMainMenu()
{
	const char *pStrMenu = "******************************************************\r\n*      1���������ݿ�              2����ѯ���ݿ�      *\r\n*      3���޸����ݿ�              4��ɾ�����ݿ�      *\r\n*      5���˳�                                       *\r\n******************************************************\r\n";
	printf(pStrMenu);
}

void CConsolMain::CreateDB()
{
	while (true)
	{
		system("cls");
		printf("0����\r\n��������Ҫ�����ı�����");
		char strTableName[256] = { 0 };
		char* pstrTableName = gets_s(strTableName, sizeof(strTableName) - 1);
		if (0 == strcmp(pstrTableName, "0"))
		{
			return;
		}
		if (0 == strcmp(pstrTableName, "") || pstrTableName == NULL)
		{
			continue;
		}

		// �������ݿ�Ϊ���̰߳�ȫģʽ������һ�����ӽ���һ���߳�ʹ��ʱ�ǰ�ȫ��
		if (m_pSqlDB->ConfigDB(SQLITE_CONFIG_MULTITHREAD))
		{
			// �����ݿ��û���򴴽� 
			if (m_pSqlDB->OpenSqllitDB(pstrTableName))
			{
				printf("�����ɹ���\r\n");
				system("pause");
				return;
			}
		}

	}
}

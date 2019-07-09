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
	// 创建主菜单
	CreateMainMenu();
	printf("请输入菜单选项：");
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
		printf("命令出错，请重新输入！\r\n");
		system("pause");
	}
	break;
	}
	system("cls");
	return 1;
}

void CConsolMain::CreateMainMenu()
{
	const char *pStrMenu = "******************************************************\r\n*      1、创建数据库              2、查询数据库      *\r\n*      3、修改数据库              4、删除数据库      *\r\n*      5、退出                                       *\r\n******************************************************\r\n";
	printf(pStrMenu);
}

void CConsolMain::CreateDB()
{
	while (true)
	{
		system("cls");
		printf("0返回\r\n请输入您要创建的表名：");
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

		// 配置数据库为多线程安全模式，仅在一个连接仅供一个线程使用时是安全的
		if (m_pSqlDB->ConfigDB(SQLITE_CONFIG_MULTITHREAD))
		{
			// 打开数据库表，没有则创建 
			if (m_pSqlDB->OpenSqllitDB(pstrTableName))
			{
				printf("创建成功！\r\n");
				system("pause");
				return;
			}
		}

	}
}

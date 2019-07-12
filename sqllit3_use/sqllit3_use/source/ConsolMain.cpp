#include "stdafx.h"
#include "ConsolMain.h"
#include <stdio.h>
#include <stdlib.h>


CConsolMain::CConsolMain()
	: m_pSqlDB(NULL)
{
	m_pSqlDB = new CSqllitDB();
	m_srCurrOpenDBName[MAX_PATH] = {0};
	m_pDBList[MAX_PATH] = { 0 };
	InitDBPath();
}

CConsolMain::~CConsolMain()
{
	if (m_pSqlDB)
	{
		m_pSqlDB->CloseSqllitDB();
		delete m_pSqlDB;
		m_pSqlDB = NULL;
	}

	CleanDBList();
}

int CConsolMain::StartProcess()
{
	// 创建主菜单
	CreateMainMenu();
	if (strcmp(m_srCurrOpenDBName, "") != 0)
	{
		printf("当前可操作数据库名：%s\r\n", m_srCurrOpenDBName);
	}
	printf("请输入菜单选项：");
	char menu[256] = { 0 };
	char* pMenu = gets_s(menu, sizeof(menu));

	int ncommend = atoi(pMenu);

	switch (ncommend)
	{
	case 1:
		CreateDB();
		break;
	case 2:
		SeeAllDB();
		break;
	case 3:
		while (ModifySqlDB());
		break;
	case 4:
		break;
	case 5:
		SeeAllInfo();
		break;
	case 6:
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

void CConsolMain::InitDBPath()
{
	char szCurrentDir[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szCurrentDir, sizeof(szCurrentDir));
	PathRemoveFileSpecA(szCurrentDir);
	m_strDBPath = szCurrentDir;
	m_strDBPath += "\\SqlDB\\";
}

void CConsolMain::UpdateDBList()
{
	long hFile;
	_finddata_t fileinfo;

	if ((hFile = _findfirst((m_strDBPath+"*.*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			int i = 0;
			if (!(fileinfo.attrib & _A_SUBDIR))//找到文件
			{
				char* pfilename = new char[_MAX_PATH];
				strcpy(pfilename, fileinfo.name);
				m_pDBList[i] = pfilename;
				i++;
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void CConsolMain::CleanDBList()
{
	for (int i = 0; i < MAX_PATH; i++)
	{
		if (m_pDBList[i] != NULL)
		{
			delete m_pDBList[i];
			m_pDBList[i] = NULL;
		}
	}
}

void CConsolMain::CreateMainMenu()
{
	const char *pStrMenu = "******************************************************\r\n*      1、打开或创建数据库        2、查看数据库      *\r\n*      3、修改数据库              4、删除数据库      *\r\n*      5、查看数据库表            6、退出            *\r\n******************************************************\r\n";
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

		strcpy_s(m_srCurrOpenDBName, pstrTableName);

		// 配置数据库为多线程安全模式，仅在一个连接仅供一个线程使用时是安全的
		if (m_pSqlDB->ConfigDB(SQLITE_CONFIG_MULTITHREAD))
		{
			// 打开数据库表，没有则创建 
			if (m_pSqlDB->OpenSqllitDB((m_strDBPath + pstrTableName+".db").c_str()))
			{
				printf("创建成功！\r\n");
			}
			else
			{
				printf("创建失败！\r\n");
			}
			system("pause");
			return;
		}
	}
}

void CConsolMain::SeeAllDB()
{
	UpdateDBList();
	system("cls");
	printf("当前数据库列表如下：\r\n");
	for (int i = 0; m_pDBList[i] != NULL && i < MAX_PATH; i++)
    {
		printf("\t%d、%s\r\n", i+1,m_pDBList[i]);
    }
	system("pause");
}

int CConsolMain::ModifySqlDB()
{
	system("cls");
	if (strcmp(m_srCurrOpenDBName, "") == 0)
	{
		printf("当前无数据库可操作，请创建或打开数据库\r\n");
		system("pause");
		return 0;
	}
	printf("1、创建表\r\n2、添加\r\n3、修改\r\n4、删除\r\n5、退出\r\n\r\n请选择：");

	char menu[256] = { 0 };
	char* pMenu = gets_s(menu, sizeof(menu) - 1);

	int ncommend = atoi(pMenu);
	switch (ncommend)
	{
	case 1:
		CreateDBTable();
		break;
	case 2:
		AddItemToDBTable();
		break;
	case 3:
		UpdateItemToDBTable();
		break;
	case 4:
		DelItemToDBTable();
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
	return 1;
}

void CConsolMain::CreateDBTable()
{
	printf("输入需要创建的表名：");
	char TableName[256] = { 0 };
	char* pTableName = gets_s(TableName, sizeof(TableName) - 1);

	char* sql = sqlite3_mprintf("CREATE TABLE IF NOT EXISTS %s(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL, AGE INT NOT NULL, INFO TEXT)", pTableName);
	if (!m_pSqlDB->ExecDBSql(sql, NULL, NULL, NULL))
	{
		printf("该表已存在！请重新创建或退出！\r\n");
	}
	else
	{
		printf("表创建成功！\r\n");
	}
	sqlite3_free(sql);
	system("pause");
}

void CConsolMain::AddItemToDBTable()
{
	printf("请输入要插入人员的ID 、 姓名 、 年龄、 信息描述\r\n");
	printf("例如: 1 老王 29 这是一个地球人\r\n");
	printf("请输入:");
	int nId = -1;
	char pName[30] = { 0 };
	int nage = -1;
	char pInfo[300] = { 0 };
	std::string strInfo = "";
	char pStrInfo[1024] = { 0 };
	gets_s(pStrInfo);
	strInfo.append(pStrInfo);
	if (strInfo == "")
	{
		printf("输入无效，请重新输入！");
		system("pause");
		return;
	}
	int posStart = 0;
	int posEnd = strInfo.find(' ', 0);
	nId = atoi(strInfo.substr(posStart, posEnd - posStart).c_str());
	posStart = posEnd+1;
	posEnd = strInfo.find(' ', posStart);
	strcpy_s(pName, strInfo.substr(posStart, posEnd - posStart).c_str());
	posStart = posEnd+1;
	posEnd = strInfo.find(' ', posStart);
	nage = atoi(strInfo.substr(posStart, posEnd - posStart).c_str());
	posStart = posEnd+1;
	posEnd = strInfo.find(' ', posStart);
	strcpy_s(pInfo, strInfo.substr(posStart, posEnd - posStart).c_str());

	char* pSql = sqlite3_mprintf("INSERT INTO %s (ID, NAME, AGE, INFO) VALUES (%d, '%s', %d, '%s')", m_srCurrOpenDBName, nId, pName, nage, pInfo);
	if (m_pSqlDB->ExecDBSql(pSql, NULL, NULL, NULL))
	{
		printf("数据录入成功！");
	}
	else
	{
		printf("数据录入失败！");
	}
	system("pause");
	sqlite3_free(pSql);
}

void CConsolMain::UpdateItemToDBTable()
{

}

void CConsolMain::DelItemToDBTable()
{
	printf("请输入要删除人员的ID\r\n");
	printf("例如: 1\r\n");

	char* pSql = sqlite3_mprintf("DELETE FROM %s WHERE ID = ?", m_srCurrOpenDBName);
	if (m_pSqlDB->PrepareDBSql(pSql, strlen(pSql), NULL))
	{
		while (true)
		{
			printf("请输入需要删除的人员ID:");
			char pStrInfo[256] = { 0 };
			gets_s(pStrInfo);
			int id = ::atoi(pStrInfo);
			if (strcmp(pStrInfo, "") == 0 || id == 0 || id == -1)
			{
				printf("输入无效，请重新输入！");
				system("pause");
				continue;
			}

			m_pSqlDB->BindIntSqliteDB(1, id);
			if (m_pSqlDB->SqliteDBStep())
			{
				printf("删除ID为%s人员信息成功\r\n", pStrInfo);
			}
			else
			{
				printf("删除ID为%s人员信息失败\r\n", pStrInfo);
			}
			m_pSqlDB->PrepareDBReSet();

			printf("继续删除请按y,其他键退出\r\n", pStrInfo);
			if (getchar() == 'y')
			{
				m_pSqlDB->PrepareDBReSet();
				getchar();
				continue;
			}
			else
			{
				m_pSqlDB->FinalizePrepareSqliteDB();
				break;
			}
		}
	}
	sqlite3_free(pSql);
}

void CConsolMain::SeeAllInfo()
{
	char* pSql = sqlite3_mprintf("SELECT * FROM %s", m_srCurrOpenDBName);
	if (!m_pSqlDB->ExecDBSql(pSql, DBCallBack, NULL, NULL))
	{
		printf("查看失败！\r\n");
	}
	sqlite3_free(pSql);
	system("pause");
}

int CConsolMain::DBCallBack(void*callbackdata, int ncols, char** values, char** headers)
{
	for (int i = 0; i < ncols; i++ )
	{
		printf("%s\t%s\t", headers[i], values[i]);
	}
	printf("\r\n");
	return SQLITE_OK;
}


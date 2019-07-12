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
	// �������˵�
	CreateMainMenu();
	if (strcmp(m_srCurrOpenDBName, "") != 0)
	{
		printf("��ǰ�ɲ������ݿ�����%s\r\n", m_srCurrOpenDBName);
	}
	printf("������˵�ѡ�");
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
		printf("����������������룡\r\n");
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
			if (!(fileinfo.attrib & _A_SUBDIR))//�ҵ��ļ�
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
	const char *pStrMenu = "******************************************************\r\n*      1���򿪻򴴽����ݿ�        2���鿴���ݿ�      *\r\n*      3���޸����ݿ�              4��ɾ�����ݿ�      *\r\n*      5���鿴���ݿ��            6���˳�            *\r\n******************************************************\r\n";
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

		strcpy_s(m_srCurrOpenDBName, pstrTableName);

		// �������ݿ�Ϊ���̰߳�ȫģʽ������һ�����ӽ���һ���߳�ʹ��ʱ�ǰ�ȫ��
		if (m_pSqlDB->ConfigDB(SQLITE_CONFIG_MULTITHREAD))
		{
			// �����ݿ��û���򴴽� 
			if (m_pSqlDB->OpenSqllitDB((m_strDBPath + pstrTableName+".db").c_str()))
			{
				printf("�����ɹ���\r\n");
			}
			else
			{
				printf("����ʧ�ܣ�\r\n");
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
	printf("��ǰ���ݿ��б����£�\r\n");
	for (int i = 0; m_pDBList[i] != NULL && i < MAX_PATH; i++)
    {
		printf("\t%d��%s\r\n", i+1,m_pDBList[i]);
    }
	system("pause");
}

int CConsolMain::ModifySqlDB()
{
	system("cls");
	if (strcmp(m_srCurrOpenDBName, "") == 0)
	{
		printf("��ǰ�����ݿ�ɲ������봴��������ݿ�\r\n");
		system("pause");
		return 0;
	}
	printf("1��������\r\n2�����\r\n3���޸�\r\n4��ɾ��\r\n5���˳�\r\n\r\n��ѡ��");

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
		printf("����������������룡\r\n");
		system("pause");
	}
		break;
	}
	return 1;
}

void CConsolMain::CreateDBTable()
{
	printf("������Ҫ�����ı�����");
	char TableName[256] = { 0 };
	char* pTableName = gets_s(TableName, sizeof(TableName) - 1);

	char* sql = sqlite3_mprintf("CREATE TABLE IF NOT EXISTS %s(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL, AGE INT NOT NULL, INFO TEXT)", pTableName);
	if (!m_pSqlDB->ExecDBSql(sql, NULL, NULL, NULL))
	{
		printf("�ñ��Ѵ��ڣ������´������˳���\r\n");
	}
	else
	{
		printf("�����ɹ���\r\n");
	}
	sqlite3_free(sql);
	system("pause");
}

void CConsolMain::AddItemToDBTable()
{
	printf("������Ҫ������Ա��ID �� ���� �� ���䡢 ��Ϣ����\r\n");
	printf("����: 1 ���� 29 ����һ��������\r\n");
	printf("������:");
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
		printf("������Ч�����������룡");
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
		printf("����¼��ɹ���");
	}
	else
	{
		printf("����¼��ʧ�ܣ�");
	}
	system("pause");
	sqlite3_free(pSql);
}

void CConsolMain::UpdateItemToDBTable()
{

}

void CConsolMain::DelItemToDBTable()
{
	printf("������Ҫɾ����Ա��ID\r\n");
	printf("����: 1\r\n");

	char* pSql = sqlite3_mprintf("DELETE FROM %s WHERE ID = ?", m_srCurrOpenDBName);
	if (m_pSqlDB->PrepareDBSql(pSql, strlen(pSql), NULL))
	{
		while (true)
		{
			printf("��������Ҫɾ������ԱID:");
			char pStrInfo[256] = { 0 };
			gets_s(pStrInfo);
			int id = ::atoi(pStrInfo);
			if (strcmp(pStrInfo, "") == 0 || id == 0 || id == -1)
			{
				printf("������Ч�����������룡");
				system("pause");
				continue;
			}

			m_pSqlDB->BindIntSqliteDB(1, id);
			if (m_pSqlDB->SqliteDBStep())
			{
				printf("ɾ��IDΪ%s��Ա��Ϣ�ɹ�\r\n", pStrInfo);
			}
			else
			{
				printf("ɾ��IDΪ%s��Ա��Ϣʧ��\r\n", pStrInfo);
			}
			m_pSqlDB->PrepareDBReSet();

			printf("����ɾ���밴y,�������˳�\r\n", pStrInfo);
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
		printf("�鿴ʧ�ܣ�\r\n");
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


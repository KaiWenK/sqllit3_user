#include "stdafx.h"
#include "SqllitDB.h"

CSqllitDB::CSqllitDB()
	: m_pSqliteDB(NULL)
	, m_pStmt(NULL)
{

}

CSqllitDB::~CSqllitDB()
{

}

bool CSqllitDB::ConfigDB(int config)
{
	// SQLITE_CONFIG_SINGLETHREAD���̰߳�ȫ
	// SQLITE_CONFIG_MULTITHREAD ���̰߳�ȫ��ֻҪһ������ֻ��һ���߳�ʹ�þ��ǰ�ȫ��
	// SQLITE_CONFIG_SERIALIZED ���У����̰߳�ȫ
	if (SQLITE_OK == sqlite3_config(config))
	{
		return true;
	}
	return false;
}

int CSqllitDB::GetDBThreadMode()
{
	return sqlite3_threadsafe();
}

bool CSqllitDB::SetDBSharedCash(int bShared)
{
	if (SQLITE_OK == sqlite3_enable_shared_cache(bShared))
	{
		return true;
	}
	return false;
}

bool CSqllitDB::OpenSqllitDB(const char* filename)
{
	// ���µ����ݿ�õ�m_pSqliteDB���
	if (SQLITE_OK == sqlite3_open(filename, &m_pSqliteDB))
	{
		return true;
	}
	return false;
}

bool CSqllitDB::CloseSqllitDB()
{
	if (!m_pSqliteDB)
	{
		return false;
	}
	if (SQLITE_OK == sqlite3_close(m_pSqliteDB))
	{
		m_pSqliteDB = NULL;
		return true;
	}
	return false;
}

bool CSqllitDB::ExecDBSql(const char* sql, SqliteCallBackFun callfun, void* pData, char** msg)
{
	if (!m_pSqliteDB)
	{
		return false;
	}
	if (SQLITE_OK == sqlite3_exec(m_pSqliteDB, sql, callfun, pData, msg))
	{
		return true;
	}
	return false;
}

bool CSqllitDB::PrepareDBSql(const char* strSql, int nByte, const char **pzTail)
{
	if (!m_pSqliteDB)
	{
		return false;
	}
	if (SQLITE_OK == sqlite3_prepare_v2(m_pSqliteDB, strSql, nByte, &m_pStmt, pzTail))
	{
		return true;
	}
	return false;
}

bool CSqllitDB::SqliteDBStep()
{
	if (!m_pStmt || !m_pSqliteDB)
	{
		return false;
	}
	if (SQLITE_OK == sqlite3_step(m_pStmt))
	{
		return true;
	}
	return false;
}

bool CSqllitDB::FinalizePrepareSqliteDB()
{
	if (!m_pStmt || !m_pSqliteDB)
	{
		return false;
	}
	if (SQLITE_OK == sqlite3_finalize(m_pStmt))
	{
		return true;
	}
	return false;
}

/*===========================================================================================================
** ���� ��ִ��һ���Բ�ѯ��䣬�õ���ṹ�������
** ����1��const char *zSql һ�����ڲ����SQL��䣬����SELECT
** ����2��char ***pazResult ��ѯ���
** ����3��int *pnRow д��Ľ������
** ����4��int *pnColumn д��Ľ������
** ����5��char **pzErrmsg д��Ĵ�����Ϣ
**===========================================================================================================*/
bool CSqllitDB::GetTableSqlDB(const char *zSql, char ***pazResult, int *pnRow, int *pnColumn, char **pzErrmsg)
{
	if (!m_pSqliteDB)
	{
		return false;
	}
	if (SQLITE_OK == sqlite3_get_table(m_pSqliteDB, zSql, pazResult, pnRow, pnColumn, pzErrmsg))
	{
		return true;
	}
	return false;
}

void CSqllitDB::FreeSqlTableDB(char **result)
{
	if (!m_pSqliteDB)
	{
		return;
	}
	sqlite3_free_table(result);
}

const unsigned char* CSqllitDB::GetColTextSqliteDB(int iCol)
{
	if (!m_pStmt || !m_pSqliteDB)
	{
		return NULL;
	}
	return sqlite3_column_text(m_pStmt, iCol);
}

double CSqllitDB::GetColDoubleSqliteDB(int iCol)
{
	if (!m_pStmt || !m_pSqliteDB)
	{
		return 0.0;
	}
	return sqlite3_column_double(m_pStmt, iCol);
}

int CSqllitDB::GetColNumSqliteDB(sqlite3_stmt* pStmt)
{
	if (!m_pSqliteDB || !m_pStmt)
	{
		return -1;
	}
	return sqlite3_column_count(pStmt);
}
/*====================================================================
** ���� ����ֵ��Ԥ���������
** ����1��sqlte_stmt* Ԥ����sql����ָ��
** ����2����?������������1��ʼ
** ����3�����������ݵ���ʼָ��
** ����4�����������ݵĳ���
** ����5�������ص�������һ��Ĭ��Ϊ��
*====================================================================*/
int CSqllitDB::BindBlobSqliteDB(int i, const void*p, int n)
{
	if (!m_pStmt || !m_pSqliteDB)
	{
		return -1;
	}
	return sqlite3_bind_blob(m_pStmt, i, p, n, NULL);
}

const void* CSqllitDB::GetBlobSqliteDB(int iCol)
{
	if (!m_pStmt || !m_pSqliteDB)
	{
		return NULL;
	}
	return sqlite3_column_blob(m_pStmt, iCol);
}

int CSqllitDB::GetBlobColBytesSqliteDB(int iCol)
{
	if (!m_pStmt || !m_pSqliteDB)
	{
		return NULL;
	}
	return sqlite3_column_bytes(m_pStmt, iCol);
}

int CSqllitDB::GetBlobColIntSqliteDB(int iCol)
{
	if (!m_pStmt || !m_pSqliteDB)
	{
		return NULL;
	}
	return sqlite3_column_int(m_pStmt, iCol);
}

__int64 CSqllitDB::GetBlobColInt64SqliteDB(int iCol)
{
	if (!m_pStmt || !m_pSqliteDB)
	{
		return NULL;
	}
	return sqlite3_column_int64(m_pStmt, iCol);
}

const char* CSqllitDB::GetLastSqlErrorMsg()
{
	if (!m_pSqliteDB)
	{
		return NULL;
	}
	return sqlite3_errmsg(m_pSqliteDB);
}

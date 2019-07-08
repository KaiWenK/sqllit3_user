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
	// SQLITE_CONFIG_SINGLETHREAD单线程安全
	// SQLITE_CONFIG_MULTITHREAD 多线程安全，只要一个连接只有一个线程使用就是安全的
	// SQLITE_CONFIG_SERIALIZED 串行，多线程安全
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
	// 打开新的数据库得到m_pSqliteDB句柄
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
** 功能 ：执行一般性查询语句，得到表结构相关数据
** 参数1：const char *zSql 一般用于插叙的SQL语句，比如SELECT
** 参数2：char ***pazResult 查询结果
** 参数3：int *pnRow 写入的结果行数
** 参数4：int *pnColumn 写入的结果列数
** 参数5：char **pzErrmsg 写入的错误消息
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

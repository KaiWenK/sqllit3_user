#ifndef SQLLITDB_H
#define SQLLITDB_H

// 执行数据库回调
typedef int(*SqliteCallBackFun)(void*callbackdata, int ncols, char** values, char** headers);

class CSqllitDB
{
public:
	CSqllitDB();
	virtual ~CSqllitDB();

public:
	bool ConfigDB(int config);
	int  GetDBThreadMode();                    // 返回sql库在编译时指定的线程安全模式
	bool SetDBSharedCash(int bShared = 1);     // 设置是否共享缓存
	bool OpenSqllitDB(const char* filename);   // 打开数据库
	bool CloseSqllitDB();                      // 关闭数据库
	bool ExecDBSql(const char* sql, SqliteCallBackFun callfun, void* pData, char** msg);  // 执行数据库语句
	bool PrepareDBSql(const char* strSql, int nByte, const char **pzTail);  // 准备SQL语句
	bool SqliteDBStep();                      // 这个过程用于执行有前面PrepareDBSql创建的准备语句。这个语句执行到结果的第一行可用的位置
	bool GetTableSqlDB(const char *zSql, char ***pazResult, int *pnRow, int *pnColumn, char **pzErrmsg);    // 非回调的select 查询
	void FreeSqlTableDB(char **result);       // 释放GetTableSqlDB得到的结果集
private:
	sqlite3*            m_pSqliteDB;   // 打开数据库文件时得到的连接对象指针
	sqlite3_stmt*		m_pStmt;       // 表示一条被编译过的语句对象指针
};



#endif // SQLLITDB_H
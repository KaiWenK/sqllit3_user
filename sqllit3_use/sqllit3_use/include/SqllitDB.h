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
	int  GetDBThreadMode();                                     // 返回sql库在编译时指定的线程安全模式
	bool SetDBSharedCash(int bShared = 1);                      // 设置是否共享缓存
	bool OpenSqllitDB(const char* filename);                    // 打开数据库
	bool CloseSqllitDB();                                       // 关闭数据库

	bool ExecDBSql(const char* sql,
		           SqliteCallBackFun callfun,
				   void* pData,
				   char** msg );                               // 执行数据库语句

	bool PrepareDBSql(const char* strSql,
		              int nByte,
					  const char **pzTail );                  // 准备SQL语句
	bool SqliteDBStep();                                      // 这个过程用于执行有前面PrepareDBSql创建的准备语句。这个语句执行到结果的第一行可用的位置
	bool FinalizePrepareSqliteDB();                           // 这个过程销毁前面被PrepareDBSql创建的准备语句，每个准备语句都必须使用这个函数去销毁以防止内存泄露
	
	bool GetTableSqlDB(const char *zSql,
		               char ***pazResult, 
					   int *pnRow, 
					   int *pnColumn, 
					   char **pzErrmsg);                      // 非回调的select 查询

	void FreeSqlTableDB(char **result);                       // 释放GetTableSqlDB得到的结果集
	const unsigned char* GetColTextSqliteDB(int iCol);        // 执行sqlite3_step()执行一个准备语句得到的结果集的当前行中返回一个列
	double GetColDoubleSqliteDB(int iCol);                    // 同GetColTextSqliteDB
	int GetColNumSqliteDB(sqlite3_stmt* pStmt);               // 获得结果集中的列数
	int BindBlobSqliteDB(int i, const void*p, int n);         // 用以给预编译的语句插入值（语句中带？，下标从1开始）
	const void* GetBlobSqliteDB(int iCol);                    // 从执行sqlite3_step()执行一个准备语句得到的结果集的当前行中返回一个列
	int GetBlobColBytesSqliteDB(int iCol);                    // 从执行sqlite3_step()执行一个准备语句得到的结果集的当前行中返回一个列
	int GetBlobColIntSqliteDB(int iCol);
	__int64 GetBlobColInt64SqliteDB(int iCol);
	const char* GetLastSqlErrorMsg();                         // 返回最近一次错误描述，返回值UTF-8编码的英文字符串

private:
	sqlite3*            m_pSqliteDB;   // 打开数据库文件时得到的连接对象指针
	sqlite3_stmt*		m_pStmt;       // 表示一条被编译过的语句对象指针
};

// 注意：使用sqlite3_mprintf格式化sql语句时记得调用sqlite3_free

#endif // SQLLITDB_H
#ifndef SQLLITDB_H
#define SQLLITDB_H

// ִ�����ݿ�ص�
typedef int(*SqliteCallBackFun)(void*callbackdata, int ncols, char** values, char** headers);

class CSqllitDB
{
public:
	CSqllitDB();
	virtual ~CSqllitDB();

public:
	bool ConfigDB(int config);
	int  GetDBThreadMode();                    // ����sql���ڱ���ʱָ�����̰߳�ȫģʽ
	bool SetDBSharedCash(int bShared = 1);     // �����Ƿ�����
	bool OpenSqllitDB(const char* filename);   // �����ݿ�
	bool CloseSqllitDB();                      // �ر����ݿ�
	bool ExecDBSql(const char* sql, SqliteCallBackFun callfun, void* pData, char** msg);  // ִ�����ݿ����
	bool PrepareDBSql(const char* strSql, int nByte, const char **pzTail);  // ׼��SQL���
	bool SqliteDBStep();                      // �����������ִ����ǰ��PrepareDBSql������׼����䡣������ִ�е�����ĵ�һ�п��õ�λ��
	bool GetTableSqlDB(const char *zSql, char ***pazResult, int *pnRow, int *pnColumn, char **pzErrmsg);    // �ǻص���select ��ѯ
	void FreeSqlTableDB(char **result);       // �ͷ�GetTableSqlDB�õ��Ľ����
private:
	sqlite3*            m_pSqliteDB;   // �����ݿ��ļ�ʱ�õ������Ӷ���ָ��
	sqlite3_stmt*		m_pStmt;       // ��ʾһ�����������������ָ��
};



#endif // SQLLITDB_H
#ifndef CONSOLMAIN_H
#define CONSOLMAIN_H

class CConsolMain
{
public:
	CConsolMain();
	virtual ~CConsolMain();

public:
	int StartProcess();
private:
	void InitDBPath();          // 初始化数据库路径
	void UpdateDBList();        // 更新数据库列表
	void CleanDBList();         // 删除数据库列表 
	void CreateMainMenu();      // 创建主菜单
	void CreateDB();            // 创建数据库
	void SeeAllDB();            // 查看所有数据库

	int ModifySqlDB();          // 修改数据库
	void CreateDBTable();       // 创建数据库表
	void AddItemToDBTable();    // 添加数据
	void UpdateItemToDBTable(); // 添加数据
	void DelItemToDBTable();    // 删除数据
	void SeeAllInfo();          // 查看当前表的所有成员

	static int DBCallBack(void*callbackdata, int ncols, char** values, char** headers);
private:
	CSqllitDB* m_pSqlDB;                  // 数据库操作类实例指针
	std::string m_strDBPath;              // 当前数据库的路径
	char m_srCurrOpenDBName[MAX_PATH];    // 当前打开的数据库
	char* m_pDBList[MAX_PATH];            // 当前存在所有数据库列表

};

#endif // CONSOLMAIN_H
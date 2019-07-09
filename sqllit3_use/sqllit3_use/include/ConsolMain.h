#ifndef CONSOLMAIN_H
#define CONSOLMAIN_H

class CConsolMain
{
public:
	CConsolMain();
	virtual ~CConsolMain();

public:
	int Process();
private:
	void CreateMainMenu();      // 创建主菜单
	void CreateDB();            // 创建数据库表

private:
	CSqllitDB* m_pSqlDB;        // 数据库操作类实例指针

};

#endif // CONSOLMAIN_H
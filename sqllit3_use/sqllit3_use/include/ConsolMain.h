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
	void CreateMainMenu();      // �������˵�
	void CreateDB();            // �������ݿ��

private:
	CSqllitDB* m_pSqlDB;        // ���ݿ������ʵ��ָ��

};

#endif // CONSOLMAIN_H
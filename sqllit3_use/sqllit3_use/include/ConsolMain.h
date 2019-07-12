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
	void InitDBPath();          // ��ʼ�����ݿ�·��
	void UpdateDBList();        // �������ݿ��б�
	void CleanDBList();         // ɾ�����ݿ��б� 
	void CreateMainMenu();      // �������˵�
	void CreateDB();            // �������ݿ�
	void SeeAllDB();            // �鿴�������ݿ�

	int ModifySqlDB();          // �޸����ݿ�
	void CreateDBTable();       // �������ݿ��
	void AddItemToDBTable();    // �������
	void UpdateItemToDBTable(); // �������
	void DelItemToDBTable();    // ɾ������
	void SeeAllInfo();          // �鿴��ǰ������г�Ա

	static int DBCallBack(void*callbackdata, int ncols, char** values, char** headers);
private:
	CSqllitDB* m_pSqlDB;                  // ���ݿ������ʵ��ָ��
	std::string m_strDBPath;              // ��ǰ���ݿ��·��
	char m_srCurrOpenDBName[MAX_PATH];    // ��ǰ�򿪵����ݿ�
	char* m_pDBList[MAX_PATH];            // ��ǰ�����������ݿ��б�

};

#endif // CONSOLMAIN_H
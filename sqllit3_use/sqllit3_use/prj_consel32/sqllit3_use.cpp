// sqllit3_use.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ConsolMain.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CConsolMain* pmain = new CConsolMain();
	while (pmain->StartProcess());
	return (int)0;
}


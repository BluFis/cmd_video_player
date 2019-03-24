#include "pch.h"
#include "welcome.h"

welcome::welcome()
{
	HWND hq = FindWindow("ConsoleWindowClass", NULL);
	ShowWindow(hq, SW_MAXIMIZE);
	SetConsoleTitle(LPCSTR("cmd video player"));
	HANDLE hcsb = CreateFileA("CONOUT$", GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	/*COORD size;
	size.X = 200;
	size.Y = 70;
	SetConsoleScreenBufferSize(hcsb, size);*/
	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	GetCurrentConsoleFontEx(hcsb, FALSE, &cfi);
	cfi.dwFontSize.Y = 20;
	SetCurrentConsoleFontEx(hcsb, FALSE, &cfi);
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(hcsb, &bInfo);
	this->print((bInfo.dwMaximumWindowSize.X - 70) / 2, (bInfo.dwMaximumWindowSize.Y - 25) / 2, string(70, '/'));
	for (short i = (bInfo.dwMaximumWindowSize.Y - 25) / 2 +1; i <= (bInfo.dwMaximumWindowSize.Y - 25) / 2 + 24; i++) {
		this->print((bInfo.dwMaximumWindowSize.X - 70) / 2, i, string(1, '/'));
		this->print((bInfo.dwMaximumWindowSize.X - 70) / 2 + 70, i, string(1, '/'));
	}
	this->print((bInfo.dwMaximumWindowSize.X - 70) / 2, (bInfo.dwMaximumWindowSize.Y - 25) / 2 + 25, string(70, '/'));
	this->print((bInfo.dwMaximumWindowSize.X - 70) / 2 + 20, (bInfo.dwMaximumWindowSize.Y - 25) / 2 + 7, "Command Line Video Player V1.1");
	this->print((bInfo.dwMaximumWindowSize.X - 70) / 2 + 21, (bInfo.dwMaximumWindowSize.Y - 25) / 2 + 15, "������Ƶ�ļ����س�����ʼ����");
	this->print((bInfo.dwMaximumWindowSize.X - 70) / 2 + 29, (bInfo.dwMaximumWindowSize.Y - 25) / 2 + 17, "����exit�˳�");
	this->print((bInfo.dwMaximumWindowSize.X - 70) / 2 + 22, (bInfo.dwMaximumWindowSize.Y - 25) / 2 + 19, "ע�⣡·���в��ܳ������ģ�");
	this->print((bInfo.dwMaximumWindowSize.X - 70) / 2 + 2, (bInfo.dwMaximumWindowSize.Y - 25) / 2 + 23, "programmed by Blufis С��ҲҪcode@BILIBILI");
	COORD top;
	top.X = (bInfo.dwMaximumWindowSize.X - 70) / 2 + 5;
	top.Y = (bInfo.dwMaximumWindowSize.Y - 25) / 2 + 27;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), top);
	
}

void welcome::print(short x, short y, string str)
{
	COORD top;
	top.X = x;
	top.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), top);
	std::cout << str << endl;
}

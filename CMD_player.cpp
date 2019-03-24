#include "pch.h"
#include "CMD_player.h"


CMD_player::CMD_player(videoStream * vs)
{
	Sleep(500);
	system("CLS");
	this->vs = vs;
	this->current = vs->next;
	this->hq = FindWindow(NULL, "cmd video player");
	HANDLE hcsb = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hcsb, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = false; //隐藏控制台光标
	CursorInfo.dwSize = 0;
	SetConsoleCursorInfo(hcsb, &CursorInfo);//设置控制台光标状态
	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	GetCurrentConsoleFontEx(hcsb, FALSE, &cfi);
	cfi.dwFontSize.Y = 6;
	SetCurrentConsoleFontEx(hcsb, FALSE, &cfi);
	PCOORD dwOldMode = 0;
	SetConsoleDisplayMode(hcsb, CONSOLE_FULLSCREEN_MODE, dwOldMode);
	RECT rect;
	GetWindowRect(hq, &rect);
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(hcsb, &bInfo);
	COORD NewSBSize;
	NewSBSize.X = bInfo.dwMaximumWindowSize.X;
	NewSBSize.Y = bInfo.dwMaximumWindowSize.Y;
	SetConsoleScreenBufferSize(hcsb, NewSBSize);
	this->pos.X = (bInfo.dwMaximumWindowSize.X - this->vs->width) / 2-1;
	this->pos.Y = (bInfo.dwMaximumWindowSize.Y - 2*this->vs->height) / 2-1;
	this->processpos.X = this->pos.X;
	this->processpos.Y = this->pos.Y + 2 * this->vs->height + 5;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), this->processpos);
	cout << string(this->vs->width,'=');
}

char CMD_player::img_to_txt(int gray)
{
	string d(" .,-'`:!1+*abcdefghijklmnopqrstuvwxyz<>(){}[]?234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ%&@#$");
	int length = charsetL;
	int index = 257 / length;
	return d[int(gray / index)];
}

void CMD_player::gotoxy()
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), this->pos);
}

void CMD_player::operator ()()
{
	while (true) {
		if (this->current->next==NULL) {
			break;
		}
		cv::Mat img = videostream().readFrame(this->current);
		if (img.empty()) {
			std::cout << "loading image failed" << endl; system("pause");
		}
		
		string out="";
		for (int i = 0; i < img.rows; i++) {

			for (int j = 0; j <= img.cols; j++) {
				//this->gotoxy(i, j);
				if (j%img.cols == 0) {
					
					out += '\n';
					out += string(this->pos.X, ' ');
					//cout << '\n';
				}else {
					out += this->img_to_txt(int(0.114*img.at<unsigned char>(i, j * 3) + 0.587*img.at<unsigned char>(i, j * 3 + 1) + 0.299*img.at<unsigned char>(i, j * 3 + 2)));
					//cout << this->img_to_txt(img.at<unsigned char>(i, j));
				}
				
			}
			//int(0.114*img.at<unsigned char>(i, j * 3) + 0.587*img.at<unsigned char>(i, j * 3 + 1) + 0.299*img.at<unsigned char>(i, j * 3 + 2))
		}
		this->gotoxy();
		cout << out;
		float percent = float(this->current->index) / float(this->vs->totalFrame);
		COORD prepos;
		prepos.X = this->pos.X + int(percent * this->vs->width);
		prepos.Y = this->pos.Y + 2 * this->vs->height + 5;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), prepos);
		cout << "> ";
		videostream().deleteFrame(this->vs);
		Sleep(85);
		if (GetAsyncKeyState(VK_ESCAPE) && 0x8000) {
			break;
		}
		if (this->vs->next!=NULL&&(&(this->vs->next->frame)) != NULL) {
			this->current = this->vs->next;
		}
		else {
			break;		
		}
	}
}

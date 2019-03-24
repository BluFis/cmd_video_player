#include "pch.h"
#include "Audio_player.h"


Audio_player::Audio_player()
{
	this->player = CreateZPlay();
	
}

void Audio_player::operator()()
{
	int result = this->player->OpenFile("temp.aac", sfAutodetect);
	if (result == 0)
	{
		cout << "Error: %s\n" << player->GetError() << endl;
		this->player->Release();
		return;
	}
	this->player->Play();
	while (1)
	{
		if (GetAsyncKeyState(VK_ESCAPE) && 0x8000) {
			break;
		}

		TStreamStatus status;
		this->player->GetStatus(&status);
		if (status.fPlay == 0)
			break; 
		Sleep(300); 
	}

	this->player->Release();
}


Audio_player::~Audio_player()
{
}

#include "pch.h"
#include "CMD_main.h"
using namespace std;
int main()
{
	
	while (1) {
		
		welcome wc = welcome();
		string url="";
		cin >> url;
		if (url == "exit") {
			return 0;
		}
		videoStream * vs = videostream().create();
		string filename = url;
		std::thread FrameProsser((FrameProsser(filename, vs)));
		FrameProsser.detach();
		Sleep(500);
		std::thread CMD_player((CMD_player(vs)));
		CMD_player.join();
	}
	
}


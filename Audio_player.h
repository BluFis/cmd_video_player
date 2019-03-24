#pragma once
#include "libzplay.h"
#include <windows.h>  
#include <conio.h> 
#include <iostream>
using namespace std;
using namespace libZPlay;
class Audio_player
{
public:
	Audio_player();
	void operator()();
	~Audio_player();
	ZPlay *player;
};


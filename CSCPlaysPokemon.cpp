// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#define WINVER 0x500

#include "stdafx.h"
#include <windows.h>
#include <queue>
#include <stdio.h>

class bPress {
	INPUT ip;
	int typePress;
public:
	bPress(int typePress) {
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = 0;
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;
		this->typePress = typePress;
	}
public:
	void exec() {
		ip.ki.wVk = typePress;
		ip.ki.dwFlags = 0;
		SendInput(1, &ip, sizeof(INPUT));
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	std::queue<bPress> inputs;
	inputs.push(bPress(0x41));
	inputs.push(bPress(0x42));
	Sleep(5000);
	while (!inputs.empty()) {
		inputs.front().exec();
		inputs.pop();
		Sleep(500);
	}
	return 0;
}
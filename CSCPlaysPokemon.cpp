#define WINVER 0x500
#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"
#include <WinSock2.h>
#include <windows.h>
#include <queue>
#include <stdio.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class bPress {
	INPUT ip;
public:
	int typePress;
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

std::queue<bPress> inputs;

int getComments() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	SOCKET ConnectSocket;
	addrinfo *result;
	getaddrinfo("irc.twitch.tv","6667",NULL,&result);
	ConnectSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	connect(ConnectSocket,result->ai_addr,(int)result->ai_addrlen);
	int iResult;
	char recvbuf[100];
	int found = 0;
	send(ConnectSocket, "PASS oauth:tihk65dtz8bw1ow3qzwh92bwmsk9lze\r\n", strlen("PASS oauth:tihk65dtz8bw1ow3qzwh92bwmsk9lze\r\n"),NULL);
	send(ConnectSocket, "NICK EricFalkenberg\r\n", strlen("NICK EricFalkenberg\r\n"), NULL);
	send(ConnectSocket, "USER custom 0 0 e_falk\r\n", strlen("USER custom 0 0 e_falk\r\n"), NULL);
	send(ConnectSocket, "JOIN #thelogcat\r\n", strlen("JOIN #twitchplayspokemon\r\n"), NULL);
	do {
		if (strstr(recvbuf,"start") != NULL) {
			inputs.push(bPress(0x0D));
			found = 1;
		}
		else if (strstr(recvbuf,"up") != NULL && !found) {
			inputs.push(bPress(0x26));
			found = 1;
		}
		else if (strstr(recvbuf,"down") != NULL && !found) {
			inputs.push(bPress(0x28));
			found = 1;
		}
		else if (strstr(recvbuf,"left") != NULL && !found) {
			inputs.push(bPress(0x25));
			found = 1;
		}
		else if (strstr(recvbuf,"right") != NULL && !found) {
			inputs.push(bPress(0x27));
			found = 1;
		}
		else if (strstr(recvbuf,"a") != NULL && !found) {
			inputs.push(bPress(0x41));
			found = 1;
		}
		else if (strstr(recvbuf,"b") != NULL && !found) {
			inputs.push(bPress(0x42));
			found = 1;
		}
		found = 0;
		for (int i = 0; i < 100; i++) {
			recvbuf[i] = '\0';
		}
		iResult = recv(ConnectSocket, recvbuf, 100, 0);
		if (iResult > 0) {
			printf("%d\n",inputs.size());
		}
		else if (iResult == 0) {
			printf("Connection closed\n");
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
		}
	} while (iResult > 0); 
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	getComments();
	/*
	inputs.push(bPress(0x41));
	inputs.push(bPress(0x42));
	Sleep(5000);
	while (!inputs.empty()) {
		inputs.front().exec();
		inputs.pop();
		Sleep(500);
	}
	return 0;
	*/
}


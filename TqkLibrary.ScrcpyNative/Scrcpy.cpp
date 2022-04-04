#include "pch.h"
#include "ProcessWrapper.h"
#include "ws2tcpip.h"
#include "Scrcpy.h"

const wchar_t* adbPath = L"adb.exe";
const int portMin = 5000;
const int portMax = 65535;
const int sockTimeoutSecond = 5;
SOCKET CreateListenSock(int port) {
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		return INVALID_SOCKET;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	InetPton(AF_INET,L"127.0.0.1",&addr.sin_addr);
	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		closesocket(sock);
		return INVALID_SOCKET;
	}

	if (listen(sock, 1) == SOCKET_ERROR) {
		closesocket(sock);
		return INVALID_SOCKET;
	}

	return sock;
}

SOCKET FindPort(int& port, int maxTry = 100) {
	SOCKET sock = INVALID_SOCKET;
	port = -1;
	for (int i = 0; i < maxTry && sock == INVALID_SOCKET; i++) {
		int range = portMax - portMin + 1;
		port = rand() % range + portMin;
		sock = CreateListenSock(port);
	}
	
	timeval timeout = { sockTimeoutSecond, 0 };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(sock, &fds);
	select(sock + 1, &fds, NULL, NULL, &timeout);
	
	return sock;
}

SOCKET AcceptConnection(SOCKET sock)
{
	sockaddr_in addr;
	int addrLen = sizeof(addr);
	SOCKET client = accept(sock, (sockaddr*)&addr, &addrLen);
	if (client == INVALID_SOCKET) {
		return INVALID_SOCKET;
	}
	return client;
}


Scrcpy::Scrcpy(LPCWSTR deviceId) {
	this->_deviceId = deviceId;
}

Scrcpy::~Scrcpy() {
	if (this->_process != nullptr)
	{
		delete this->_process;
		this->_process = nullptr;
	}
}

bool Scrcpy::Connect(LPCWSTR config, const ScrcpyNativeConfig* nativeConfig) {

	RunAdbProcess(L"reverse --remove localabstract:scrcpy");

	int port = -1;
	SOCKET sock = FindPort(port);
	if (sock == INVALID_SOCKET) 
		return false;

	RunAdbProcess(L"push scrcpy-server /sdcard/scrcpy-server-tqk.jar");
	
	
	std::wstring arg(L"reverse localabstract:scrcpy tcp:");
	arg += port;
	RunAdbProcess(arg.c_str());

	
	//run main process
	LPCWSTR cmds[]
	{
		L"-s",
		this->_deviceId.c_str(),
		L"shell CLASSPATH=/sdcard/scrcpy-server-tqk.jar",
		L"app_process / com.genymobile.scrcpy.Server",
		config
	};
	std::wstring args(adbPath);
	for (int i = 0; i < 5; i++)
	{
		args.append(L" ");
		args.append(cmds[i]);
	}
	this->_process = new ProcessWrapper((LPWSTR)args.c_str());

		
	this->_video = AcceptConnection(sock);
	if (this->_video == INVALID_SOCKET) {
		closesocket(sock);
		return false;
	}
	if (nativeConfig->IsControl) {
		this->_control = AcceptConnection(sock);
		if (this->_control == INVALID_SOCKET) {
			closesocket(this->_video);
			closesocket(sock);
			return false;
		}
	}

	//work with socket in thread









	closesocket(sock);
	return true;
}





void Scrcpy::RunAdbProcess(LPCWSTR argument)
{
	LPCWSTR cmds[]
	{
		L"-s",
		this->_deviceId.c_str(),
		argument
	};
	std::wstring args(adbPath);
	for (int i = 0; i < 3; i++)
	{
		args.append(L" ");
		args.append(cmds[i]);
	}
	ProcessWrapper p((LPWSTR)args.c_str());
	p.WaitForExit();
}

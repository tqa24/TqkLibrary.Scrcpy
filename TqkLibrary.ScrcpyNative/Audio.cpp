#include "pch.h"
#include "Scrcpy_pch.h"

Audio::Audio(const Scrcpy* scrcpy, SOCKET sock, const ScrcpyNativeConfig& nativeConfig) {
	this->_scrcpy = scrcpy;
	this->_audioSock = new SocketWrapper(sock);
	this->_audioBuffer = new BYTE[32];
}
Audio::~Audio() {
	delete this->_audioBuffer;
	delete this->_audioSock;
	CloseHandle(this->_threadHandle);
}
void Audio::Start() {
	//
	this->_threadHandle =
		CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			MyThreadFunction,       // thread function name
			this,					// argument to thread function 
			0,                      // use default creation flags 
			&_threadId);
}
void Audio::Stop() {
	this->_audioSock->Stop();
	this->_isStop = true;
	if (this->_threadHandle != INVALID_HANDLE_VALUE)
		WaitForSingleObject(this->_threadHandle, INFINITE);
}
bool Audio::Init() {
	return true;
}

DWORD Audio::MyThreadFunction(LPVOID lpParam) {
	Audio* audio = (Audio*)lpParam;
	audio->threadStart();
	audio->_isStopped = true;
	return 0;
}
void Audio::threadStart() {
	while (!this->_isStop)
	{
		if (this->_audioSock->ReadAll(this->_audioBuffer, 32) < 0) {
			return;
		}
	}
}
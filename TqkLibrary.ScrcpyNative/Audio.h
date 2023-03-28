#ifndef _H_Audio_H_
#define _H_Audio_H_
class Audio
{
public:
	Audio(const Scrcpy* scrcpy, SOCKET sock, const ScrcpyNativeConfig& nativeConfig);
	~Audio();
	void Start();
	void Stop();
	bool Init();

private:
	const Scrcpy* _scrcpy;

	bool _isStopped = false;

	BYTE* _audioBuffer{ nullptr };
	SocketWrapper* _audioSock{ nullptr };





	void threadStart();
	bool _isStop = false;
	DWORD _threadId{ 0 };
	HANDLE _threadHandle{ INVALID_HANDLE_VALUE };

	static DWORD WINAPI MyThreadFunction(LPVOID lpParam);
};
#endif


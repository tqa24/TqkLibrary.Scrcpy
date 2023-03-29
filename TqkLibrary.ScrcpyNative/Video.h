#ifndef _H_Video_H_
#define _H_Video_H_

class Video
{
	friend Scrcpy;
public:
	Video(const Scrcpy* scrcpy, SOCKET sock, const ScrcpyNativeConfig& nativeConfig);
	~Video();
	void Start();
	void Stop();
	bool Init();
	bool WaitForFirstFrame(DWORD timeout);

	bool GetScreenSize(int& w, int& h);
	bool GetCurrentRgbaFrame(AVFrame* frame);
private:
	ScrcpyNativeConfig _nativeConfig{};
	const Scrcpy* _scrcpy;

	bool _isStopped{ false };

	bool _ishaveFrame{ false };
	ParsePacket* _parsePacket{ nullptr };
	VideoDecoder* _videoDecoder{ nullptr };
	SocketWrapper* _videoSock{ nullptr };
	BYTE* _videoBuffer{ nullptr };

	HANDLE _mtx_waitFirstFrame{ INVALID_HANDLE_VALUE };

	void threadStart();

	bool _isStopMainLoop = false;
	DWORD _threadId{ 0 };
	HANDLE _threadHandle{ INVALID_HANDLE_VALUE };
	static DWORD WINAPI MyThreadFunction(LPVOID lpParam);
};

#endif // !Video_H
#ifndef _H_ScrcpyWorking_H_
#define _H_ScrcpyWorking_H_
class ScrcpyInstance
{
	friend Scrcpy;
public:
	ScrcpyInstance(const Scrcpy* scrcpy, const ScrcpyNativeConfig& nativeConfig);
	~ScrcpyInstance();
	bool Start();

private:
	bool _wsa_isStartUp = false;
	//function	
	DWORD RunAdbProcess(LPCWSTR argument);

	//const
	const Scrcpy* _scrcpy{ nullptr };//don't delete
	ScrcpyNativeConfig _nativeConfig{ };
	
	//need release
	SOCKET _listenSock{ INVALID_SOCKET };
	ProcessWrapper* _process{ nullptr };
	Video* _video{ nullptr };
	Control* _control{ nullptr };
};
#endif
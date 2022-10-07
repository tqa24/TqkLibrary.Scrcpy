#ifndef _H_SocketWrapper_H_
#define _H_SocketWrapper_H_
class SocketWrapper
{
public:
	SocketWrapper(SOCKET sock);
	~SocketWrapper();
	void Stop();
	int ReadAll(BYTE* buff, int length);
	int Write(const BYTE* buff, int length);
	bool ChangeBlockMode(bool isBlock);
	bool ChangeBufferSize(int sizeInByte);
private:
	SOCKET _sock;
};
#endif // !SocketWrapper_H
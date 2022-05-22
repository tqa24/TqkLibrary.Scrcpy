#include "pch.h"
#include <winsock2.h>
#include "SocketWrapper.h"

SocketWrapper::SocketWrapper(SOCKET sock) {
	assert(sock != INVALID_SOCKET);
	this->_sock = sock;
}

SocketWrapper::~SocketWrapper() {
	closesocket(this->_sock);
}

int SocketWrapper::ReadAll(BYTE* buff, int length) {
	return recv(this->_sock, (char*)buff, length, MSG_WAITALL);
}
int SocketWrapper::Write(const BYTE* buff, int length) {
	return send(this->_sock, (const char*)buff, length, NULL);//MSG_OOB
}

void SocketWrapper::Stop() {
	shutdown(this->_sock, SD_BOTH);
}

bool SocketWrapper::ChangeBlockMode(bool isBlock) {
	u_long iMode = isBlock ? 0 : 1;
	if (ioctlsocket(this->_sock, FIONBIO, &iMode) == SOCKET_ERROR) {
		return false;
	}
	return true;
}
bool SocketWrapper::ChangeBufferSize(int sizeInByte) {
	return setsockopt(this->_sock, SOL_SOCKET, SO_RCVBUF, (LPCSTR)&sizeInByte, sizeof(int)) != SOCKET_ERROR;
}
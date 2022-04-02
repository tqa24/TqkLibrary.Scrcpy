#ifndef Utils_H
#define Utils_H

static inline void arlet(const char* s) {
	::MessageBoxA(NULL, (LPCSTR)s, (LPCSTR)"ShaderErr", MB_OK);
}

//#define avcheck(x) if(x < 0){\
//	char buffer[1024];\
//	av_strerror(err, buffer, 1024);\
//	::MessageBoxA(NULL, (LPCSTR)buffer, (LPCSTR)"Libav", MB_OK);\
//	return false;\
//}


static inline bool avcheck(int err) {
	if (err < 0) {
		char buffer[1024];
		av_strerror(err, buffer, 1024);
		::MessageBoxA(NULL, (LPCSTR)buffer, (LPCSTR)"Libav", MB_OK);
		return false;
	}
	return true;
}









#endif // !Utils_H

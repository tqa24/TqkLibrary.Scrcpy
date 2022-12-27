#ifndef _H_ScrcpyNativeConfig_H_
#define _H_ScrcpyNativeConfig_H_
struct ScrcpyNativeConfig {
	BYTE HwType;//AVHWDeviceType
	bool ForceAdbForward;
	bool IsControl;
	bool IsUseD3D11Shader; 
	LPCWSTR AdbPath;
	LPCWSTR ScrcpyServerPath;
	LPCWSTR ConfigureArguments;
	INT32 ConnectionTimeout;
	D3D11_FILTER Filter;
};
#endif // !ScrcpyNativeConfig_H


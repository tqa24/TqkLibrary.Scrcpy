#ifndef ScrcpyNativeConfig_H
#define ScrcpyNativeConfig_H
struct ScrcpyNativeConfig {
	BYTE HwType;//AVHWDeviceType
	bool ForceAdbForward;
	bool IsControl;
	bool IsUseD3D11Shader;
	LPCWSTR ScrcpyServerPath;
	INT32 ConnectionTimeout;
	D3D11_FILTER Filter;
};
#endif // !ScrcpyNativeConfig_H


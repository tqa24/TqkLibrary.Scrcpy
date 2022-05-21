#ifndef ScrcpyNativeConfig_H
#define ScrcpyNativeConfig_H
struct ScrcpyNativeConfig {
	BYTE HwType;//AVHWDeviceType
	bool ForceAdbForward;
	bool IsControl;
	bool IsUseD3D11Shader;
	INT32 ConnectionTimeout;
};
#endif // !ScrcpyNativeConfig_H


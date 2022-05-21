#ifndef ScrcpyNativeExports_H
#define ScrcpyNativeExports_H

ScrcpyNativeExport BYTE FFmpegHWSupport(BYTE bHWSupport);
ScrcpyNativeExport bool ClearKey();
ScrcpyNativeExport bool AddKey(BYTE* key, const int sizeInByte);
ScrcpyNativeExport Scrcpy* ScrcpyAlloc(LPCWSTR deviceId);
ScrcpyNativeExport void ScrcpyFree(Scrcpy* scrcpy);
ScrcpyNativeExport bool ScrcpyConnect(Scrcpy* scrcpy, LPCWSTR config, const ScrcpyNativeConfig& nativeConfig);
ScrcpyNativeExport void ScrcpyStop(Scrcpy* scrcpy);
ScrcpyNativeExport bool ScrcpyGetScreenSize(Scrcpy* scrcpy, int& w, int& h);
ScrcpyNativeExport bool ScrcpyControlCommand(Scrcpy* scrcpy, const BYTE* command, const int sizeInByte);
ScrcpyNativeExport bool ScrcpyGetScreenShot(Scrcpy* scrcpy, BYTE* buffer, const int sizeInByte, const int w, const int h, const int lineSize);

typedef bool (*ClipboardReceivedDelegate)(BYTE* buffer, int length);
ScrcpyNativeExport bool RegisterClipboardEvent(Scrcpy* scrcpy, ClipboardReceivedDelegate clipboardDelegate);
#endif // !ScrcpyNativeExports_H

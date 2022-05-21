using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TqkLibrary.Scrcpy
{
    internal static class NativeWrapper
    {
        [DllImport("TqkLibrary.ScrcpyNative.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern byte FFmpegHWSupport(byte bHWSupport);

        [DllImport("TqkLibrary.ScrcpyNative.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool ClearKey();
        
        [DllImport("TqkLibrary.ScrcpyNative.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool AddKey(byte[] key, int sizeInByte);

        [DllImport("TqkLibrary.ScrcpyNative.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr ScrcpyAlloc(string deviceId);

        [DllImport("TqkLibrary.ScrcpyNative.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr ScrcpyFree(IntPtr intPtr);

        [DllImport("TqkLibrary.ScrcpyNative.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool ScrcpyConnect(IntPtr intPtr, string config, ref ScrcpyNativeConfig nativeConfig);

        [DllImport("TqkLibrary.ScrcpyNative.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void ScrcpyStop(IntPtr intPtr);

        [DllImport("TqkLibrary.ScrcpyNative.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool ScrcpyGetScreenSize(IntPtr intPtr, ref int w, ref int h);

        [DllImport("TqkLibrary.ScrcpyNative.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool ScrcpyControl(IntPtr intPtr, byte[] command, int sizeInByte);

        [DllImport("TqkLibrary.ScrcpyNative.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool ScrcpyGetScreenShot(IntPtr intPtr, IntPtr buffer, int sizeInByte, int w, int h, int lineSize);
    }
}

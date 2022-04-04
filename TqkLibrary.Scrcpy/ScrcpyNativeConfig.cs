using System.Runtime.InteropServices;

namespace TqkLibrary.Scrcpy
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct ScrcpyNativeConfig
    {
        public Orientations Orientation;//int
        public FFmpegAVHWDeviceType HwType;//int
        public int PacketBufferLength;
        public bool ForceAdbForward;
        public bool IsControl;
    }
}

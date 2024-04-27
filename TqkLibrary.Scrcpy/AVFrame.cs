using System;

namespace TqkLibrary.Scrcpy
{
    /// <summary>
    /// 
    /// </summary>
    public class AVFrame : IDisposable
    {
        IntPtr _handle = IntPtr.Zero;
        /// <summary>
        /// Frame pointer
        /// </summary>
        public IntPtr Handle { get { return _handle; } }
        /// <summary>
        /// 
        /// </summary>
        public AVFrame()
        {
            _handle = NativeWrapper.av_frame_alloc();
        }
        /// <summary>
        /// 
        /// </summary>
        ~AVFrame()
        {
            Dispose(false);
        }
        /// <summary>
        /// 
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        void Dispose(bool disposing)
        {
            if (_handle != IntPtr.Zero)
            {
                NativeWrapper.av_frame_free(ref _handle);
            }
        }
    }
}

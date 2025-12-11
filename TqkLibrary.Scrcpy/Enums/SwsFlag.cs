using System;
using System.Collections.Generic;
using System.Text;

namespace TqkLibrary.Scrcpy.Enums
{
#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
    public enum SwsFlag : uint
    {
        SWS_FAST_BILINEAR = 1,
        SWS_BILINEAR = 2,
        SWS_BICUBIC = 4,
        SWS_X = 8,
        SWS_POINT = 0x10,
        SWS_AREA = 0x20,
        SWS_BICUBLIN = 0x40,
        SWS_GAUSS = 0x80,
        SWS_SINC = 0x100,
        SWS_LANCZOS = 0x200,
        SWS_SPLINE = 0x400,
    }
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
}

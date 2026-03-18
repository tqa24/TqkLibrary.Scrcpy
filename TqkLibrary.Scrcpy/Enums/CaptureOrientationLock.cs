namespace TqkLibrary.Scrcpy.Enums
{
#pragma warning disable CS1591
    /// <summary>
    /// Lock mode for capture_orientation in scrcpy 3.0
    /// </summary>
    public enum CaptureOrientationLock
    {
        /// <summary>No lock — orientation can change freely</summary>
        Unlocked,
        /// <summary>Lock to the specified <see cref="CaptureOrientations"/> value (prefix "@")</summary>
        LockedValue,
        /// <summary>Lock to the initial device orientation at startup (value "@" alone)</summary>
        LockedInitial,
    }
#pragma warning restore CS1591
}

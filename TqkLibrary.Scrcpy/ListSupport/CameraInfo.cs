using System.Drawing;

namespace TqkLibrary.Scrcpy.ListSupport
{
    /// <summary>
    /// 
    /// </summary>
    public class CameraInfo
    {
        /// <summary>
        /// 
        /// </summary>
        public int CameraId { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public CameraFacing CameraFacing { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool IsHighSpeed { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string Size { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int FpsMax { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int FpsMin { get; set; }
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public Size? GetSize()
        {
            if (!string.IsNullOrWhiteSpace(Size))
            {
                var split = Size.Split('x');
                if (split.Length == 2)
                {
                    if (int.TryParse(split[0], out int w) && int.TryParse(split[1], out int h))
                    {
                        return new Size(w, h);
                    }
                }
            }
            return null;
        }
    }
}
